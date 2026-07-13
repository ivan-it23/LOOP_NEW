using System.Diagnostics;
using System.Globalization;
using System.IO.Ports;
using System.Runtime.CompilerServices;
using System.Text;
using LoopNext.Core.Logging;
using LoopNext.Core.Models;

namespace LoopNext.Hardware;

// Последовательный текстовый обмен с кольцом. Все операции одного SerialPort
// сериализованы: ответы разных команд не могут быть прочитаны соседним workflow.
public static class RingConnection
{
    private static readonly ConditionalWeakTable<SerialPort, object> PortLocks = new();

    public enum RingConnectStatus
    {
        Connected,
        PortOpenFailed,
        NoResponse,
        Rejected
    }

    public static RingConnectResult Connect(SerialPort port, string? correlationId = null)
    {
        var correlation = NormalizeCorrelation(correlationId);
        var operationId = AppLogger.NextOperationId("ring-connect");
        var operationStopwatch = Stopwatch.StartNew();
        lock (GetPortLock(port))
        {
            AppLogger.WriteDiagnostic(
                "RING_CONNECT",
                "BEGIN",
                $"correlation={correlation}|operation={operationId}|port={port.PortName}|was_open={port.IsOpen}|baud={RingProtocolConstants.BaudRate}|timeout_ms={RingProtocolConstants.TimeoutMs}");
            AppLogger.Write("=== НАЧАЛО ВКЛЮЧЕНИЯ КОЛЬЦА ===");
            AppLogger.Write($"Порт кольца={port.PortName}, BaudRate={RingProtocolConstants.BaudRate}, Timeout={RingProtocolConstants.TimeoutMs}ms");

            try
            {
                if (port.IsOpen)
                {
                    AppLogger.Write("Порт кольца уже открыт, закрываем перед переподключением.");
                    port.Close();
                }

                ConfigurePort(port);
                port.Open();
                AppLogger.Write("Порт кольца открыт успешно (9600, 8N1, ASCII, CRLF).");
                AppLogger.WriteDiagnostic(
                    "RING_CONNECT",
                    "PORT_OPENED",
                    $"correlation={correlation}|operation={operationId}|port={port.PortName}|elapsed_ms={operationStopwatch.ElapsedMilliseconds}|data_bits={port.DataBits}|parity={port.Parity}|stop_bits={port.StopBits}|handshake={port.Handshake}|newline=CRLF");
            }
            catch (Exception ex)
            {
                AppLogger.Write(ex, "ОШИБКА открытия порта кольца");
                AppLogger.WriteDiagnostic(
                    "RING_CONNECT",
                    "FAILED",
                    ex,
                    $"correlation={correlation}|operation={operationId}|port={port.PortName}|stage=open|elapsed_ms={operationStopwatch.ElapsedMilliseconds}");
                return new RingConnectResult(RingConnectStatus.PortOpenFailed, RingDeviceInfo.Unknown);
            }

            Exception? signatureError = null;
            if (TryExchangeNoLock(
                    port,
                    RingProtocolConstants.ReadSignatureCommand,
                    out var signatureResponse,
                    out signatureError,
                    correlation,
                    operationId,
                    "SGNREAD"))
            {
                AppLogger.Write($"SGNREAD: ответ '{signatureResponse.Trim()}'");
                if (RingProtocolParser.TryParseSignature(signatureResponse, out var signature))
                {
                    var device = RingDeviceInfo.Extended(signature);
                    AppLogger.Write($"=== КОЛЬЦО ПОДКЛЮЧЕНО: новая прошивка, SGN={signature}, ячеек={device.ImpedanceCellCount} ===");
                    LogConnectCompleted(correlation, operationId, port, device, "sgnread", operationStopwatch.ElapsedMilliseconds);
                    return new RingConnectResult(RingConnectStatus.Connected, device);
                }

                if (RingProtocolParser.IsUnknownCommand(signatureResponse))
                {
                    var device = RingDeviceInfo.Legacy;
                    AppLogger.Write("=== КОЛЬЦО ПОДКЛЮЧЕНО: старая прошивка без SGNREAD ===");
                    LogConnectCompleted(correlation, operationId, port, device, "sgnread_unknown", operationStopwatch.ElapsedMilliseconds);
                    return new RingConnectResult(RingConnectStatus.Connected, device);
                }

                AppLogger.Write("SGNREAD: получен неизвестный формат ответа, выполняется совместимая проверка '$'.");
            }
            else
            {
                AppLogger.Write(signatureError!, "SGNREAD: нет корректного ответа, выполняется совместимая проверка '$'");
            }

            Exception? legacyError = null;
            if (TryExchangeNoLock(
                    port,
                    RingProtocolConstants.LegacyIdentifyCommand,
                    out var legacyResponse,
                    out legacyError,
                    correlation,
                    operationId,
                    "LEGACY_IDENTIFY"))
            {
                AppLogger.Write($"Legacy identify '$': ответ '{legacyResponse.Trim()}'");
                if (RingProtocolParser.IsUnknownCommand(legacyResponse))
                {
                    AppLogger.Write("=== КОЛЬЦО ПОДКЛЮЧЕНО: старая прошивка, legacy fallback ===");
                    LogConnectCompleted(correlation, operationId, port, RingDeviceInfo.Legacy, "legacy_fallback", operationStopwatch.ElapsedMilliseconds);
                    return new RingConnectResult(RingConnectStatus.Connected, RingDeviceInfo.Legacy);
                }
            }
            else
            {
                AppLogger.Write(legacyError!, "Legacy identify '$': нет ответа");
            }

            CloseAfterFailedConnect(port);
            var status = signatureError is not null && legacyError is not null
                ? RingConnectStatus.NoResponse
                : RingConnectStatus.Rejected;
            AppLogger.WriteDiagnostic(
                "RING_CONNECT",
                "FAILED",
                $"correlation={correlation}|operation={operationId}|port={port.PortName}|status={status}|stage=identify|elapsed_ms={operationStopwatch.ElapsedMilliseconds}|sgn_error={signatureError?.GetType().Name ?? "none"}|legacy_error={legacyError?.GetType().Name ?? "none"}");
            return new RingConnectResult(status, RingDeviceInfo.Unknown);
        }
    }

