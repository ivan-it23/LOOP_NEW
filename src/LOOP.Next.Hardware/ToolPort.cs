using System.Collections.Generic;
using System.IO;
using System.IO.Ports;
using System.Runtime.InteropServices;
using System.Text;
using LoopNext.Core.Logging;
using LoopNext.Core.Models;
using LoopNext.Core.Protocol;
using LoopNext.Metrology;

namespace LoopNext.Hardware;

// Высокоуровневый опрос прибора по открытому COM-порту.
// Перенос get_ph_from_tool (function.cpp:318) и read_tool_signature (function.cpp:296).
public static class ToolPort
{
    private const int SignaturePayloadLength = 6;
    private const int MaxRetries = 1;
    private const int PreFrameReceiveDelayMs = 50;

    // Полный дамп кадра пишется один раз за сеанс связи — см. LogRawToolData.
    private static bool _loggedFrameDump;

    /// <summary>
    /// Сбрасывает однократную диагностику, чтобы новый сеанс связи снова
    /// выдал дамп кадра. Иначе при смене прибора без перезапуска приложения
    /// в журнале остался бы дамп предыдущего.
    /// </summary>
    public static void ResetSessionDiagnostics()
    {
        _loggedFrameDump = false;
    }

    // Постоянная нагрузка запроса данных, как в эталоне: { 0x05, 0x00 }.
    private static readonly byte[] RequestData = { 0x05, 0x00 };

    public readonly record struct PhaseFrameResult(V10 Phases, V10 RawPhases, ToolResponseCode Code, float Temperature);

    public readonly record struct SignatureResult(ToolResponseCode Code, uint Signature);

    // Опрос кадра измерений с одной повторной попыткой при таймауте.
    // Метрология передаётся явно, чтобы применялась ориентация фаз по Rx_Position.
    public static PhaseFrameResult GetPhaseFromTool(SerialPort com, byte cmd, in GpToolMetrology metrology)
    {
        var phases = default(V10);
        phases.Clear();
        var rawPhases = default(V10);
        rawPhases.Clear();

        if (!com.IsOpen)
        {
            return new PhaseFrameResult(phases, rawPhases, ToolResponseCode.PortClosed, 0f);
        }

        var metrologyIdentity = ToolIdentity.Decode(metrology.Signature);
        var acceptZeroFrameCrc = metrologyIdentity.IsCartographLwd4TxMode;

        // Запрос кадра у обоих поколений прошивки одинаков, отличается только
        // длина ответа: 240 байт кадра у старой прошивки и 320 у новой.
        // Транспорт читает пакет фиксированной длины, поэтому размер кадра
        // нужно знать заранее — его называет сигнатура метрологии.
        //
        // Вторая попытка идёт с длиной другого поколения: если метрология
        // прибора не размечена, её сигнатура размер не назовёт, и ошибка длины
        // выглядит как таймаут (ждём больше, чем прислали) либо как несовпадение
        // CRC (прислали больше, чем ждём). Обе ситуации лечатся повтором с
        // другой длиной, а не разбором мусора.
        var frameSizes = BuildFrameSizeAttempts(metrologyIdentity.FrameSize);
        var dataIn = new byte[ToolProtocolConstants.GetToolPayloadLength(ToolIdentity.ExtendedFrameSize)];
        var lastCode = ToolResponseCode.Timeout;

        for (var retry = 0; retry <= MaxRetries; retry++)
        {
            var frameSize = frameSizes[Math.Min(retry, frameSizes.Length - 1)];
            var framePayloadLength = ToolProtocolConstants.GetToolPayloadLength(frameSize);

            try
            {
                com.DiscardInBuffer();
            }
            catch (Exception ex) when (ex is InvalidOperationException or IOException)
            {
                AppLogger.Write(ex, "Порт прибора закрыт во время подготовки к опросу кадра.");
                return new PhaseFrameResult(phases, rawPhases, ToolResponseCode.PortClosed, 0f);
            }

            if (ToolSerialTransport.Send(com, cmd, RequestData, RequestData.Length) != 0)
            {
                return new PhaseFrameResult(phases, rawPhases, ToolResponseCode.Timeout, 0f);
            }

            Thread.Sleep(PreFrameReceiveDelayMs);

            ToolResponseCode result;
            try
            {
                result = (ToolResponseCode)ToolSerialTransport.Receive(
                    com,
                    out _,
                    dataIn,
                    framePayloadLength,
                    acceptZeroCrc: acceptZeroFrameCrc);
            }
            catch (Exception ex) when (ex is InvalidOperationException or IOException)
            {
                AppLogger.Write(ex, "Порт прибора закрыт во время чтения кадра.");
                return new PhaseFrameResult(phases, rawPhases, ToolResponseCode.PortClosed, 0f);
            }

            lastCode = result;

            if (result == ToolResponseCode.Ok)
            {
                // GP_TOOL_DATA расположен со смещения 2 в принятой нагрузке.
                // У кадра новой прошивки за первыми 240 байтами идут амплитудные
                // величины; общая часть не сдвинулась, поэтому разбор одинаков,
                // а хвост этому приложению не нужен — оно считает по фазе.
                var toolData = MemoryMarshal.Read<GpToolData>(
                    dataIn.AsSpan(ToolProtocolConstants.ToolServicePrefixLength));
                if (acceptZeroFrameCrc && toolData.Signature != metrology.Signature)
                {
                    AppLogger.Write(
                        $"CARTOGRAPH_LWD_SIGNATURE_MISMATCH: metrology=0x{metrology.Signature:X8}, frame=0x{toolData.Signature:X8}");
                    return new PhaseFrameResult(phases, rawPhases, ToolResponseCode.CrcError, 0f);
                }

                LogRawToolData(in toolData, dataIn.AsSpan(0, framePayloadLength), frameSize);
                var phaseResult = ToolPhaseProcessor.FillPhaseFromToolData(in toolData, in metrology);
                phases = phaseResult.Phases;
                rawPhases = phaseResult.RawPhases;
                return new PhaseFrameResult(phases, rawPhases, ToolResponseCode.Ok, toolData.Temperature);
            }

            if (retry < MaxRetries)
            {
                var nextFrameSize = frameSizes[Math.Min(retry + 1, frameSizes.Length - 1)];
                if (result == ToolResponseCode.Timeout || nextFrameSize != frameSize)
                {
                    AppLogger.Write(
                        $"TOOL_FRAME_RETRY: result={result}, frame_size={frameSize}, next_frame_size={nextFrameSize}, " +
                        $"metrology_signature=0x{metrology.Signature:X8}");
                    continue;
                }
            }

            break;
        }

        return new PhaseFrameResult(phases, rawPhases, lastCode, 0f);
    }

