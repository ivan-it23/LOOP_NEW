using System.IO.Ports;
using System.Runtime.InteropServices;
using LoopNext.Core.Models;
using LoopNext.Core.Protocol;
using LoopNext.Metrology;

namespace LoopNext.Hardware;

// Подключение и отключение прибора по единому COM-алгоритму.
// Перенос активной ветки button_ON_OFF_Click (WorkForm.h:5876-6028).
public static class ToolConnection
{
    private const int MetrologyPayloadLength = ToolProtocolConstants.ToolPayloadLength; // 242
    private const int ReadAttempts = 3;
    private const int PostCloseDelayMs = 100;
    private const int PostOpenDelayMs = 150;
    private const int PreReceiveDelayMs = 100;

    private const byte ReadSubCommand = 0x05;

    public readonly record struct ToolConnectResult(
        bool Success,
        GpToolMetrology Metrology,
        IReadOnlyList<SondeParameter> SondeParameters);

    // Включение прибора: открытие порта на 125000 бод и чтение метрологии командой 0x13.
    // Успех определяется отсутствием исключения при открытии/обмене, как в эталоне
    // (on_off=true даже если метрология не считана; в этом случае параметры зонда пусты).
    public static ToolConnectResult Connect(SerialPort port)
    {
        try
        {
            if (port.IsOpen)
            {
                port.Close();
            }

            Thread.Sleep(PostCloseDelayMs);

            port.BaudRate = ToolProtocolConstants.ToolBaudRate;
            port.ReadTimeout = ToolProtocolConstants.ReadTimeoutMs;
            port.WriteTimeout = ToolProtocolConstants.WriteTimeoutMs;
            port.Open();
            port.DiscardOutBuffer();
            Thread.Sleep(PostOpenDelayMs);

            var metrology = ReadMetrologyOnConnect(port);

            port.DiscardOutBuffer();
            Thread.Sleep(PostCloseDelayMs);

            var sondeParameters = MetrologyConverter.Convert(in metrology);
            return new ToolConnectResult(true, metrology, sondeParameters);
        }
        catch
        {
            // Эталон при исключении оставлял порт открытым; здесь порт закрывается,
            // чтобы не удерживать ресурс при неуспешном подключении.
            if (port.IsOpen)
            {
                port.Close();
            }

            return new ToolConnectResult(false, default, Array.Empty<SondeParameter>());
        }
    }

    // Выключение прибора: закрытие порта.
    public static void Disconnect(SerialPort port)
    {
        if (port.IsOpen)
        {
            port.Close();
        }

        Thread.Sleep(PostCloseDelayMs);
    }

    // Чтение метрологии при подключении: 3 попытки команды 0x13 с паузой перед приёмом.
    private static GpToolMetrology ReadMetrologyOnConnect(SerialPort port)
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

            Thread.Sleep(PreReceiveDelayMs);
            result = (ToolResponseCode)ToolSerialTransport.Receive(port, out _, dataIn, MetrologyPayloadLength);
            if (result == ToolResponseCode.Ok)
            {
                metrology = MemoryMarshal.Read<GpToolMetrology>(dataIn.AsSpan(2));
            }
        }

        return metrology;
    }
}
