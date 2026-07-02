using System.IO.Ports;
using System.Runtime.InteropServices;
using LoopNext.Core.Models;
using LoopNext.Core.Protocol;

namespace LoopNext.Hardware;

// Чтение метрологии прибора по открытому COM-порту.
// Перенос buttonRead_from_sonde_Click (MetroForm.h:1721).
public static class MetrologyPort
{
    private const int MetrologyPayloadLength = ToolProtocolConstants.ToolPayloadLength; // 242
    private const int ReadAttempts = 3;
    private const int RetryDelayMs = 100;

    private const byte ReadSubCommand = 0x05;

    public readonly record struct MetrologyReadResult(ToolResponseCode Code, GpToolMetrology Metrology);

    // Чтение метрологии с тремя попытками. На неудачной попытке буфер сбрасывается с паузой.
    public static MetrologyReadResult ReadFromTool(SerialPort port)
    {
        Span<byte> payload = stackalloc byte[] { ReadSubCommand, 0x00 };
        var dataIn = new byte[MetrologyPayloadLength];
        var result = ToolResponseCode.Timeout;
        var metrology = default(GpToolMetrology);

        for (var attempt = 0; attempt < ReadAttempts && result != ToolResponseCode.Ok; attempt++)
        {
            if (ToolSerialTransport.Send(port, ToolProtocolConstants.CmdGetMetrology, payload, 2) != 0)
            {
                continue;
            }

            result = (ToolResponseCode)ToolSerialTransport.Receive(port, out _, dataIn, MetrologyPayloadLength);
            if (result == ToolResponseCode.Ok)
            {
                // GP_TOOL_METROLOGY расположен со смещения 2 в принятой нагрузке.
                metrology = MemoryMarshal.Read<GpToolMetrology>(dataIn.AsSpan(2));
            }
            else
            {
                port.DiscardInBuffer();
                Thread.Sleep(RetryDelayMs);
            }
        }

        return new MetrologyReadResult(result, metrology);
    }
}
