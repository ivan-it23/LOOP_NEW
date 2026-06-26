using System.IO.Ports;
using System.Globalization;
using LoopNext.Core.Logging;
using LoopNext.Core.Models;

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
        AppLogger.Write("=== НАЧАЛО ВКЛЮЧЕНИЯ КОЛЬЦА ===");
        AppLogger.Write($"Порт кольца={port.PortName}, BaudRate={RingProtocolConstants.BaudRate}, Timeout={RingProtocolConstants.TimeoutMs}ms");

        if (port.IsOpen)
        {
            AppLogger.Write("Порт кольца уже открыт, закрываем перед переподключением.");
            port.Close();
        }

        port.BaudRate = RingProtocolConstants.BaudRate;
        port.ReadTimeout = RingProtocolConstants.TimeoutMs;
        port.WriteTimeout = RingProtocolConstants.TimeoutMs;

        try
        {
            AppLogger.Write("Открытие COM-порта кольца...");
            port.Open();
            AppLogger.Write("Порт кольца открыт успешно.");
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "ОШИБКА открытия порта кольца");
            return RingConnectStatus.PortOpenFailed;
        }

        port.DiscardInBuffer();
        AppLogger.Write("Отправка команды идентификации '$'...");
        port.Write(RingProtocolConstants.IdentifyCommand);

        string response;
        try
        {
            response = port.ReadLine();
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Таймаут ожидания ответа кольца");
            port.Close();
            return RingConnectStatus.NoResponse;
        }

        AppLogger.Write($"Ответ кольца (длина {response.Length}): '{response}'");

        if (response.StartsWith(RingProtocolConstants.ExpectedReplyPrefix, StringComparison.Ordinal))
        {
            AppLogger.Write("=== КОЛЬЦО УСПЕШНО ПОДКЛЮЧЕНО ===");
            return RingConnectStatus.Connected;
        }

        AppLogger.Write("Ответ не содержит 'Unknown'. Подключение отклонено.");
        port.Close();
        return RingConnectStatus.Rejected;
    }

    // Отключение кольца: закрытие порта.
    public static void Disconnect(SerialPort port)
    {
        if (port.IsOpen)
        {
            AppLogger.Write("=== ВЫКЛЮЧЕНИЕ КОЛЬЦА ===");
            port.Close();
            AppLogger.Write("Порт кольца закрыт.");
        }
    }

    public static RingLoopParameterReadResult ReadLoopParameters(SerialPort port)
    {
        AppLogger.Write("=== ЧТЕНИЕ ПАРАМЕТРОВ КОЛЬЦА ===");
        if (!port.IsOpen)
        {
            AppLogger.Write("ОШИБКА: порт кольца не открыт.");
            return new RingLoopParameterReadResult(false, Array.Empty<RingLoopParameterCell>(), Array.Empty<LoopParameter>());
        }

        var cells = new List<RingLoopParameterCell>(RingProtocolConstants.LoopParameterCellCount);
        for (var index = 0; index < RingProtocolConstants.LoopParameterCellCount; index++)
        {
            var command = string.Format(CultureInfo.InvariantCulture, RingProtocolConstants.ReadLoopParameterCommandFormat, index);
            var prefix = string.Format(CultureInfo.InvariantCulture, RingProtocolConstants.ReadLoopParameterReplyPrefixFormat, index);

            try
            {
                port.DiscardInBuffer();
                port.Write(command);
                AppLogger.Write($"ZREAD n={index}: отправлено '{command.Trim()}'");
                Thread.Sleep(50);

                var response = port.ReadLine();
                AppLogger.Write($"ZREAD n={index}: ответ '{response.Trim()}'");
                cells.Add(ParseLoopParameterCell(index, response, prefix));
            }
            catch (Exception ex)
            {
                AppLogger.Write(ex, $"ZREAD n={index}: ошибка чтения");
                cells.Add(new RingLoopParameterCell(index, 0f, 0f, false));
            }
        }

        var parameters = new List<LoopParameter>(cells.Count);
        foreach (var cell in cells)
        {
            if (!cell.HasValue || cell.Reactance <= 0.000001f)
            {
                continue;
            }

            parameters.Add(new LoopParameter
            {
                Resistance = cell.Resistance,
                Reactance = cell.Reactance,
                Radius = 0.5f,
                AdditionalWireResistance = 0.5f
            });
        }

        AppLogger.Write($"Параметры кольца прочитаны: {parameters.Count} ячеек.");
        return new RingLoopParameterReadResult(parameters.Count > 0, cells, parameters);
    }

    public static RingWorkResult SetMeasurementPoint(SerialPort port, int resistanceIndex, int capacitanceIndex)
    {
        if (!port.IsOpen)
        {
            AppLogger.Write("ОШИБКА: невозможно установить точку кольца — порт не открыт.");
            return RingWorkResult.NotConnected;
        }

        var resistanceCommand = string.Format(CultureInfo.InvariantCulture, RingProtocolConstants.SetResistanceCommandFormat, resistanceIndex);
        var capacitanceCommand = string.Format(CultureInfo.InvariantCulture, RingProtocolConstants.SetCapacitanceCommandFormat, capacitanceIndex);
        var resistanceReply = string.Format(CultureInfo.InvariantCulture, RingProtocolConstants.SetResistanceReplyFormat, resistanceIndex);
        var capacitanceReply = string.Format(CultureInfo.InvariantCulture, RingProtocolConstants.SetCapacitanceReplyFormat, capacitanceIndex);

        AppLogger.Write($"LOOP_WORK: request R={resistanceIndex}, C={capacitanceIndex}");

        for (var attempt = 0; attempt < 2; attempt++)
        {
            try
            {
                Thread.Sleep(50);
                port.DiscardInBuffer();
                port.Write(resistanceCommand);
                Thread.Sleep(50);
                port.Write(capacitanceCommand);
                Thread.Sleep(50);

                var first = port.ReadLine();
                var second = port.ReadLine();
                AppLogger.Write($"LOOP_WORK: attempt={attempt + 1}, response1='{first.Trim()}', response2='{second.Trim()}'");
                if (first.StartsWith(resistanceReply, StringComparison.Ordinal) &&
                    second.StartsWith(capacitanceReply, StringComparison.Ordinal))
                {
                    AppLogger.Write("LOOP_WORK: точка установлена успешно.");
                    return RingWorkResult.Ok;
                }
            }
            catch (Exception ex)
            {
                AppLogger.Write(ex, $"LOOP_WORK: attempt={attempt + 1}, ошибка установки точки");
            }

            Thread.Sleep(150);
        }

        AppLogger.Write("LOOP_WORK: не удалось установить точку после 2 попыток.");
        return RingWorkResult.Failed;
    }

    private static RingLoopParameterCell ParseLoopParameterCell(int index, string response, string prefix)
    {
        if (!response.StartsWith(prefix, StringComparison.Ordinal))
        {
            return new RingLoopParameterCell(index, 0f, 0f, false);
        }

        var payload = response[prefix.Length..].Trim();
        var separatorIndex = payload.IndexOf(' ');
        if (separatorIndex < 0)
        {
            return new RingLoopParameterCell(index, 0f, 0f, false);
        }

        var resistanceText = payload[..separatorIndex].Trim();
        var reactanceText = payload[(separatorIndex + 1)..].Trim();
        var hasResistance = float.TryParse(
            resistanceText,
            NumberStyles.Float,
            CultureInfo.InvariantCulture,
            out var resistance);
        var hasReactance = float.TryParse(
            reactanceText,
            NumberStyles.Float,
            CultureInfo.InvariantCulture,
            out var reactance);

        if (!hasResistance || !hasReactance || !float.IsFinite(resistance) || !float.IsFinite(reactance))
        {
            return new RingLoopParameterCell(index, 0f, 0f, false);
        }

        return new RingLoopParameterCell(index, resistance, reactance, true);
    }
}

public enum RingWorkResult
{
    Ok,
    Failed,
    NotConnected
}

public readonly record struct RingLoopParameterCell(int Index, float Resistance, float Reactance, bool HasValue);

public sealed record RingLoopParameterReadResult(
    bool Success,
    IReadOnlyList<RingLoopParameterCell> Cells,
    IReadOnlyList<LoopParameter> Parameters);
