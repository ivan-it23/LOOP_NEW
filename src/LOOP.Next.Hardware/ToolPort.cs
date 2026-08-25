using System.Collections.Generic;
using System.IO;
using System.IO.Ports;
using System.Runtime.InteropServices;
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

                LogRawToolData(in toolData);
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

    // Чтение сигнатуры прибора. Возвращает код результата и саму сигнатуру.
    private static void LogRawToolData(in GpToolData data)
    {
        var identity = ToolIdentity.Decode(data.Signature);
        var txCount = (int)identity.TransmitterCount;
        var freqCount = GpToolData.FrequencyCount;
        var rhoValues = new List<string>(freqCount * txCount);
        var phaseValues = new List<string>(freqCount * txCount);
        var deltaValues = new List<string>(freqCount * txCount);
        for (var freq = 0; freq < freqCount; freq++)
        {
            for (var tx = 0; tx < txCount; tx++)
            {
                var rho = data.GetRhoSmt(freq, tx);
                var phase = data.GetPhaseSmt(freq, tx) * PhysicalConstants.Milligrad;
                var delta = data.GetDeltaPhase(freq, tx) * PhysicalConstants.Milligrad;
                rhoValues.Add($"F{freq}T{tx}={rho:F2}");
                phaseValues.Add($"F{freq}T{tx}={phase:F1}");
                deltaValues.Add($"F{freq}T{tx}={delta:F1}");
            }
        }

        AppLogger.Write(
            $"RAW_TOOL_DATA: signature=0x{data.Signature:X8}, firmware={(identity.IsExtendedFirmware ? "new" : "legacy")}, " +
            $"frame_size={identity.FrameSize}, Tx={txCount}, condition={data.Condition}, frame={data.Frame}, temp={data.Temperature:F2}, " +
            $"rho=[{string.Join(", ", rhoValues)}], phase_mG=[{string.Join(", ", phaseValues)}], delta_mG=[{string.Join(", ", deltaValues)}]");
    }

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
