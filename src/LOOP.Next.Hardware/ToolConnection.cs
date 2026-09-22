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
    private const int ReadAttempts = 2;
    private const int PostCloseDelayMs = 100;
    private const int PostOpenDelayMs = 150;
    private const int PreReceiveDelayMs = 100;

    private const byte ReadSubCommand = 0x05;

    public readonly record struct ToolConnectResult(
        bool Success,
        GpToolMetrology Metrology,
        IReadOnlyList<SondeParameter> SondeParameters,
        string? ErrorMessage = null,
        uint ProbedSignature = 0);

    // Включение прибора: открытие порта на 125000 бод, проба сигнатуры командой
    // 0x16 и чтение метрологии командой 0x13. Успех определяется отсутствием
    // исключения при открытии/обмене, как в эталоне (on_off=true даже если
    // метрология не считана; в этом случае параметры зонда пусты).
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

            // Сигнатура из 0x16 — единственная идентификация, не зависящая от
            // того, размечена ли метрология. Она же называет транспорт: Картограф
            // в режиме LWD 4tx этой команды не имеет и молчит.
            var probedSignature = ProbeSignature(port);
            var probedIdentity = ToolIdentity.Decode(probedSignature);

            // Транспорт без поля длины остался только у Картографа в режиме
            // LWD 4tx со старой прошивкой. После перепрошивки он отвечает как
            // все: прибор 320349001 прислал кадр с полем длины 325 и ответил на
            // 0x16, которого у него прежде не было.
            //
            // Молчание на 0x16 тоже указывает на старый Картограф, поэтому при
            // нём чтение метрологии начинается с фиксированной длины. Для
            // обычного прибора порядок безразличен: пакет метрологии в 240 байт
            // у обоих транспортов побайтово одинаков.
            var preferFixedLength = probedSignature == 0 || ToolPort.UsesFixedLengthTransport(in probedIdentity);

            AppLogger.Write("Чтение метрологии (команда 0x13)...");
            var metrology = ReadMetrologyOnConnect(port, preferFixedLength, out var metrologyStructure);

            port.DiscardOutBuffer();
            Thread.Sleep(PostCloseDelayMs);

            var sondeParameters = MetrologyConverter.Convert(in metrology);
            var identity = ToolIdentity.Decode(metrology.Signature);
            AppLogger.Write(
                $"Метрология прибора: Signature=0x{metrology.Signature:X8}, Serial={metrology.Serial}, " +
                $"N_Tx={identity.TransmitterCount}, Channels={sondeParameters.Count}, DeviceType={identity.DeviceType}, Valid={identity.IsValid}, " +
                $"Firmware={(identity.IsExtendedFirmware ? "new" : "legacy")}, FrameSize={identity.FrameSize}");
            LogSignatureCrossCheck(probedSignature, in metrology);
            LogMetrologyDetails(in metrology, metrologyStructure);
            AppLogger.Write("=== ПРИБОР УСПЕШНО ВКЛЮЧЕН ===");
            return new ToolConnectResult(true, metrology, sondeParameters, null, probedSignature);
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

    // Проба сигнатуры. Молчание не является ошибкой подключения: у Картографа в
    // режиме LWD 4tx обработчика этой команды нет.
    private static uint ProbeSignature(SerialPort port)
    {
        var result = ToolPort.ReadSignature(port);
        if (result.Code != ToolResponseCode.Ok)
        {
            AppLogger.Write(
                $"TOOL_SIGNATURE_PROBE: result={result.Code} — прибор не ответил на 0x16; " +
                "это ожидаемо для Картографа в режиме LWD 4tx, для остальных приборов — повод посмотреть линию");
            return 0;
        }

        var identity = ToolIdentity.Decode(result.Signature);
        AppLogger.Write(
            $"TOOL_SIGNATURE_PROBE: signature=0x{result.Signature:X8} ({result.Signature}) | " +
            $"тип={identity.DeviceType} Tx={identity.TransmitterCount} мод.{identity.Modification} № {identity.Number} | " +
            $"кадр={identity.FrameSize}Б валидна={identity.IsValid}");
        return result.Signature;
    }

    // Сверка сигнатуры из 0x16 с сигнатурой метрологии. Расхождение означает,
    // что прибор и его паспорт описывают разные приборы.
    private static void LogSignatureCrossCheck(uint probedSignature, in GpToolMetrology metrology)
    {
        if (probedSignature == 0 || metrology.Signature == 0)
        {
            return;
        }

        if (probedSignature == metrology.Signature)
        {
            AppLogger.Write(
                $"TOOL_SIGNATURE_MATCH: 0x{probedSignature:X8} — сигнатура прибора и метрологии совпадают");
            return;
        }

        AppLogger.Write(
            $"TOOL_SIGNATURE_MISMATCH: probe=0x{probedSignature:X8} ({probedSignature}) " +
            $"metrology=0x{metrology.Signature:X8} ({metrology.Signature}) — " +
            "прибор и его метрология описывают разные приборы");
    }

    // Полный разбор метрологии прибора: все поля структуры, а для новой
    // прошивки — ещё и поправки затухания с отметкой времени записи, которые она
    // разместила в начале служебной области.
    private static void LogMetrologyDetails(in GpToolMetrology metrology, ReadOnlySpan<byte> structure)
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

        LogExtendedMetrologyFields(identity, structure, txCount, freqCount);

        AppLogger.Write(ToolSerialTransport.FormatHexDump("METROLOGY_DUMP", structure));
    }

    // Поправки затухания лежат по смещению 84, отметка времени — по 124, и
    // существуют только у новой прошивки: у старой там нерасшифрованная
    // служебная область, и выводить её как калибровку нельзя.
    private static void LogExtendedMetrologyFields(
        ToolIdentity identity,
        ReadOnlySpan<byte> structure,
        int txCount,
        int freqCount)
    {
        if (!identity.IsExtendedFirmware)
        {
            AppLogger.Write(
                "METROLOGY_EXTENDED_FIELDS: прибор старой прошивки, поправок затухания и отметки времени нет");
            return;
        }

        if (structure.Length < ExtendedMetrologyFields.MetrologySize)
        {
            return;
        }

        var values = new List<string>(freqCount * txCount);
        for (var freq = 0; freq < freqCount; freq++)
        {
            for (var tx = 0; tx < txCount; tx++)
            {
                values.Add(
                    ExtendedMetrologyFields.TryReadAirAttenuationDb(structure, freq, tx, out var value)
                        ? $"F{freq + 1}T{tx + 1}={value:F4}"
                        : $"F{freq + 1}T{tx + 1}=—");
            }
        }

        AppLogger.Write($"METROLOGY_AIR_ATTENUATION (дБ): air_att_dB=[{string.Join(", ", values)}]");

        // Отметку ставит записывающая программа по часам компьютера, а не прибор.
        AppLogger.Write(
            ExtendedMetrologyFields.TryReadWriteTimestamp(structure, out var timestamp)
                ? $"METROLOGY_WRITE_TIMESTAMP: {timestamp.UtcDateTime:yyyy-MM-dd HH:mm:ss} UTC " +
                  $"({timestamp.ToLocalTime():yyyy-MM-dd HH:mm:ss} местное)"
                : "METROLOGY_WRITE_TIMESTAMP: отметки нет (метрология записана до появления поля либо память не размечена)");
    }

    /// <summary>
    /// Чтение метрологии при подключении. Возвращает структуру и её байты.
    /// </summary>
    /// <remarks>
    /// Метрология занимает 240 байт в обоих поколениях, и оба отвечают
    /// length-framed пакетом с <c>LEN = 245</c>. Ветка фиксированной длины
    /// остаётся для Картографа в режиме LWD 4tx, который вместо поля длины шлёт
    /// два служебных байта. Порядок веток задаёт проба сигнатуры; если она
    /// молчала, пробуются обе — так подключение переживает и неразмеченную
    /// метрологию, и молчащий 0x16.
    /// </remarks>
    private static GpToolMetrology ReadMetrologyOnConnect(
        SerialPort port,
        bool preferFixedLength,
        out byte[] structure)
    {
        var order = preferFixedLength
            ? new[] { true, false }
            : new[] { false, true };

        foreach (var useFixedLength in order)
        {
            for (var attempt = 0; attempt < ReadAttempts; attempt++)
            {
                var code = useFixedLength
                    ? TryReadMetrologyFixedLength(port, out structure)
                    : TryReadMetrologyFramed(port, out structure);

                if (code == ToolResponseCode.Ok)
                {
                    AppLogger.Write(
                        $"Метрология получена ({(useFixedLength ? "фиксированная длина" : "поле длины")}, попытка {attempt + 1}).");
                    return MemoryMarshal.Read<GpToolMetrology>(structure);
                }

                AppLogger.Write(
                    $"ОШИБКА чтения метрологии: result={code}, транспорт={(useFixedLength ? "fixed" : "framed")}, попытка {attempt + 1}");
            }
        }

        structure = new byte[ToolProtocolConstants.ToolMetrologySize];
        return default;
    }

    private static ToolResponseCode TryReadMetrologyFramed(SerialPort port, out byte[] structure)
    {
        structure = new byte[ToolProtocolConstants.ToolMetrologySize];
        port.DiscardInBuffer();
        if (ToolSerialTransport.SendFramed(port, ToolProtocolConstants.CmdGetMetrology, ReadOnlySpan<byte>.Empty) != 0)
        {
            return ToolResponseCode.Timeout;
        }

        Thread.Sleep(PreReceiveDelayMs);
        var code = ToolSerialTransport.ReceiveFramed(port, out var cmd, out var payload);
        if (code != ToolResponseCode.Ok)
        {
            return code;
        }

        if (cmd != ToolProtocolConstants.CmdGetMetrology || payload.Length != ToolProtocolConstants.ToolMetrologySize)
        {
            AppLogger.Write(
                $"METROLOGY_FRAMED_REJECTED: cmd=0x{cmd:X2} payload={payload.Length} " +
                $"expected_cmd=0x{ToolProtocolConstants.CmdGetMetrology:X2} expected_payload={ToolProtocolConstants.ToolMetrologySize}");
            return ToolResponseCode.InvalidResponse;
        }

        structure = payload;
        return ToolResponseCode.Ok;
    }

    private static ToolResponseCode TryReadMetrologyFixedLength(SerialPort port, out byte[] structure)
    {
        structure = new byte[ToolProtocolConstants.ToolMetrologySize];
        Span<byte> request = stackalloc byte[] { ReadSubCommand, 0x00 };
        var dataIn = new byte[MetrologyPayloadLength];

        port.DiscardInBuffer();
        if (ToolSerialTransport.Send(port, ToolProtocolConstants.CmdGetMetrology, request, request.Length) != 0)
        {
            return ToolResponseCode.Timeout;
        }

        Thread.Sleep(PreReceiveDelayMs);
        var code = (ToolResponseCode)ToolSerialTransport.Receive(port, out _, dataIn, MetrologyPayloadLength);
        if (code != ToolResponseCode.Ok)
        {
            return code;
        }

        dataIn.AsSpan(ToolProtocolConstants.ToolServicePrefixLength).CopyTo(structure);
        return ToolResponseCode.Ok;
    }
}