    // Порядок попыток по размеру кадра: сначала размер, названный сигнатурой
    // прибора, затем — размер другого поколения как запасной.
    private static int[] BuildFrameSizeAttempts(int frameSizeFromSignature)
    {
        var alternate = frameSizeFromSignature == ToolIdentity.ExtendedFrameSize
            ? ToolIdentity.LegacyFrameSize
            : ToolIdentity.ExtendedFrameSize;
        return [frameSizeFromSignature, alternate];
    }

    // Полный разбор принятого кадра: все поля структуры измерений и, для новой
    // прошивки, её хвост с амплитудными величинами.
    //
    // Хвост читается прямо из принятой нагрузки: расчёты этого приложения идут
    // только по разности фаз, поэтому амплитудные поля не заведены в структуру
    // данных, а в журнал попадают ради проверки обмена.
    private static void LogRawToolData(in GpToolData data, ReadOnlySpan<byte> payload, int frameSize)
    {
        var identity = ToolIdentity.Decode(data.Signature);
        var txCount = (int)identity.TransmitterCount;

        AppLogger.Write(
            $"TOOL_FRAME: signature=0x{data.Signature:X8} ({data.Signature}) | {DescribeDevice(identity)} | " +
            $"прошивка={(identity.IsExtendedFirmware ? "новая" : "старая")} кадр={identity.FrameSize}Б принято={frameSize}Б | " +
            $"frame={data.Frame} | temperature={data.Temperature:F3} °C");
        AppLogger.Write(
            $"TOOL_FRAME_CONDITION: raw=0x{data.Condition:X8} | {DescribeCondition(data.Condition, txCount)}");

        AppLogger.Write($"TOOL_FRAME_RHO_SMT (Ohm*m): {FormatChannels(data.GetRhoSmt, txCount, 1.0f, "F2")}");
        AppLogger.Write(
            $"TOOL_FRAME_PHASE_SMT (mG): " +
            $"{FormatChannels(data.GetPhaseSmt, txCount, (float)PhysicalConstants.Milligrad, "F1")}");
        AppLogger.Write($"TOOL_FRAME_AM_RX_1: {FormatChannels(data.GetAmRx1, txCount, 1.0f, "F4")}");
        AppLogger.Write($"TOOL_FRAME_AM_RX_2: {FormatChannels(data.GetAmRx2, txCount, 1.0f, "F4")}");
        AppLogger.Write(
            $"TOOL_FRAME_DELTA_PH (mG): " +
            $"{FormatChannels(data.GetDeltaPhase, txCount, (float)PhysicalConstants.Milligrad, "F1")}");
        AppLogger.Write(
            $"TOOL_FRAME_SILENT: ZERO_AM_RX_1=[{data.GetZeroAmRx1(0):F4}, {data.GetZeroAmRx1(1):F4}] " +
            $"ZERO_AM_RX_2=[{data.GetZeroAmRx2(0):F4}, {data.GetZeroAmRx2(1):F4}] " +
            $"ZERO_dPH_mG=[{data.GetZeroDeltaPhase(0) * PhysicalConstants.Milligrad:F1}, " +
            $"{data.GetZeroDeltaPhase(1) * PhysicalConstants.Milligrad:F1}]");

        LogExtendedTail(payload, frameSize, txCount);

        // Полный дамп принятых байт пишется один раз за сессию: по нему
        // сверяются смещения полей, а повторять его на каждом кадре означало бы
        // утопить журнал.
        if (!_loggedFrameDump)
        {
            _loggedFrameDump = true;
            AppLogger.Write(ToolSerialTransport.FormatHexDump("TOOL_FRAME_DUMP (первый кадр сессии)", payload));
        }
    }

