using System.IO.Ports;
using System.Globalization;
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

    public static RingLoopParameterReadResult ReadLoopParameters(SerialPort port)
    {
        if (!port.IsOpen)
        {
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
                Thread.Sleep(50);

                var response = port.ReadLine();
                cells.Add(ParseLoopParameterCell(index, response, prefix));
            }
            catch
            {
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

        return new RingLoopParameterReadResult(parameters.Count > 0, cells, parameters);
    }

    public static RingWorkResult SetMeasurementPoint(SerialPort port, int resistanceIndex, int capacitanceIndex)
    {
        if (!port.IsOpen)
        {
            return RingWorkResult.NotConnected;
        }

        var resistanceCommand = string.Format(CultureInfo.InvariantCulture, RingProtocolConstants.SetResistanceCommandFormat, resistanceIndex);
        var capacitanceCommand = string.Format(CultureInfo.InvariantCulture, RingProtocolConstants.SetCapacitanceCommandFormat, capacitanceIndex);
        var resistanceReply = string.Format(CultureInfo.InvariantCulture, RingProtocolConstants.SetResistanceReplyFormat, resistanceIndex);
        var capacitanceReply = string.Format(CultureInfo.InvariantCulture, RingProtocolConstants.SetCapacitanceReplyFormat, capacitanceIndex);

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
                if (first.StartsWith(resistanceReply, StringComparison.Ordinal) &&
                    second.StartsWith(capacitanceReply, StringComparison.Ordinal))
                {
                    return RingWorkResult.Ok;
                }
            }
            catch
            {
                // The old Loop_work retries once after write/read failures.
            }

            Thread.Sleep(150);
        }

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