    public static void Disconnect(SerialPort port, string? correlationId = null)
    {
        var correlation = NormalizeCorrelation(correlationId);
        var operationId = AppLogger.NextOperationId("ring-disconnect");
        lock (GetPortLock(port))
        {
            if (!port.IsOpen)
            {
                AppLogger.WriteDiagnostic(
                    "RING_CONNECT",
                    "DISCONNECT_SKIPPED",
                    $"correlation={correlation}|operation={operationId}|port={port.PortName}|reason=already_closed");
                return;
            }

            var stopwatch = Stopwatch.StartNew();
            AppLogger.WriteDiagnostic(
                "RING_CONNECT",
                "DISCONNECT_BEGIN",
                $"correlation={correlation}|operation={operationId}|port={port.PortName}");
            AppLogger.Write("=== ВЫКЛЮЧЕНИЕ КОЛЬЦА ===");
            port.Close();
            AppLogger.Write("Порт кольца закрыт.");
            AppLogger.WriteDiagnostic(
                "RING_CONNECT",
                "DISCONNECT_COMPLETED",
                $"correlation={correlation}|operation={operationId}|port={port.PortName}|elapsed_ms={stopwatch.ElapsedMilliseconds}");
        }
    }

    public static RingSignatureReadResult ReadSignature(SerialPort port, string? correlationId = null)
    {
        var correlation = NormalizeCorrelation(correlationId);
        var operationId = AppLogger.NextOperationId("ring-sgnread");
        lock (GetPortLock(port))
        {
            if (!port.IsOpen)
            {
                return new RingSignatureReadResult(false, null, $"Порт кольца {port.PortName} не открыт.");
            }

            if (!TryExchangeNoLock(
                    port,
                    RingProtocolConstants.ReadSignatureCommand,
                    out var response,
                    out var error,
                    correlation,
                    operationId,
                    "SGNREAD"))
            {
                return new RingSignatureReadResult(false, null, DescribeRingError(error!, port.PortName));
            }

            return RingProtocolParser.TryParseSignature(response, out var signature)
                ? new RingSignatureReadResult(true, signature, null)
                : new RingSignatureReadResult(false, null, $"Неожиданный ответ SGNREAD: '{response.Trim()}'.");
        }
    }

    public static RingWorkOutcome WriteSignature(SerialPort port, uint signature, string? correlationId = null)
    {
        var command = string.Format(CultureInfo.InvariantCulture, RingProtocolConstants.WriteSignatureCommandFormat, signature);
        var correlation = NormalizeCorrelation(correlationId);
        var operationId = AppLogger.NextOperationId("ring-sgnwrite");
        lock (GetPortLock(port))
        {
            return ExecuteAckCommandNoLock(
                port,
                command,
                response => RingProtocolParser.IsWriteSignatureReply(signature, response),
                "SGNWRITE",
                correlation,
                operationId);
        }
    }

    public static RingWorkOutcome EraseLoopParameters(SerialPort port, string? correlationId = null)
    {
        var correlation = NormalizeCorrelation(correlationId);
        var operationId = AppLogger.NextOperationId("ring-zerase");
        lock (GetPortLock(port))
        {
            return ExecuteAckCommandNoLock(
                port,
                RingProtocolConstants.EraseLoopParametersCommand,
                RingProtocolParser.IsEraseReply,
                "ZERASE",
                correlation,
                operationId);
        }
    }

