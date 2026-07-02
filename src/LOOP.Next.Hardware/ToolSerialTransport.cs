using System.IO.Ports;
using LoopNext.Core.Logging;

namespace LoopNext.Hardware;

// Коды результата обмена, соответствующие возвратам receive_from_tool (function.cpp:909).
public enum ToolResponseCode
{
    Ok = 0,
    Timeout = 1,
    CrcError = 2,
    PortClosed = 3
}

// Низкоуровневый протокол обмена с прибором по UART.
// Формат старого пакета: [CMD:1][PAYLOAD:N][CRC_lo:1][CRC_hi:1]; CRC считается по [CMD + PAYLOAD].
public static class ToolSerialTransport
{
    // Отправка команды прибору. Возвращает 0 при успехе, 1 при ошибке записи (send_to_tool).
    public static int Send(SerialPort com, byte cmd, ReadOnlySpan<byte> data, int length)
    {
        var packet = new byte[length + 3];
        packet[0] = cmd;
        for (var i = 1; i < length + 1; i++)
        {
            packet[i] = data[i - 1];
        }

        var crc = Crc16.Compute(packet, length + 1);
        packet[length + 1] = (byte)crc;
        packet[length + 2] = (byte)(crc >> 8);

        try
        {
            com.Write(packet, 0, length + 3);
        }
        catch
        {
            return 1;
        }

        return 0;
    }

    // Приём ответа фиксированной длины с накоплением до полного пакета или таймаута.
    // Возвращает 0 (Ok), 1 (Timeout/короткий ответ) или 2 (CRC не совпал). Перенос receive_from_tool.
    public static int Receive(SerialPort com, out byte cmd, byte[] data, int length, bool acceptZeroCrc = false)
    {
        cmd = 0;
        var total = length + 3;
        var packet = new byte[total];
        var stop = false;
        var bytesRead = 0;
        var deadline = DateTime.Now.AddMilliseconds(com.ReadTimeout);

        try
        {
            while (bytesRead < total)
            {
                var n = com.Read(packet, bytesRead, total - bytesRead);
                if (n <= 0)
                {
                    break;
                }

                bytesRead += n;
                if (DateTime.Now > deadline)
                {
                    break;
                }
            }
        }
        catch
        {
            // Исключение трактуется как таймаут только если не получено ни одного байта.
            if (bytesRead == 0)
            {
                stop = true;
            }
        }

        if (!stop && bytesRead < total)
        {
            com.DiscardInBuffer();
            return (int)ToolResponseCode.Timeout;
        }

        if (!stop)
        {
            var calcCrc = Crc16.Compute(packet, length + 1);
            var recvCrc = (ushort)(packet[length + 1] | (packet[length + 2] << 8));
            if (calcCrc == recvCrc || (acceptZeroCrc && recvCrc == 0))
            {
                cmd = packet[0];
                for (var i = 0; i < length; i++)
                {
                    data[i] = packet[i + 1];
                }

                if (calcCrc != recvCrc)
                {
                    AppLogger.Write(
                        $"TOOL_ZERO_CRC_ACCEPTED: expected_payload={length}, bytes_read={bytesRead}, cmd=0x{packet[0]:X2}, " +
                        $"calc=0x{calcCrc:X4}, recv=0x{recvCrc:X4}, head={FormatPacketSlice(packet, 0, Math.Min(bytesRead, 16))}");
                }

                return (int)ToolResponseCode.Ok;
            }

            AppLogger.Write(
                $"TOOL_CRC_ERROR: expected_payload={length}, bytes_read={bytesRead}, cmd=0x{packet[0]:X2}, " +
                $"calc=0x{calcCrc:X4}, recv=0x{recvCrc:X4}, head={FormatPacketSlice(packet, 0, Math.Min(bytesRead, 16))}, " +
                $"tail={FormatPacketSlice(packet, Math.Max(0, bytesRead - 8), Math.Min(bytesRead, 8))}");
            return (int)ToolResponseCode.CrcError;
        }

        return (int)ToolResponseCode.Timeout;
    }

    private static string FormatPacketSlice(byte[] packet, int start, int count)
    {
        if (count <= 0)
        {
            return string.Empty;
        }

        return Convert.ToHexString(packet.AsSpan(start, count));
    }
}
