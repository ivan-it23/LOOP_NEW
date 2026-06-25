using System.IO.Ports;

namespace LoopNext.Hardware;

// Подключение и отключение кольца (loop) по текстовому протоколу.
// Перенос BT_ON (WorkForm.h:6431): открытие порта на 9600 бод, команда "$\r\n",
// успех при ответе, начинающемся с "Unknown".
public static class RingConnection
{
    // Результат подключения кольца с диагностической причиной для UI.
    public enum RingConnectStatus
    {
        Connected,
        PortOpenFailed,
        NoResponse,
        Rejected
    }

    // Включение кольца. Возвращает статус подключения; запуск опроса и сбор воздуха —
    // ответственность вышележащего слоя (button_AIR/таймеры в эталоне).
    public static RingConnectStatus Connect(SerialPort port)
    {
        if (port.IsOpen)
        {
            port.Close();
        }

        port.BaudRate = RingProtocolConstants.BaudRate;
        port.ReadTimeout = RingProtocolConstants.TimeoutMs;
        port.WriteTimeout = RingProtocolConstants.TimeoutMs;

        try
        {
            port.Open();
        }
        catch
        {
            return RingConnectStatus.PortOpenFailed;
        }

        port.DiscardInBuffer();
        port.Write(RingProtocolConstants.IdentifyCommand);

        string response;
        try
        {
            response = port.ReadLine();
        }
        catch
        {
            port.Close();
            return RingConnectStatus.NoResponse;
        }

        if (response.StartsWith(RingProtocolConstants.ExpectedReplyPrefix, StringComparison.Ordinal))
        {
            return RingConnectStatus.Connected;
        }

        port.Close();
        return RingConnectStatus.Rejected;
    }

    // Отключение кольца: закрытие порта.
    public static void Disconnect(SerialPort port)
    {
        if (port.IsOpen)
        {
            port.Close();
        }
    }
}