    public static RingLoopParameterReadResult ReadLoopParameters(
        SerialPort port,
        RingDeviceInfo deviceInfo,
        string? correlationId = null)
    {
        var correlation = NormalizeCorrelation(correlationId);
        var operationId = AppLogger.NextOperationId("ring-memory-read");
        var operationStopwatch = Stopwatch.StartNew();
        lock (GetPortLock(port))
        {
            AppLogger.WriteDiagnostic(
                "RING_MEMORY",
                "READ_BEGIN",
                $"correlation={correlation}|operation={operationId}|port={port.PortName}|profile={deviceInfo.ProtocolVersion}|signature={deviceInfo.Signature?.ToString(CultureInfo.InvariantCulture) ?? "none"}|declared_cells={deviceInfo.ImpedanceCellCount}|working_cells={RingProtocolConstants.WorkingParameterCellCount}");
            AppLogger.Write("=== ЧТЕНИЕ ПАРАМЕТРОВ КОЛЬЦА ===");
            if (!port.IsOpen)
            {
                AppLogger.Write("ОШИБКА: порт кольца не открыт.");
                return new RingLoopParameterReadResult(false, Array.Empty<RingLoopParameterCell>(), Array.Empty<LoopParameter>());
            }

            var cellCount = deviceInfo.ImpedanceCellCount > 0
                ? deviceInfo.ImpedanceCellCount
                : RingProtocolConstants.LegacyMemoryCellCount;
            var cells = new List<RingLoopParameterCell>(cellCount);
            for (var index = 0; index < cellCount; index++)
            {
                var command = string.Format(CultureInfo.InvariantCulture, RingProtocolConstants.ReadLoopParameterCommandFormat, index);
                var cellStopwatch = Stopwatch.StartNew();
                try
                {
                    port.DiscardInBuffer();
                    LogComTransmit(correlation, operationId, port, "ZREAD", command, index);
                    port.Write(command);
                    AppLogger.Write($"ZREAD n={index}: отправлено '{command.Trim()}'");
                    Thread.Sleep(RingProtocolConstants.CommandDelayMs);

                    var response = port.ReadLine();
                    LogComReceive(correlation, operationId, port, "ZREAD", response, cellStopwatch.ElapsedMilliseconds, index);
                    AppLogger.Write($"ZREAD n={index}: ответ '{response.Trim()}'");
                    var parsed = RingProtocolParser.TryParseLoopParameterCell(index, response, out var cell);
                    var storedCell = parsed
                        ? cell
                        : new RingLoopParameterCell(index, 0f, 0f, false);
                    cells.Add(storedCell);
                    AppLogger.WriteDiagnostic(
                        "RING_MEMORY",
                        "CELL_READ",
                        $"correlation={correlation}|operation={operationId}|index={index}|role={(index < RingProtocolConstants.WorkingParameterCellCount ? "working" : "extended_preserved")}|raw={EscapeProtocolText(response)}|parsed={parsed}|resistance={storedCell.Resistance:G9}|reactance={storedCell.Reactance:G9}|elapsed_ms={cellStopwatch.ElapsedMilliseconds}");
                }
                catch (Exception ex)
                {
                    AppLogger.Write(ex, $"ZREAD n={index}: ошибка чтения");
                    AppLogger.WriteDiagnostic(
                        "RING_MEMORY",
                        "CELL_READ_FAILED",
                        ex,
                        $"correlation={correlation}|operation={operationId}|index={index}|elapsed_ms={cellStopwatch.ElapsedMilliseconds}");
                    cells.Add(new RingLoopParameterCell(index, 0f, 0f, false));
                }
            }

            // Математический контракт не меняется: в расчёты идут только прежние Z0..Z9.
            var parameters = new List<LoopParameter>(RingProtocolConstants.WorkingParameterCellCount);
            foreach (var cell in cells.Take(RingProtocolConstants.WorkingParameterCellCount))
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

            AppLogger.Write($"Память кольца прочитана: {cells.Count} ячеек; рабочих расчётных параметров: {parameters.Count}.");
            var invalidIndexes = string.Join(",", cells.Where(cell => !cell.HasValue).Select(cell => cell.Index));
            AppLogger.WriteDiagnostic(
                "RING_MEMORY",
                "READ_COMPLETED",
                $"correlation={correlation}|operation={operationId}|port={port.PortName}|cells={cells.Count}|valid_cells={cells.Count(cell => cell.HasValue)}|invalid_indexes=[{invalidIndexes}]|calculation_parameters={parameters.Count}|elapsed_ms={operationStopwatch.ElapsedMilliseconds}");
            return new RingLoopParameterReadResult(parameters.Count > 0, cells, parameters);
        }
    }