    // Амплитудные величины новой прошивки: rho_att_smt со смещения 240 и
    // att_dB_smt со смещения 280 внутри кадра.
    private static void LogExtendedTail(ReadOnlySpan<byte> payload, int frameSize, int txCount)
    {
        if (frameSize != ToolIdentity.ExtendedFrameSize)
        {
            return;
        }

        var frame = payload[ToolProtocolConstants.ToolServicePrefixLength..];

        AppLogger.Write(
            $"TOOL_FRAME_RHO_ATT_SMT (Ohm*m): " +
            $"{FormatRawChannels(frame, ExtendedFrameFields.TryReadRhoAttSmt, txCount, "F2")}");
        AppLogger.Write(
            $"TOOL_FRAME_ATT_DB_SMT (dB): " +
            $"{FormatRawChannels(frame, ExtendedFrameFields.TryReadAttDbSmt, txCount, "F5")}");
    }

    private static string DescribeDevice(ToolIdentity identity)
    {
        var typeName = identity.DeviceType switch
        {
            ToolIdentity.DeviceTypeAutonom => "Автономный",
            ToolIdentity.DeviceTypeLwd => "LWD",
            ToolIdentity.DeviceTypeCartograph => "Картограф",
            _ => "Неизвестно"
        };
        return $"{typeName} {identity.TransmitterCount}TX мод.{identity.Modification} № {identity.Number} " +
               $"(валидна={identity.IsValid})";
    }

    // Раскладка байта состояния: биты 8..12 — 400 кГц T1..T5, биты 0..4 — 2000 кГц.
    private static string DescribeCondition(uint condition, int txCount)
    {
        var builder = new StringBuilder("рабочие каналы:");
        for (var freq = 0; freq < GpToolData.FrequencyCount; freq++)
        {
            var conditionByte = freq == 0 ? (condition >> 8) & 0xFFu : condition & 0xFFu;
            builder.Append(freq == 0 ? " 400 кГц " : " | 2000 кГц ");
            for (var tx = 0; tx < txCount; tx++)
            {
                builder.Append($"T{tx + 1}={((conditionByte & (1u << tx)) != 0 ? 1 : 0)} ");
            }
        }

        return builder.ToString().TrimEnd();
    }

    private static string FormatChannels(
        Func<int, int, float> valueProvider,
        int txCount,
        float scale,
        string format)
    {
        var builder = new StringBuilder();
        for (var freq = 0; freq < GpToolData.FrequencyCount; freq++)
        {
            builder.Append(freq == 0 ? "400 кГц [" : " | 2000 кГц [");
            for (var tx = 0; tx < txCount; tx++)
            {
                if (tx > 0)
                {
                    builder.Append(", ");
                }

                builder.Append($"T{tx + 1}=").Append((valueProvider(freq, tx) * scale).ToString(format));
            }

            builder.Append(']');
        }

        return builder.ToString();
    }

    private delegate bool ExtendedFieldReader(ReadOnlySpan<byte> frame, int frequencyIndex, int transmitterIndex, out float value);

    private static string FormatRawChannels(
        ReadOnlySpan<byte> frame,
        ExtendedFieldReader reader,
        int txCount,
        string format)
    {
        var builder = new StringBuilder();
        for (var freq = 0; freq < GpToolData.FrequencyCount; freq++)
        {
            builder.Append(freq == 0 ? "400 кГц [" : " | 2000 кГц [");
            for (var tx = 0; tx < txCount; tx++)
            {
                if (tx > 0)
                {
                    builder.Append(", ");
                }

                builder.Append($"T{tx + 1}=")
                    .Append(reader(frame, freq, tx, out var value) ? value.ToString(format) : "—");
            }

            builder.Append(']');
        }

        return builder.ToString();
    }

    // Чтение сигнатуры прибора. Возвращает код результата и саму сигнатуру.
    public static SignatureResult ReadSignature(SerialPort com)
    {
        if (ToolSerialTransport.Send(com, ToolProtocolConstants.CmdGetSignature, RequestData, RequestData.Length) != 0)
        {
            return new SignatureResult(ToolResponseCode.Timeout, 0);
        }

        Thread.Sleep(50);

        var dataIn = new byte[SignaturePayloadLength];
        var result = (ToolResponseCode)ToolSerialTransport.Receive(com, out var recvCmd, dataIn, SignaturePayloadLength);
        if (result != ToolResponseCode.Ok)
        {
            return new SignatureResult(result, 0);
        }

        if (recvCmd != ToolProtocolConstants.CmdGetSignature)
        {
            return new SignatureResult(ToolResponseCode.PortClosed, 0);
        }

        // Сигнатура читается со смещения 2 в принятой нагрузке (memcpy DATA_IN + 2).
        var signature = BitConverter.ToUInt32(dataIn, 2);
        return new SignatureResult(ToolResponseCode.Ok, signature);
    }
}
