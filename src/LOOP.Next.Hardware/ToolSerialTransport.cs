using System.IO.Ports;
using System.Buffers.Binary;
using System.Text;
using LoopNext.Core.Logging;

namespace LoopNext.Hardware;

// Коды результата обмена, соответствующие возвратам receive_from_tool (function.cpp:909).
public enum ToolResponseCode
{
    Ok = 0,
    Timeout = 1,
    CrcError = 2,
    PortClosed = 3,
    InvalidResponse = 4
}

// Низкоуровневый протокол обмена с прибором по UART.
// Формат старого пакета: [CMD:1][PAYLOAD:N][CRC_lo:1][CRC_hi:1]; CRC считается по [CMD + PAYLOAD].
public static class ToolSerialTransport
{
    public const int FramedHeaderLength = 3;
    public const int FramedCrcLength = 2;
    public const int FramedMinimumPacketLength = FramedHeaderLength + FramedCrcLength;
    public const int FramedMaximumPacketLength = 4096;

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

    /// <summary>
    /// Формирует пакет новой прошивки:
    /// [CMD][полная длина LE:2][payload][CRC16 LE:2].
    /// </summary>
    public static byte[] BuildFramedPacket(byte cmd, ReadOnlySpan<byte> payload)
    {
        var totalLength = checked(payload.Length + FramedMinimumPacketLength);
        if (totalLength > ushort.MaxValue)
        {
            throw new ArgumentOutOfRangeException(nameof(payload), "Пакет прибора превышает поле длины uint16.");
        }

        var packet = new byte[totalLength];
        packet[0] = cmd;
        BinaryPrimitives.WriteUInt16LittleEndian(packet.AsSpan(1, sizeof(ushort)), (ushort)totalLength);
        payload.CopyTo(packet.AsSpan(FramedHeaderLength));

        var crc = Crc16.Compute(packet, totalLength - FramedCrcLength);
        BinaryPrimitives.WriteUInt16LittleEndian(packet.AsSpan(totalLength - FramedCrcLength), crc);
        return packet;
    }

    /// <summary>
    /// Проверяет и разбирает полный пакет новой прошивки.
    /// </summary>
    public static ToolResponseCode ParseFramedPacket(
        ReadOnlySpan<byte> packet,
        out byte cmd,
        out byte[] payload)
    {
        cmd = 0;
        payload = Array.Empty<byte>();
        if (packet.Length < FramedMinimumPacketLength)
        {
            return ToolResponseCode.InvalidResponse;
        }

        var declaredLength = BinaryPrimitives.ReadUInt16LittleEndian(packet.Slice(1, sizeof(ushort)));
        if (declaredLength != packet.Length || declaredLength > FramedMaximumPacketLength)
        {
            return ToolResponseCode.InvalidResponse;
        }

        var calculatedCrc = Crc16.Compute(packet, packet.Length - FramedCrcLength);
        var receivedCrc = BinaryPrimitives.ReadUInt16LittleEndian(packet[^FramedCrcLength..]);
        if (calculatedCrc != receivedCrc)
        {
            return ToolResponseCode.CrcError;
        }

        cmd = packet[0];
        payload = packet.Slice(
            FramedHeaderLength,
            packet.Length - FramedMinimumPacketLength).ToArray();
        return ToolResponseCode.Ok;
    }

    /// <summary>
    /// Отправляет пакет с полем длины, используемый только новой прошивкой.
    /// </summary>
    public static int SendFramed(SerialPort com, byte cmd, ReadOnlySpan<byte> payload)
    {
        var packet = BuildFramedPacket(cmd, payload);
        try
        {
            com.Write(packet, 0, packet.Length);
        }
        catch (Exception ex)
        {
            AppLogger.Write(
                $"TOOL_FRAMED_TX: cmd=0x{cmd:X2} size={packet.Length} ОШИБКА ЗАПИСИ {ex.GetType().Name}: {ex.Message}");
            return 1;
        }

        AppLogger.Write(
            $"TOOL_FRAMED_TX: cmd=0x{cmd:X2} size={packet.Length} bytes={Convert.ToHexString(packet)}");
        return 0;
    }

    /// <summary>
    /// Читает ответ новой прошивки по длине из заголовка, затем проверяет CRC.
    /// </summary>
    public static ToolResponseCode ReceiveFramed(SerialPort com, out byte cmd, out byte[] payload)
    {
        cmd = 0;
        payload = Array.Empty<byte>();
        var startedAt = DateTime.Now;
        var header = new byte[FramedHeaderLength];

        if (!ReadExactly(com, header, out var headerBytesRead))
        {
            AppLogger.Write(
                $"TOOL_FRAMED_RX: expected_header={FramedHeaderLength} read={headerBytesRead} " +
                $"elapsed_ms={Elapsed(startedAt)} result=Timeout");
            TryDiscardInput(com);
            return ToolResponseCode.Timeout;
        }

        var declaredLength = BinaryPrimitives.ReadUInt16LittleEndian(header.AsSpan(1, sizeof(ushort)));
        if (declaredLength < FramedMinimumPacketLength || declaredLength > FramedMaximumPacketLength)
        {
            AppLogger.Write(
                $"TOOL_FRAMED_RX: header={Convert.ToHexString(header)} declared={declaredLength} " +
                $"elapsed_ms={Elapsed(startedAt)} result=InvalidResponse");
            TryDiscardInput(com);
            return ToolResponseCode.InvalidResponse;
        }

        var packet = new byte[declaredLength];
        header.CopyTo(packet, 0);
        var tail = new byte[declaredLength - FramedHeaderLength];
        if (!ReadExactly(com, tail, out var tailBytesRead))
        {
            AppLogger.Write(
                $"TOOL_FRAMED_RX: declared={declaredLength} read={FramedHeaderLength + tailBytesRead} " +
                $"elapsed_ms={Elapsed(startedAt)} result=Timeout");
            TryDiscardInput(com);
            return ToolResponseCode.Timeout;
        }

        tail.CopyTo(packet, FramedHeaderLength);
        var result = ParseFramedPacket(packet, out cmd, out payload);
        var calculatedCrc = Crc16.Compute(packet, packet.Length - FramedCrcLength);
        var receivedCrc = BinaryPrimitives.ReadUInt16LittleEndian(packet.AsSpan(packet.Length - FramedCrcLength));
        AppLogger.Write(
            $"TOOL_FRAMED_RX: cmd=0x{packet[0]:X2} declared={declaredLength} payload={payload.Length} " +
            $"read={packet.Length} elapsed_ms={Elapsed(startedAt)} crc_calc=0x{calculatedCrc:X4} " +
            $"crc_recv=0x{receivedCrc:X4} result={result}");

        if (result != ToolResponseCode.Ok)
        {
            AppLogger.Write(FormatHexDump("TOOL_FRAMED_RX_DUMP", packet));
        }

        return result;
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

    private static bool ReadExactly(SerialPort com, byte[] buffer, out int bytesRead)
    {
        bytesRead = 0;
        try
        {
            while (bytesRead < buffer.Length)
            {
                var count = com.Read(buffer, bytesRead, buffer.Length - bytesRead);
                if (count <= 0)
                {
                    return false;
                }

                bytesRead += count;
            }

            return true;
        }
        catch (TimeoutException)
        {
            return false;
        }
    }

    private static void TryDiscardInput(SerialPort com)
    {
        try
        {
            com.DiscardInBuffer();
        }
        catch (Exception ex) when (ex is InvalidOperationException or IOException)
        {
            AppLogger.Write(ex, "Не удалось очистить входной буфер прибора после ошибки framed-пакета.");
        }
    }
}