    public static RingLoopParameterWriteResult WriteLoopParameters(
        SerialPort port,
        IReadOnlyList<RingLoopParameterCell> cells,
        string? correlationId = null)
    {
        var correlation = NormalizeCorrelation(correlationId);
        var operationId = AppLogger.NextOperationId("ring-memory-write");
        var operationStopwatch = Stopwatch.StartNew();
        lock (GetPortLock(port))
        {
            AppLogger.WriteDiagnostic(
                "RING_MEMORY",
                "WRITE_BEGIN",
                $"correlation={correlation}|operation={operationId}|port={port.PortName}|input_cells={cells.Count}|allowed_range=Z0..Z9|preserved_range=Z10..Z31");
            AppLogger.Write("=== ЗАПИСЬ РАБОЧИХ ПАРАМЕТРОВ КОЛЬЦА Z0..Z9 ===");
            if (!port.IsOpen)
            {
                AppLogger.Write("ОШИБКА: порт кольца не открыт.");
                return new RingLoopParameterWriteResult(false, Array.Empty<RingLoopParameterWriteCell>());
            }

            // Не дополнять список до 32: Z10..Z31 новой прошивки могут содержать
            // служебные данные и при штатной записи должны остаться неизменными.
            var workingCells = cells
                .Where(cell => cell.Index is >= 0 and < RingProtocolConstants.WorkingParameterCellCount)
                .GroupBy(cell => cell.Index)
                .Select(group => group.Last())
                .OrderBy(cell => cell.Index)
                .ToArray();
            var results = new List<RingLoopParameterWriteCell>(workingCells.Length);
            foreach (var cell in workingCells)
            {
                var cellStopwatch = Stopwatch.StartNew();
                var resistanceText = cell.Resistance.ToString("G9", CultureInfo.InvariantCulture);
                var reactanceText = cell.Reactance.ToString("G9", CultureInfo.InvariantCulture);
                var command = string.Format(
                    CultureInfo.InvariantCulture,
                    RingProtocolConstants.WriteLoopParameterCommandFormat,
                    cell.Index,
                    resistanceText,
                    reactanceText);
                RingLoopParameterWriteCell? cellResult = null;
                string lastResponse = string.Empty;
                string? lastError = null;
                for (var attempt = 1; attempt <= RingProtocolConstants.MemoryCommandMaxAttempts; attempt++)
                {
                    var attemptStopwatch = Stopwatch.StartNew();
                    try
                    {
                        port.DiscardInBuffer();
                        LogComTransmit(correlation, operationId, port, "ZWRITE", command, cell.Index, attempt);
                        port.Write(command);
                        AppLogger.Write($"ZWRITE n={cell.Index}: попытка {attempt}/{RingProtocolConstants.MemoryCommandMaxAttempts}, отправлено '{command.Trim()}'");
                        Thread.Sleep(RingProtocolConstants.CommandDelayMs);

                        var response = port.ReadLine();
                        lastResponse = response.Trim();
                        LogComReceive(correlation, operationId, port, "ZWRITE", response, attemptStopwatch.ElapsedMilliseconds, cell.Index);
                        var success = RingProtocolParser.IsWriteLoopParameterReply(cell.Index, response);
                        AppLogger.Write($"ZWRITE n={cell.Index}: попытка {attempt}/{RingProtocolConstants.MemoryCommandMaxAttempts}, ответ '{lastResponse}', success={success}");
                        AppLogger.WriteDiagnostic(
                            "RING_MEMORY",
                            "CELL_WRITE_ATTEMPT",
                            $"correlation={correlation}|operation={operationId}|index={cell.Index}|attempt={attempt}|max_attempts={RingProtocolConstants.MemoryCommandMaxAttempts}|resistance={cell.Resistance:G9}|reactance={cell.Reactance:G9}|raw={EscapeProtocolText(response)}|ack={success}|elapsed_ms={attemptStopwatch.ElapsedMilliseconds}");
                        if (success)
                        {
                            cellResult = new RingLoopParameterWriteCell(cell.Index, true, lastResponse, null);
                            break;
                        }

                        lastError = $"Неожиданный ответ: '{lastResponse}'.";
                    }
                    catch (Exception ex)
                    {
                        lastError = ex.Message;
                        AppLogger.Write(ex, $"ZWRITE n={cell.Index}: попытка {attempt}/{RingProtocolConstants.MemoryCommandMaxAttempts}, ошибка записи");
                        AppLogger.WriteDiagnostic(
                            "RING_MEMORY",
                            "CELL_WRITE_ATTEMPT_FAILED",
                            ex,
                            $"correlation={correlation}|operation={operationId}|index={cell.Index}|attempt={attempt}|max_attempts={RingProtocolConstants.MemoryCommandMaxAttempts}|elapsed_ms={attemptStopwatch.ElapsedMilliseconds}");

                        if (attempt < RingProtocolConstants.MemoryCommandMaxAttempts)
                        {
                            TryReopenPortNoLock(port, correlation, operationId, attempt);
                        }
                    }

                    if (attempt < RingProtocolConstants.MemoryCommandMaxAttempts)
                    {
                        Thread.Sleep(RingProtocolConstants.RetryDelayMs);
                    }
                }

                cellResult ??= new RingLoopParameterWriteCell(cell.Index, false, lastResponse, lastError);
                results.Add(cellResult.Value);
                AppLogger.WriteDiagnostic(
                    "RING_MEMORY",
                    cellResult.Value.Success ? "CELL_WRITE" : "CELL_WRITE_FAILED",
                    $"correlation={correlation}|operation={operationId}|index={cell.Index}|resistance={cell.Resistance:G9}|reactance={cell.Reactance:G9}|raw={EscapeProtocolText(lastResponse)}|ack={cellResult.Value.Success}|error={EscapeProtocolText(lastError ?? "none")}|elapsed_ms={cellStopwatch.ElapsedMilliseconds}");
            }

            var allSucceeded = workingCells.Length > 0 && results.All(cell => cell.Success);
            AppLogger.Write(allSucceeded
                ? "ZWRITE: запись рабочих ячеек завершена успешно; Z10..Z31 не изменялись."
                : "ZWRITE: запись рабочих ячеек завершена с ошибками.");
            var failedIndexes = string.Join(",", results.Where(cell => !cell.Success).Select(cell => cell.Index));
            AppLogger.WriteDiagnostic(
                "RING_MEMORY",
                "WRITE_COMPLETED",
                $"correlation={correlation}|operation={operationId}|attempted={workingCells.Length}|succeeded={results.Count(cell => cell.Success)}|failed_indexes=[{failedIndexes}]|preserved_range=Z10..Z31|success={allSucceeded}|elapsed_ms={operationStopwatch.ElapsedMilliseconds}");
            return new RingLoopParameterWriteResult(allSucceeded, results);
        }
    }

