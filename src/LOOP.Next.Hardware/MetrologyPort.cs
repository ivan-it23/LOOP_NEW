using System.IO.Ports;
using System.Runtime.InteropServices;
using LoopNext.Core.Models;
using LoopNext.Core.Protocol;

namespace LoopNext.Hardware;

// Чтение и запись метрологии прибора по открытому COM-порту.
// Перенос buttonRead_from_sonde_Click и buttonWrite_to_sonde_Click (MetroForm.h:1721, 1835).
// В эталоне и чтение, и запись выполняются командой 0x13; направление задаётся первым байтом
// нагрузки: 0x05 — чтение, 0xF5 — запись. Константа CmdSetMetrology (0x12) рабочим приложением
// не используется и оставлена в протоколе как справочная.
public static class MetrologyPort
{
    private const int MetrologyPayloadLength = ToolProtocolConstants.ToolPayloadLength; // 242
    private const int ReadAttempts = 3;
    private const int RetryDelayMs = 100;
    private const int WriteAckDelayMs = 150;
    private const int WriteAckLength = 2;

    // Подкоманды направления в первом байте нагрузки.
    private const byte ReadSubCommand = 0x05;
    private const byte WriteSubCommand = 0xF5;

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

    // Запись метрологии в прибор. Нагрузка: [0xF5][0x00][GP_TOOL_METROLOGY:240].
    public static bool WriteToTool(SerialPort port, in GpToolMetrology metrology)
    {
        var payload = new byte[MetrologyPayloadLength];
        payload[0] = WriteSubCommand;
        payload[1] = 0x00;

        var value = metrology;
        MemoryMarshal.Write(payload.AsSpan(2), in value);

        ToolSerialTransport.Send(port, ToolProtocolConstants.CmdGetMetrology, payload, MetrologyPayloadLength);

        Thread.Sleep(WriteAckDelayMs);

        var ack = new byte[WriteAckLength];
        return (ToolResponseCode)ToolSerialTransport.Receive(port, out _, ack, WriteAckLength) == ToolResponseCode.Ok;
    }
}
