using System.IO.Ports;
using System.Text;
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
        catch (Exception ex)
        {
            AppLogger.Write($"TOOL_TX: cmd=0x{cmd:X2} size={packet.Length} ОШИБКА ЗАПИСИ {ex.GetType().Name}: {ex.Message}");
            return 1;
        }

        // Запрос короткий, поэтому пишется целиком: по нему видно и команду, и CRC.
        AppLogger.Write($"TOOL_TX: cmd=0x{cmd:X2} size={packet.Length} bytes={Convert.ToHexString(packet)}");
        return 0;
    }

    // Полный шестнадцатеричный дамп с адресами смещений. Нужен там, где по
    // журналу сверяются смещения полей структуры.
    public static string FormatHexDump(string label, ReadOnlySpan<byte> bytes)
    {
        if (bytes.IsEmpty)
        {
            return $"{label} (0 B)";
        }

        var builder = new StringBuilder(label.Length + 24 + bytes.Length * 3);
        builder.Append(label).Append(" (").Append(bytes.Length).Append(" B):");
        const int bytesPerLine = 16;
        for (var offset = 0; offset < bytes.Length; offset += bytesPerLine)
        {
            builder.Append(Environment.NewLine).Append("  ").Append(offset.ToString("D4")).Append(": ");
            var slice = bytes.Slice(offset, Math.Min(bytesPerLine, bytes.Length - offset));
            for (var i = 0; i < slice.Length; i++)
            {
                builder.Append(slice[i].ToString("X2")).Append(' ');
            }
        }

        return builder.ToString();
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
        var startedAt = DateTime.Now;
        var deadline = startedAt.AddMilliseconds(com.ReadTimeout);

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
            AppLogger.Write(
                $"TOOL_RX: expected={total} read={bytesRead} elapsed_ms={Elapsed(startedAt)} " +
                $"result=Timeout (ответ короче ожидаемого)");
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

                AppLogger.Write(
                    $"TOOL_RX: cmd=0x{packet[0]:X2} payload={length} read={bytesRead} elapsed_ms={Elapsed(startedAt)} " +
                    $"crc_calc=0x{calcCrc:X4} crc_recv=0x{recvCrc:X4} result=Ok" +
                    (calcCrc != recvCrc ? " (принят нулевой CRC)" : string.Empty));

                return (int)ToolResponseCode.Ok;
            }

            // Несовпадение CRC — единственный случай, когда полезен весь пакет:
            // по нему видно и мусор в линии, и ответ неожиданной длины.
            AppLogger.Write(
                $"TOOL_RX: cmd=0x{packet[0]:X2} payload={length} read={bytesRead} elapsed_ms={Elapsed(startedAt)} " +
                $"crc_calc=0x{calcCrc:X4} crc_recv=0x{recvCrc:X4} result=CrcError");
            AppLogger.Write(FormatHexDump("TOOL_RX_DUMP", packet.AsSpan(0, bytesRead)));
            return (int)ToolResponseCode.CrcError;
        }

        AppLogger.Write(
            $"TOOL_RX: expected={total} read={bytesRead} elapsed_ms={Elapsed(startedAt)} result=Timeout (ответа нет)");
        return (int)ToolResponseCode.Timeout;
    }

    private static long Elapsed(DateTime startedAt)
    {
        return (long)(DateTime.Now - startedAt).TotalMilliseconds;
    }
}
