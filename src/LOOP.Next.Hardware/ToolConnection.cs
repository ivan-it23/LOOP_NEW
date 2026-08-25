using System.Collections.Generic;
using System.IO.Ports;
using System.Runtime.InteropServices;
using LoopNext.Core.Logging;
using LoopNext.Core.Models;
using LoopNext.Core.Protocol;
using LoopNext.Metrology;

namespace LoopNext.Hardware;

// Подключение и отключение прибора по единому COM-алгоритму.
// Перенос активной ветки button_ON_OFF_Click (WorkForm.h:5876-6028).
public static class ToolConnection
{
    private const int MetrologyPayloadLength = ToolProtocolConstants.ToolPayloadLength; // 242
    private const int ReadAttempts = 3;
    private const int PostCloseDelayMs = 100;
    private const int PostOpenDelayMs = 150;
    private const int PreReceiveDelayMs = 100;

    private const byte ReadSubCommand = 0x05;

    // Смещение, по которому кончается часть метрологии, общая для обоих
    // поколений прошивки. Дальше у старой служебная область, у новой —
    // воздушные поправки затухания.
    private const int MetrologyCommonSize = 84;

    public readonly record struct ToolConnectResult(
        bool Success,
        GpToolMetrology Metrology,
        IReadOnlyList<SondeParameter> SondeParameters,
        string? ErrorMessage = null);