    public static RingWorkOutcome SetMeasurementPoint(
        SerialPort port,
        int resistanceIndex,
        int capacitanceIndex,
        string? correlationId = null)
    {
        var correlation = NormalizeCorrelation(correlationId);
        var operationId = AppLogger.NextOperationId("ring-point");
        var operationStopwatch = Stopwatch.StartNew();
        lock (GetPortLock(port))
        {
            AppLogger.WriteDiagnostic(
                "RING_POINT",
                "BEGIN",
                $"correlation={correlation}|operation={operationId}|port={port.PortName}|R={resistanceIndex}|C={capacitanceIndex}|port_open={port.IsOpen}");
            if (resistanceIndex is < RingProtocolConstants.MinimumResistanceIndex or > RingProtocolConstants.MaximumResistanceIndex ||
                capacitanceIndex is < RingProtocolConstants.MinimumCapacitanceIndex or > RingProtocolConstants.MaximumCapacitanceIndex)
            {
                AppLogger.WriteDiagnostic(
                    "RING_POINT",
                    "REJECTED",
                    $"correlation={correlation}|operation={operationId}|R={resistanceIndex}|C={capacitanceIndex}|reason=out_of_range|elapsed_ms={operationStopwatch.ElapsedMilliseconds}");
                return RingWorkOutcome.Failed($"Недопустимая точка кольца R={resistanceIndex}, C={capacitanceIndex}.");
            }

            if (!port.IsOpen)
            {
                AppLogger.Write("ОШИБКА: невозможно установить точку кольца — порт не открыт.");
                AppLogger.WriteDiagnostic(
                    "RING_POINT",
                    "REJECTED",
                    $"correlation={correlation}|operation={operationId}|R={resistanceIndex}|C={capacitanceIndex}|reason=port_closed|elapsed_ms={operationStopwatch.ElapsedMilliseconds}");
                return RingWorkOutcome.NotConnected($"Порт кольца {port.PortName} не открыт.");
            }

            var resistanceCommand = string.Format(CultureInfo.InvariantCulture, RingProtocolConstants.SetResistanceCommandFormat, resistanceIndex);
            var capacitanceCommand = string.Format(CultureInfo.InvariantCulture, RingProtocolConstants.SetCapacitanceCommandFormat, capacitanceIndex);

            AppLogger.Write($"LOOP_WORK: request R={resistanceIndex}, C={capacitanceIndex}");
            var maxAttempts = RingProtocolConstants.SetPointMaxAttempts;
            string? lastReason = null;
            for (var attempt = 0; attempt < maxAttempts; attempt++)
            {
                var stopwatch = Stopwatch.StartNew();
                try
                {
                    Thread.Sleep(RingProtocolConstants.CommandDelayMs);
                    port.DiscardInBuffer();
                    LogComTransmit(correlation, operationId, port, "RES", resistanceCommand, resistanceIndex, attempt + 1);
                    port.Write(resistanceCommand);
                    Thread.Sleep(RingProtocolConstants.CommandDelayMs);
                    LogComTransmit(correlation, operationId, port, "CAP", capacitanceCommand, capacitanceIndex, attempt + 1);
                    port.Write(capacitanceCommand);
                    Thread.Sleep(RingProtocolConstants.CommandDelayMs);

                    var first = port.ReadLine();
                    var second = port.ReadLine();
                    stopwatch.Stop();
                    AppLogger.Write(
                        $"LOOP_WORK: attempt={attempt + 1}/{maxAttempts}, elapsed_ms={stopwatch.ElapsedMilliseconds}, " +
                        $"response1='{first.Trim()}', response2='{second.Trim()}'");
                    var resistanceAcknowledged = RingProtocolParser.IsResistanceReply(resistanceIndex, first);
                    var capacitanceAcknowledged = RingProtocolParser.IsCapacitanceReply(capacitanceIndex, second);
                    AppLogger.WriteDiagnostic(
                        "RING_COM",
                        "POINT_RESPONSES",
                        $"correlation={correlation}|operation={operationId}|port={port.PortName}|attempt={attempt + 1}|R={resistanceIndex}|C={capacitanceIndex}|res_response={EscapeProtocolText(first)}|cap_response={EscapeProtocolText(second)}|res_ack={resistanceAcknowledged}|cap_ack={capacitanceAcknowledged}|attempt_elapsed_ms={stopwatch.ElapsedMilliseconds}|total_elapsed_ms={operationStopwatch.ElapsedMilliseconds}");
                    if (resistanceAcknowledged && capacitanceAcknowledged)
                    {
                        AppLogger.Write("LOOP_WORK: точка установлена успешно.");
                        AppLogger.WriteDiagnostic(
                            "RING_POINT",
                            "ACKNOWLEDGED",
                            $"correlation={correlation}|operation={operationId}|port={port.PortName}|R={resistanceIndex}|C={capacitanceIndex}|attempt={attempt + 1}|elapsed_ms={operationStopwatch.ElapsedMilliseconds}");
                        return RingWorkOutcome.Ok();
                    }

                    lastReason =
                        $"Неожиданный ответ кольца на порту {port.PortName}: '{first.Trim()}' / '{second.Trim()}'.";
                    AppLogger.Write($"LOOP_WORK: attempt={attempt + 1}/{maxAttempts}, {lastReason}");
                    AppLogger.WriteDiagnostic(
                        "RING_POINT",
                        "ATTEMPT_REJECTED",
                        $"correlation={correlation}|operation={operationId}|R={resistanceIndex}|C={capacitanceIndex}|attempt={attempt + 1}|reason=unexpected_response|elapsed_ms={operationStopwatch.ElapsedMilliseconds}");
                }
                catch (Exception ex)
                {
                    lastReason = DescribeRingError(ex, port.PortName);
                    AppLogger.Write(ex, $"LOOP_WORK: attempt={attempt + 1}/{maxAttempts}, ошибка установки точки");
                    AppLogger.WriteDiagnostic(
                        "RING_POINT",
                        "ATTEMPT_FAILED",
                        ex,
                        $"correlation={correlation}|operation={operationId}|port={port.PortName}|R={resistanceIndex}|C={capacitanceIndex}|attempt={attempt + 1}|elapsed_ms={operationStopwatch.ElapsedMilliseconds}");
                }

                if (attempt < maxAttempts - 1)
                {
                    TryReopenPortNoLock(port, correlation, operationId, attempt + 1);
                    Thread.Sleep(RingProtocolConstants.RetryDelayMs);
                }
            }

            AppLogger.Write($"LOOP_WORK: не удалось установить точку после {maxAttempts} попыток.");
            AppLogger.WriteDiagnostic(
                "RING_POINT",
                "FAILED",
                $"correlation={correlation}|operation={operationId}|port={port.PortName}|R={resistanceIndex}|C={capacitanceIndex}|attempts={maxAttempts}|reason={EscapeProtocolText(lastReason ?? "no_response")}|elapsed_ms={operationStopwatch.ElapsedMilliseconds}");
            return RingWorkOutcome.Failed(lastReason ?? $"Кольцо на порту {port.PortName} не отвечает.");
        }
    }

