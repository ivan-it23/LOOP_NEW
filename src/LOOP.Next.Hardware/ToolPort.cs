using System.Collections.Generic;
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
    private const int FramePayloadLength = ToolProtocolConstants.ToolPayloadLength; // 242
    private const int SignaturePayloadLength = 6;
    private const int MaxRetries = 1;

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

        var dataIn = new byte[FramePayloadLength];
        var lastCode = ToolResponseCode.Timeout;

        for (var retry = 0; retry <= MaxRetries; retry++)
        {
            com.DiscardInBuffer();

            if (ToolSerialTransport.Send(com, cmd, RequestData, RequestData.Length) != 0)
            {
                return new PhaseFrameResult(phases, rawPhases, ToolResponseCode.Timeout, 0f);
            }

            var result = (ToolResponseCode)ToolSerialTransport.Receive(com, out _, dataIn, FramePayloadLength);
            lastCode = result;

            if (result == ToolResponseCode.Ok)
            {
                // GP_TOOL_DATA расположен со смещения 2 в принятой нагрузке.
                var toolData = MemoryMarshal.Read<GpToolData>(dataIn.AsSpan(2));
                LogRawToolData(in toolData);
                var phaseResult = ToolPhaseProcessor.FillPhaseFromToolData(in toolData, in metrology);
                phases = phaseResult.Phases;
                rawPhases = phaseResult.RawPhases;
                return new PhaseFrameResult(phases, rawPhases, ToolResponseCode.Ok, toolData.Temperature);
            }

            if (result == ToolResponseCode.Timeout && retry < MaxRetries)
            {
                continue;
            }

            break;
        }

        return new PhaseFrameResult(phases, rawPhases, lastCode, 0f);
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
            $"RAW_TOOL_DATA: signature=0x{data.Signature:X8}, Tx={txCount}, condition={data.Condition}, frame={data.Frame}, temp={data.Temperature:F2}, " +
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