    // Включение прибора: открытие порта на 125000 бод и чтение метрологии командой 0x13.
    // Успех определяется отсутствием исключения при открытии/обмене, как в эталоне
    // (on_off=true даже если метрология не считана; в этом случае параметры зонда пусты).
    public static ToolConnectResult Connect(SerialPort port)
    {
        AppLogger.Write("=== НАЧАЛО ВКЛЮЧЕНИЯ ПРИБОРА (единый COM-опрос) ===");

        // Новый сеанс связи снова выдаёт однократную диагностику: дамп первого
        // кадра и состав конвейера фаз. Иначе после смены прибора в журнале
        // остались бы данные предыдущего.
        ToolPort.ResetSessionDiagnostics();
        ToolPhaseProcessor.ResetSessionDiagnostics();
        try
        {
            if (port.IsOpen)
            {
                AppLogger.Write("Порт прибора уже открыт, закрываем перед переподключением.");
                port.Close();
            }

            Thread.Sleep(PostCloseDelayMs);

            port.BaudRate = ToolProtocolConstants.ToolBaudRate;
            port.ReadTimeout = ToolProtocolConstants.ReadTimeoutMs;
            port.WriteTimeout = ToolProtocolConstants.WriteTimeoutMs;
            AppLogger.Write($"Установка BaudRate = {ToolProtocolConstants.ToolBaudRate}, порт={port.PortName}");

            port.Open();
            AppLogger.Write("Порт прибора открыт успешно.");
            port.DiscardOutBuffer();
            Thread.Sleep(PostOpenDelayMs);

            AppLogger.Write("Чтение метрологии (команда 0x13)...");
            var metrology = ReadMetrologyOnConnect(port, out var metrologyPayload);

            port.DiscardOutBuffer();
            Thread.Sleep(PostCloseDelayMs);

            var sondeParameters = MetrologyConverter.Convert(in metrology);
            var identity = ToolIdentity.Decode(metrology.Signature);
            AppLogger.Write(
                $"Метрология прибора: Signature=0x{metrology.Signature:X8}, Serial={metrology.Serial}, " +
                $"N_Tx={identity.TransmitterCount}, Channels={sondeParameters.Count}, DeviceType={identity.DeviceType}, Valid={identity.IsValid}, " +
                $"Firmware={(identity.IsExtendedFirmware ? "new" : "legacy")}, FrameSize={identity.FrameSize}");
            LogMetrologyDetails(in metrology, metrologyPayload);
            AppLogger.Write("=== ПРИБОР УСПЕШНО ВКЛЮЧЕН ===");
            return new ToolConnectResult(true, metrology, sondeParameters);
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "ИСКЛЮЧЕНИЕ при открытии порта или чтении метрологии");
            // В отличие от эталона, неуспешное подключение освобождает порт и не удерживает COM-ресурс.
            if (port.IsOpen)
            {
                port.Close();
            }

            AppLogger.Write("ОШИБКА: Не удалось включить прибор!");
            return new ToolConnectResult(false, default, Array.Empty<SondeParameter>(), "Не удается открыть порт прибора!");
        }
    }

    // Выключение прибора: закрытие порта.
    public static void Disconnect(SerialPort port)
    {
        if (port.IsOpen)
        {
            AppLogger.Write("=== ВЫКЛЮЧЕНИЕ ПРИБОРА ===");
            port.Close();
            AppLogger.Write("Порт прибора закрыт.");
        }

        Thread.Sleep(PostCloseDelayMs);
    }

    // Полный разбор метрологии прибора: все поля структуры, а для новой
    // прошивки — ещё и воздушные поправки затухания, которые она разместила в
    // начале служебной области. Раскладка самой структуры здесь не меняется:
    // поправки читаются из принятой нагрузки только ради журнала.
    private static void LogMetrologyDetails(in GpToolMetrology metrology, ReadOnlySpan<byte> payload)
    {
        var identity = ToolIdentity.Decode(metrology.Signature);
        var txCount = (int)identity.TransmitterCount;
        var freqCount = GpToolMetrology.FrequencyCount;

        var l1Values = new List<string>(txCount);
        var l2Values = new List<string>(txCount);
        for (var tx = 0; tx < txCount; tx++)
        {
            l1Values.Add($"T{tx + 1}={metrology.GetL1(tx) / 1000.0:F3}");
            l2Values.Add($"T{tx + 1}={metrology.GetL2(tx) / 1000.0:F3}");
        }

        AppLogger.Write(
            $"METROLOGY_GEOMETRY: L1(м)=[{string.Join(", ", l1Values)}] L2(м)=[{string.Join(", ", l2Values)}] " +
            $"F1={metrology.GetFrequency(0)} кГц F2={metrology.GetFrequency(1)} кГц");
        AppLogger.Write(
            $"METROLOGY_IDENTITY: корпус={metrology.Serial} диаметр={metrology.SondeDiameterMm} мм " +
            $"work_type={metrology.WorkType} Rx_Position={metrology.RxPosition}");

        var airZz = new List<string>(freqCount * txCount);
        var minAmp = new List<string>(freqCount * txCount);
        for (var freq = 0; freq < freqCount; freq++)
        {
            for (var tx = 0; tx < txCount; tx++)
            {
                airZz.Add($"F{freq + 1}T{tx + 1}={metrology.GetAirZz(freq, tx)}");
                minAmp.Add($"F{freq + 1}T{tx + 1}={metrology.GetAirZzAmplitude(freq, tx)}");
            }
        }

        AppLogger.Write($"METROLOGY_AIR_PHASE: Air_zz=[{string.Join(", ", airZz)}]");
        AppLogger.Write($"METROLOGY_MIN_AMPLITUDE: min_amp=[{string.Join(", ", minAmp)}]");

        LogAirAttenuation(identity, payload, txCount, freqCount);

        AppLogger.Write(
            ToolSerialTransport.FormatHexDump("METROLOGY_DUMP", payload));
    }

    // Воздушные поправки затухания лежат по смещению 84 внутри метрологии и
    // существуют только у новой прошивки: у старой там нерасшифрованная
    // служебная область, и выводить её как калибровку нельзя.
    private static void LogAirAttenuation(
        ToolIdentity identity,
        ReadOnlySpan<byte> payload,
        int txCount,
        int freqCount)
    {
        if (!identity.IsExtendedFirmware)
        {
            AppLogger.Write("METROLOGY_AIR_ATTENUATION: прибор старой прошивки, поправок затухания нет");
            return;
        }

        var structure = payload[ToolProtocolConstants.ToolServicePrefixLength..];
        if (structure.Length < ToolProtocolConstants.ToolMetrologySize)
        {
            return;
        }

        var values = new List<string>(freqCount * txCount);
        for (var freq = 0; freq < freqCount; freq++)
        {
            for (var tx = 0; tx < txCount; tx++)
            {
                var offset = MetrologyCommonSize + ((freq * GpToolMetrology.MaxTransmitterCount + tx) * sizeof(float));
                var value = BitConverter.ToSingle(structure.Slice(offset, sizeof(float)));
                values.Add($"F{freq + 1}T{tx + 1}={value:F4}");
            }
        }

        AppLogger.Write($"METROLOGY_AIR_ATTENUATION (дБ): air_att_dB=[{string.Join(", ", values)}]");
    }

    // Чтение метрологии при подключении: 3 попытки команды 0x13 с паузой перед приёмом.
    private static GpToolMetrology ReadMetrologyOnConnect(SerialPort port, out byte[] receivedPayload)
    {
        Span<byte> payload = stackalloc byte[] { ReadSubCommand, 0x00 };
        var dataIn = new byte[MetrologyPayloadLength];
        receivedPayload = dataIn;
        var result = ToolResponseCode.Timeout;
        var metrology = default(GpToolMetrology);

        for (var attempt = 0; attempt < ReadAttempts && result != ToolResponseCode.Ok; attempt++)
        {
            if (ToolSerialTransport.Send(port, ToolProtocolConstants.CmdGetMetrology, payload, 2) != 0)
            {
                AppLogger.Write($"ОШИБКА: send_to_tool вернул ошибку (попытка {attempt + 1})");
                continue;
            }

            Thread.Sleep(PreReceiveDelayMs);
            result = (ToolResponseCode)ToolSerialTransport.Receive(port, out _, dataIn, MetrologyPayloadLength);
            if (result == ToolResponseCode.Ok)
            {
                metrology = MemoryMarshal.Read<GpToolMetrology>(dataIn.AsSpan(2));
                AppLogger.Write($"Метрология получена (попытка {attempt + 1}).");
            }
            else if (result == ToolResponseCode.Timeout)
            {
                AppLogger.Write($"ОШИБКА: receive_from_tool timeout (попытка {attempt + 1})");
            }
            else if (result == ToolResponseCode.CrcError)
            {
                AppLogger.Write($"ОШИБКА: receive_from_tool CRC error (попытка {attempt + 1})");
            }
        }

        return metrology;
    }
}