    private static object GetPortLock(SerialPort port) => PortLocks.GetValue(port, static _ => new object());

    private static void ConfigurePort(SerialPort port)
    {
        port.BaudRate = RingProtocolConstants.BaudRate;
        port.DataBits = 8;
        port.Parity = Parity.None;
        port.StopBits = StopBits.One;
        port.Handshake = Handshake.None;
        port.Encoding = Encoding.ASCII;
        port.NewLine = "\r\n";
        port.ReadTimeout = RingProtocolConstants.TimeoutMs;
        port.WriteTimeout = RingProtocolConstants.TimeoutMs;
    }

    private static bool TryExchangeNoLock(
        SerialPort port,
        string command,
        out string response,
        out Exception? error,
        string correlation,
        string operationId,
        string commandName)
    {
        response = string.Empty;
        error = null;
        var stopwatch = Stopwatch.StartNew();
        try
        {
            port.DiscardInBuffer();
            LogComTransmit(correlation, operationId, port, commandName, command);
            port.Write(command);
            response = port.ReadLine();
            AppLogger.WriteDiagnostic(
                "RING_COM",
                "RX",
                $"correlation={correlation}|operation={operationId}|port={port.PortName}|command={commandName}|raw={EscapeProtocolText(response)}|elapsed_ms={stopwatch.ElapsedMilliseconds}");
            return true;
        }
        catch (Exception ex)
        {
            error = ex;
            AppLogger.WriteDiagnostic(
                "RING_COM",
                "ERROR",
                ex,
                $"correlation={correlation}|operation={operationId}|port={port.PortName}|command={commandName}|elapsed_ms={stopwatch.ElapsedMilliseconds}");
            return false;
        }
    }

