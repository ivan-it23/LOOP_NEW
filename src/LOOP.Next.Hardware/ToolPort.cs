using System.IO.Ports;
using System.Runtime.InteropServices;
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

    public readonly record struct PhaseFrameResult(V10 Phases, ToolResponseCode Code);

    public readonly record struct SignatureResult(ToolResponseCode Code, uint Signature);

    // Опрос кадра измерений с одной повторной попыткой при таймауте.
    // rxPosition берётся из метрологии прибора (ранее глобальный metrology_struct.Rx_Position).
    public static PhaseFrameResult GetPhaseFromTool(SerialPort com, byte cmd, uint rxPosition)
    {
        var phases = default(V10);
        phases.Clear();

        if (!com.IsOpen)
        {
            return new PhaseFrameResult(phases, ToolResponseCode.PortClosed);
        }

        var dataIn = new byte[FramePayloadLength];
        var lastCode = ToolResponseCode.Timeout;

        for (var retry = 0; retry <= MaxRetries; retry++)
        {
            com.DiscardInBuffer();

            if (ToolSerialTransport.Send(com, cmd, RequestData, RequestData.Length) != 0)
            {
                return new PhaseFrameResult(phases, ToolResponseCode.Timeout);
            }

            var result = (ToolResponseCode)ToolSerialTransport.Receive(com, out _, dataIn, FramePayloadLength);
            lastCode = result;

            if (result == ToolResponseCode.Ok)
            {
                // GP_TOOL_DATA расположен со смещения 2 в принятой нагрузке.
                var toolData = MemoryMarshal.Read<GpToolData>(dataIn.AsSpan(2));
                phases = ToolPhaseProcessor.FillPhaseFromToolData(in toolData, rxPosition).Phases;
                return new PhaseFrameResult(phases, ToolResponseCode.Ok);
            }

            if (result == ToolResponseCode.Timeout && retry < MaxRetries)
            {
                continue;
            }

            break;
        }

        return new PhaseFrameResult(phases, lastCode);
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
