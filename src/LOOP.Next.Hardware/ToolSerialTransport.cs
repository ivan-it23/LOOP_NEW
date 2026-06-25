using System.IO.Ports;

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
// Точный перенос send_to_tool и receive_from_tool из function.cpp.
// Формат пакета: [CMD:1][PAYLOAD:N][CRC_lo:1][CRC_hi:1]; CRC считается по [CMD + PAYLOAD].
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
    public static int Receive(SerialPort com, out byte cmd, byte[] data, int length)
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
            if (calcCrc == recvCrc)
            {
                cmd = packet[0];
                for (var i = 0; i < length; i++)
                {
                    data[i] = packet[i + 1];
                }

                return (int)ToolResponseCode.Ok;
            }

            return (int)ToolResponseCode.CrcError;
        }

        return (int)ToolResponseCode.Timeout;
    }
}