    private static RingWorkOutcome ExecuteAckCommandNoLock(
        SerialPort port,
        string command,
        Func<string, bool> isExpectedReply,
        string operationName,
        string correlation,
        string operationId)
    {
        if (!port.IsOpen)
        {
            return RingWorkOutcome.NotConnected($"Порт кольца {port.PortName} не открыт.");
        }

        if (!TryExchangeNoLock(
                port,
                command,
                out var response,
                out var error,
                correlation,
                operationId,
                operationName))
        {
            return RingWorkOutcome.Failed(DescribeRingError(error!, port.PortName));
        }

        AppLogger.Write($"{operationName}: команда '{command.Trim()}', ответ '{response.Trim()}'");
        var acknowledged = isExpectedReply(response);
        AppLogger.WriteDiagnostic(
            "RING_ENGINEERING",
            "COMMAND_COMPLETED",
            $"correlation={correlation}|operation={operationId}|port={port.PortName}|command={operationName}|raw={EscapeProtocolText(response)}|ack={acknowledged}");
        return acknowledged
            ? RingWorkOutcome.Ok()
            : RingWorkOutcome.Failed($"Неожиданный ответ {operationName}: '{response.Trim()}'.");
    }

    private static void TryReopenPortNoLock(
        SerialPort port,
        string correlation,
        string operationId,
        int failedAttempt)
    {
        var stopwatch = Stopwatch.StartNew();
        try
        {
            AppLogger.WriteDiagnostic(
                "RING_COM",
                "REOPEN_BEGIN",
                $"correlation={correlation}|operation={operationId}|port={port.PortName}|after_attempt={failedAttempt}|was_open={port.IsOpen}");
            AppLogger.Write($"LOOP_WORK: переоткрытие порта {port.PortName} для сброса состояния...");
            if (port.IsOpen)
            {
                port.Close();
            }

            Thread.Sleep(RingProtocolConstants.ReopenDelayMs);
            ConfigurePort(port);
            port.Open();
            port.DtrEnable = true;
            port.RtsEnable = true;
            port.DiscardInBuffer();
            port.DiscardOutBuffer();
            AppLogger.Write($"LOOP_WORK: порт {port.PortName} переоткрыт успешно.");
            AppLogger.WriteDiagnostic(
                "RING_COM",
                "REOPEN_COMPLETED",
                $"correlation={correlation}|operation={operationId}|port={port.PortName}|after_attempt={failedAttempt}|elapsed_ms={stopwatch.ElapsedMilliseconds}|dtr={port.DtrEnable}|rts={port.RtsEnable}");
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, $"LOOP_WORK: не удалось переоткрыть порт {port.PortName}");
            AppLogger.WriteDiagnostic(
                "RING_COM",
                "REOPEN_FAILED",
                ex,
                $"correlation={correlation}|operation={operationId}|port={port.PortName}|after_attempt={failedAttempt}|elapsed_ms={stopwatch.ElapsedMilliseconds}");
        }
    }

    private static void LogConnectCompleted(
        string correlation,
        string operationId,
        SerialPort port,
        RingDeviceInfo device,
        string detectionPath,
        long elapsedMilliseconds)
    {
        AppLogger.WriteDiagnostic(
            "RING_CONNECT",
            "COMPLETED",
            $"correlation={correlation}|operation={operationId}|port={port.PortName}|profile={device.ProtocolVersion}|signature={device.Signature?.ToString(CultureInfo.InvariantCulture) ?? "none"}|memory_cells={device.ImpedanceCellCount}|detection_path={detectionPath}|elapsed_ms={elapsedMilliseconds}");
    }

    private static void LogComTransmit(
        string correlation,
        string operationId,
        SerialPort port,
        string commandName,
        string command,
        int? index = null,
        int? attempt = null)
    {
        AppLogger.WriteDiagnostic(
            "RING_COM",
            "TX",
            $"correlation={correlation}|operation={operationId}|port={port.PortName}|command={commandName}|index={index?.ToString(CultureInfo.InvariantCulture) ?? "none"}|attempt={attempt?.ToString(CultureInfo.InvariantCulture) ?? "none"}|raw={EscapeProtocolText(command)}");
    }

    private static void LogComReceive(
        string correlation,
        string operationId,
        SerialPort port,
        string commandName,
        string response,
        long elapsedMilliseconds,
        int? index = null)
    {
        AppLogger.WriteDiagnostic(
            "RING_COM",
            "RX",
            $"correlation={correlation}|operation={operationId}|port={port.PortName}|command={commandName}|index={index?.ToString(CultureInfo.InvariantCulture) ?? "none"}|raw={EscapeProtocolText(response)}|elapsed_ms={elapsedMilliseconds}");
    }

    private static string NormalizeCorrelation(string? correlationId) =>
        string.IsNullOrWhiteSpace(correlationId) ? "none" : EscapeProtocolText(correlationId);

    private static string EscapeProtocolText(string value) =>
        value
            .Replace("\r", "\\r", StringComparison.Ordinal)
            .Replace("\n", "\\n", StringComparison.Ordinal)
            .Replace("|", "/", StringComparison.Ordinal)
            .Trim();

    private static void CloseAfterFailedConnect(SerialPort port)
    {
        try
        {
            if (port.IsOpen)
            {
                port.Close();
            }
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Не удалось закрыть порт после отклонённого подключения кольца");
        }
    }

    private static string DescribeRingError(Exception ex, string portName) => ex switch
    {
        TimeoutException =>
            $"Тайм-аут обмена с кольцом на порту {portName}. Проверьте питание и кабель кольца.",
        UnauthorizedAccessException =>
            $"Порт {portName} занят другим приложением или недоступен.",
        System.IO.IOException io =>
            $"Ошибка ввода-вывода на порту {portName}: {io.Message}",
        InvalidOperationException =>
            $"Порт {portName} закрыт или находится в недопустимом состоянии.",
        _ => $"{ex.GetType().Name} на порту {portName}: {ex.Message}"
    };
}

public enum RingProtocolVersion
{
    Unknown,
    Legacy,
    Extended
}

public readonly record struct RingDeviceInfo(
    RingProtocolVersion ProtocolVersion,
    uint? Signature,
    int ImpedanceCellCount)
{
    public static RingDeviceInfo Unknown => new(RingProtocolVersion.Unknown, null, 0);

    public static RingDeviceInfo Legacy => new(
        RingProtocolVersion.Legacy,
        null,
        RingProtocolConstants.LegacyMemoryCellCount);

    public static RingDeviceInfo Extended(uint signature) => new(
        RingProtocolVersion.Extended,
        signature,
        RingProtocolConstants.ExtendedMemoryCellCount);
}

public readonly record struct RingConnectResult(
    RingConnection.RingConnectStatus Status,
    RingDeviceInfo DeviceInfo)
{
    public bool IsConnected => Status == RingConnection.RingConnectStatus.Connected;
}

public readonly record struct RingSignatureReadResult(bool Success, uint? Signature, string? ErrorMessage);

public enum RingWorkResult
{
    Ok,
    Failed,
    NotConnected
}

public readonly record struct RingWorkOutcome(RingWorkResult Result, string? FailureReason)
{
    public bool IsOk => Result == RingWorkResult.Ok;

    public static RingWorkOutcome Ok() => new(RingWorkResult.Ok, null);

    public static RingWorkOutcome Failed(string reason) => new(RingWorkResult.Failed, reason);

    public static RingWorkOutcome NotConnected(string reason) => new(RingWorkResult.NotConnected, reason);
}

public readonly record struct RingLoopParameterCell(int Index, float Resistance, float Reactance, bool HasValue);

public readonly record struct RingLoopParameterWriteCell(int Index, bool Success, string Response, string? ErrorMessage);

public sealed record RingLoopParameterReadResult(
    bool Success,
    IReadOnlyList<RingLoopParameterCell> Cells,
    IReadOnlyList<LoopParameter> Parameters);

public sealed record RingLoopParameterWriteResult(
    bool Success,
    IReadOnlyList<RingLoopParameterWriteCell> Cells);
