using System.Globalization;
using System.Text;
using CommunityToolkit.Mvvm.ComponentModel;
using LoopNext.Core.Logging;
using LoopNext.Core.Models;
using LoopNext.Hardware;
using LoopNext.Metrology;

namespace LOOP_Next_App.ViewModels;

/// <summary>
/// Оповещения о состоянии памяти кольца и о ячейках таблицы на уровне шума.
/// </summary>
/// <remarks>
/// 23.09 коллеги 13 раз нажали «Режим АВТО» на кольце с пустой ячейкой Z8, и
/// приложение ни разу не объяснило, почему ничего не происходит: причина
/// писалась только в строку состояния. Здесь собрано всё, что должно было
/// появиться на экране: постоянный индикатор, диалог при подключении и
/// объяснение при отказе запуска.
/// </remarks>
public partial class MainPageViewModel
{
    private RingMemoryAuditResult _ringMemoryAudit = RingMemoryAuditResult.NotEvaluated;
    private string? _shownRingMemoryFingerprint;

    /// <summary>
    /// Состояние памяти кольца для подключённого прибора - строка панели «Состояние кольца».
    /// </summary>
    [ObservableProperty]
    public partial string RingMemoryStateText { get; set; } = "—";

    /// <summary>Когда показывать диалог о находках проверки памяти.</summary>
    private enum RingMemoryNotice
    {
        /// <summary>Только индикатор: о проблеме сообщит вызывающая сторона.</summary>
        None,

        /// <summary>Диалог, если этот набор находок ещё не показывали.</summary>
        IfNew
    }

    /// <summary>
    /// Проверяет прочитанную память кольца и обновляет индикатор.
    /// </summary>
    private RingMemoryAuditResult EvaluateRingMemory(
        IReadOnlyList<RingLoopParameterCell> cells,
        string correlation,
        RingMemoryNotice notice)
    {
        var channelCount = IsConnected ? _channelCount : 0;
        var audit = RingMemoryAudit.Evaluate(
            cells,
            channelCount,
            CurrentRingCapacitanceMappingProfile,
            IsConnected ? GetCalculatedChannelReactances() : null);
        _ringMemoryAudit = audit;
        RingMemoryStateText = FormatRingMemoryState(audit);

        AppLogger.WriteDiagnostic(
            "RING_MEMORY_AUDIT",
            audit.IsComplete ? (audit.HasMismatches ? "MISMATCH" : "OK") : "INCOMPLETE",
            $"correlation={correlation}|channels={channelCount}|profile={CurrentRingCapacitanceMappingProfile}|nothing_read={audit.NothingRead}|issues=[{string.Join(",", audit.Issues.Select(FormatIssueDiagnostic))}]|notice={notice}");

        if (audit.IsComplete && !audit.HasMismatches)
        {
            // Проблема ушла: если она вернётся, о ней нужно сказать снова.
            _shownRingMemoryFingerprint = null;
            return audit;
        }

        if (notice == RingMemoryNotice.IfNew &&
            !string.Equals(_shownRingMemoryFingerprint, audit.Fingerprint, StringComparison.Ordinal))
        {
            ShowRingMemoryMessage(audit);
        }

        return audit;
    }

    private void ShowRingMemoryMessage(RingMemoryAuditResult audit)
    {
        _shownRingMemoryFingerprint = audit.Fingerprint;
        var (title, message) = BuildRingMemoryMessage(audit, blockedModeName: null);
        RaiseUserMessage(title, message);
    }

    private void ResetRingMemoryState()
    {
        _ringMemoryAudit = RingMemoryAuditResult.NotEvaluated;
        _shownRingMemoryFingerprint = null;
        RingMemoryStateText = "—";
    }

    private float[] GetCalculatedChannelReactances()
    {
        var result = new float[_sondeParameters.Count];
        for (var channel = 0; channel < result.Length; channel++)
        {
            var parameter = _sondeParameters[channel];
            result[channel] = parameter.HasUsableGeometry
                ? RingCalibrationCalculator.Calculate(parameter).Reactance
                : float.NaN;
        }

        return result;
    }

    private static string FormatIssueDiagnostic(RingCellIssue issue) =>
        issue.Kind == RingCellIssueKind.ReactanceMismatch
            ? string.Create(
                CultureInfo.InvariantCulture,
                $"{issue.Kind}:Z{issue.CellIndex}:ch{issue.ChannelIndex}:stored={issue.StoredReactance:G6}:calc={issue.CalculatedReactance:G6}:nearest={issue.NearestChannelIndex}")
            : $"{issue.Kind}:Z{issue.CellIndex}:ch{issue.ChannelIndex}:R{issue.ResistanceIndex + 1}";

    private string FormatRingMemoryState(RingMemoryAuditResult audit)
    {
        if (audit.NothingRead)
        {
            return "Не прочитана";
        }

        var empty = audit.Issues
            .Where(issue => issue.Kind is RingCellIssueKind.ResistanceEmpty or RingCellIssueKind.ReactanceEmpty)
            .Select(issue => issue.CellIndex)
            .Distinct()
            .OrderBy(index => index)
            .ToArray();
        if (empty.Length > 0)
        {
            return $"Не заполнено: {string.Join(", ", empty.Select(index => $"Z{index}"))}";
        }

        var unreadable = audit.Issues
            .Where(issue => issue.IsUnreadable)
            .Select(issue => issue.CellIndex)
            .Distinct()
            .OrderBy(index => index)
            .ToArray();
        if (unreadable.Length > 0)
        {
            return $"Не прочитано: {string.Join(", ", unreadable.Select(index => $"Z{index}"))}";
        }

        if (audit.HasMismatches)
        {
            var count = audit.Mismatches.Select(issue => issue.ChannelIndex).Distinct().Count();
            return $"Не под этот прибор: {count} {PluralizeChannels(count)}";
        }

        return audit.ChannelCount > 0 ? "Заполнена" : "R1–R8 в порядке";
    }

    private static string PluralizeChannels(int count)
    {
        var lastTwo = count % 100;
        var last = count % 10;
        if (lastTwo is >= 11 and <= 14)
        {
            return "каналов";
        }

        return last switch
        {
            1 => "канал",
            >= 2 and <= 4 => "канала",
            _ => "каналов"
        };
    }

    /// <summary>«T4, 2001 кГц» - так канал подписан в заголовке таблицы.</summary>
    private string FormatChannelLabel(int channel)
    {
        if (channel < 0 || channel >= _sondeParameters.Count)
        {
            return $"канал {channel + 1}";
        }

        var transmitterCount = _sondeParameters.Count / 2;
        var transmitter = transmitterCount > 0 && channel >= transmitterCount
            ? channel - transmitterCount + 1
            : channel + 1;
        return string.Create(
            CultureInfo.InvariantCulture,
            $"T{transmitter}, {_sondeParameters[channel].Frequency / 1000f:F0} кГц");
    }

    private string DescribeRingAndTool()
    {
        var ring = _ringDeviceInfo.Signature is uint signature
            ? $"Кольцо {signature.ToString(CultureInfo.InvariantCulture)}"
            : "Кольцо";
        return IsConnected
            ? $"{ring}, прибор {ToolTypeName} № {SerialText}."
            : $"{ring}.";
    }

    // Режим называется либо «Режим АВТО», либо по сопротивлению: «R3».
    private static bool IsSingleResistanceMode(string modeName) =>
        modeName.Length > 1 && modeName[0] == 'R' && char.IsDigit(modeName[1]);

    private static string DescribeBlockedStart(string modeName) =>
        IsSingleResistanceMode(modeName)
            ? $"Измерение {modeName} не запущено"
            : $"«{modeName}» не запущен";

    private static string DescribeRetry(string modeName) =>
        IsSingleResistanceMode(modeName) ? $"измерение {modeName}" : $"«{modeName}»";

    /// <summary>
    /// Текст о находках проверки памяти кольца.
    /// </summary>
    /// <param name="blockedModeName">
    /// Режим, запуск которого отклонён из-за памяти кольца, или null, если
    /// сообщение показывается при подключении или чтении.
    /// </param>
    private (string Title, string Message) BuildRingMemoryMessage(
        RingMemoryAuditResult audit,
        string? blockedModeName)
    {
        var text = new StringBuilder();
        text.AppendLine(DescribeRingAndTool());

        if (audit.NothingRead)
        {
            text.AppendLine();
            text.AppendLine("Кольцо не вернуло содержимое памяти. Это ошибка связи, а не пустая память.");
            text.AppendLine();
            text.Append("Повторите «Прочитать из кольца». Если чтение снова не удастся, отключите и снова подключите кольцо.");
            return (
                blockedModeName is null
                    ? "Не удалось прочитать память кольца"
                    : $"{DescribeBlockedStart(blockedModeName)}: память кольца не прочитана",
                text.ToString());
        }

        var emptyCells = DescribeCells(audit.Issues.Where(issue =>
            issue.Kind is RingCellIssueKind.ResistanceEmpty or RingCellIssueKind.ReactanceEmpty));
        var unreadableCells = DescribeCells(audit.Issues.Where(issue => issue.IsUnreadable));
        var mismatches = audit.Mismatches.ToArray();

        if (emptyCells.Count == 0 && unreadableCells.Count == 0)
        {
            return BuildRingMismatchMessage(text, mismatches);
        }

        if (emptyCells.Count > 0)
        {
            text.AppendLine();
            text.AppendLine("Не заполнены ячейки памяти кольца:");
            foreach (var line in emptyCells)
            {
                text.AppendLine($"• {line}");
            }
        }

        if (unreadableCells.Count > 0)
        {
            text.AppendLine();
            text.AppendLine("Кольцо не ответило при чтении ячеек:");
            foreach (var line in unreadableCells)
            {
                text.AppendLine($"• {line}");
            }
        }

        text.AppendLine();
        text.AppendLine(blockedModeName is null
            ? "Без этих ячеек эталоны не рассчитываются, и заполнение таблицы не запустится."
            : $"{DescribeBlockedStart(blockedModeName)}: без этих ячеек эталоны не рассчитываются.");

        if (emptyCells.Count > 0)
        {
            var hasEmptyResistance = audit.Issues.Any(issue => issue.Kind == RingCellIssueKind.ResistanceEmpty);
            var hasEmptyReactance = audit.Issues.Any(issue => issue.Kind == RingCellIssueKind.ReactanceEmpty);
            var step = 1;
            text.AppendLine();
            text.AppendLine("Как заполнить (прибор и кольцо должны оставаться подключёнными):");
            if (hasEmptyReactance)
            {
                text.AppendLine($"{step++}. Вкладка «Поверка кольца» → «Рассчитать».");
            }

            text.AppendLine($"{step++}. Поставьте флажок «Редактировать».");
            if (hasEmptyResistance)
            {
                text.AppendLine($"{step++}. Впишите недостающие сопротивления в строку «Активное R, Ом» — номиналы R1…R8 указаны в паспорте кольца.");
            }

            text.AppendLine($"{step++}. «Записать в кольцо».");
            text.Append($"{step}. «Прочитать из кольца» — пустых ячеек остаться не должно.");
            if (blockedModeName is not null)
            {
                text.AppendLine();
                text.Append($"Затем повторите {DescribeRetry(blockedModeName)}.");
            }
        }

        if (unreadableCells.Count > 0)
        {
            text.AppendLine();
            text.AppendLine();
            text.Append(emptyCells.Count > 0
                ? "Непрочитанные ячейки — ошибка связи, а не пустая память: для них повторите «Прочитать из кольца». Если они снова не читаются, отключите и снова подключите кольцо."
                : "Это ошибка связи, а не пустая память: записывать в кольцо ничего не нужно. Повторите «Прочитать из кольца». Если ячейки снова не читаются, отключите и снова подключите кольцо.");
        }

        if (mismatches.Length > 0 && emptyCells.Count > 0)
        {
            text.AppendLine();
            text.AppendLine();
            text.Append(
                "Кроме того, с расчётом для этого прибора расходятся реактивности каналов " +
                $"{string.Join("; ", mismatches.Select(issue => $"{FormatChannelLabel(issue.ChannelIndex)} ({FormatDeviation(issue)})"))}. " +
                "Шаги выше заменят и их.");
        }

        var title = emptyCells.Count > 0
            ? blockedModeName is null
                ? "Память кольца не заполнена для этого прибора"
                : $"{DescribeBlockedStart(blockedModeName)}: память кольца не заполнена"
            : blockedModeName is null
                ? "Не удалось прочитать часть памяти кольца"
                : $"{DescribeBlockedStart(blockedModeName)}: часть памяти кольца не прочитана";
        return (title, text.ToString());
    }

    private (string Title, string Message) BuildRingMismatchMessage(StringBuilder text, IReadOnlyList<RingCellIssue> mismatches)
    {
        text.AppendLine();
        text.AppendLine("Реактивности в памяти кольца расходятся с расчётом для этого прибора:");
        foreach (var issue in mismatches)
        {
            var nearest = issue.NearestChannelIndex >= 0
                ? $", ближе к расчёту для {FormatChannelLabel(issue.NearestChannelIndex)}"
                : string.Empty;
            text.AppendLine(string.Create(
                CultureInfo.InvariantCulture,
                $"• {FormatChannelLabel(issue.ChannelIndex)} (Z{issue.CellIndex}): в кольце {issue.StoredReactance:F4} Ом, по расчёту {issue.CalculatedReactance:F4} Ом ({FormatDeviation(issue)}){nearest}"));
        }

        text.AppendLine();
        text.AppendLine("Эталоны по этим каналам будут рассчитаны из записанных значений, и таблица по ним может быть заполнена неверно.");
        if (mismatches.Any(issue => issue.NearestChannelIndex >= 0))
        {
            text.AppendLine("Значения, похожие на расчёт соседнего канала, обычно означают, что кольцо заполняла прежняя версия приложения со сдвигом ячеек.");
        }

        text.AppendLine();
        text.AppendLine("Если в кольцо записаны значения, измеренные по сертификату кольца, расхождение может быть законным — тогда оставьте их.");
        text.Append("Иначе перезапишите кольцо: «Рассчитать» → флажок «Редактировать» → «Записать в кольцо».");
        return ("Кольцо заполнено не под этот прибор", text.ToString());
    }

    private static string FormatDeviation(RingCellIssue issue)
    {
        var deviation = issue.RelativeDeviation;
        return float.IsFinite(deviation)
            ? string.Create(CultureInfo.InvariantCulture, $"{deviation * 100f:+0;-0;0} %")
            : "нет расчёта";
    }

    /// <summary>
    /// Строки «Z8 — реактивность канала T4, 2001 кГц», по одной на ячейку.
    /// </summary>
    private List<string> DescribeCells(IEnumerable<RingCellIssue> issues) =>
        issues
            .GroupBy(issue => issue.CellIndex)
            .OrderBy(group => group.Key)
            .Select(group =>
            {
                var parts = group
                    .Where(issue => issue.ResistanceIndex >= 0)
                    .Select(issue => $"сопротивление R{issue.ResistanceIndex + 1}")
                    .Concat(group
                        .Where(issue => issue.ChannelIndex >= 0)
                        .Select(issue => $"реактивность канала {FormatChannelLabel(issue.ChannelIndex)}"))
                    .ToList();

                return $"Z{group.Key} — {string.Join(" и ", parts)}";
            })
            .ToList();

    /// <summary>
    /// Записывает в журнал, какие ячейки таблицы при текущем шуме нельзя сравнить с допуском.
    /// </summary>
    /// <remarks>
    /// Только журнал: окно с этим списком перед каждым прогоном оказалось
    /// лишним для пользователя. Для разбора прогона строка NOISE_FLOOR
    /// по-прежнему показывает, какие отклонения объясняются шумом.
    /// </remarks>
    private void LogNoiseFloor(string correlation)
    {
        V10 airStdDev;
        int airSamples;
        lock (_airLock)
        {
            airStdDev = _localAir.AirStdDev;
            airSamples = _localAir.AcceptedSampleCount;
        }

        var cells = MeasurementNoiseFloor.Evaluate(
            _phaseReferences,
            in airStdDev,
            _channelCount,
            airSamples,
            SelectedAveraging,
            ThresholdPercent,
            AveragingOptions.ToArray());
        AppLogger.WriteDiagnostic(
            "RING_MEASUREMENT",
            "NOISE_FLOOR",
            $"correlation={correlation}|averaging={SelectedAveraging}|air_samples={airSamples}|threshold_percent={ThresholdPercent}|noisy_cells=[{string.Join(",", cells.Select(cell => FormattableString.Invariant($"R{cell.ResistanceIndex}:ch{cell.ChannelIndex}:ref_mG={cell.ReferencePhaseMg:F1}:noise_mG={cell.NoiseMg:F1}:required_averaging={cell.RequiredAveraging}")))}]");
    }

    /// <summary>
    /// Текст об отказе подключения кольца.
    /// </summary>
    private static (string Title, string Message) BuildRingConnectFailureMessage(
        string portName,
        RingConnectResult result)
    {
        const string checklist =
            "Проверьте:\n" +
            "• кольцо включено (горит светодиод «ПИТ») и находится рядом с компьютером;\n" +
            "• выбран порт именно кольца. Для Bluetooth-устройства Windows создаёт два COM-порта, и подключаться нужно через исходящий;\n" +
            "• затем повторите подключение.";

        switch (result.Status)
        {
            case RingConnection.RingConnectStatus.PortOpenFailed:
            {
                var reason = string.IsNullOrWhiteSpace(result.FailureReason)
                    ? string.Empty
                    : $"\n\nОтвет Windows: {result.FailureReason.Trim()}";
                var message = result.PortOpenFailure switch
                {
                    RingPortOpenFailure.Busy =>
                        $"Порт {portName} занят другой программой.{reason}\n\n" +
                        "Закройте программу, которая работает с этим портом (терминал, вторую копию приложения), и повторите подключение.",
                    RingPortOpenFailure.NotFound =>
                        $"Порта {portName} нет в системе: устройство отключено или удалено.{reason}\n\n" +
                        "Обновите список портов, выберите порт кольца заново и повторите подключение.",
                    RingPortOpenFailure.DeviceNotResponding =>
                        $"Порт {portName} есть, но устройство за ним не отвечает.{reason}\n\n" +
                        "Обычно это значит, что выбран не тот порт или кольцо недоступно по Bluetooth.\n\n" + checklist,
                    _ =>
                        $"Порт {portName} не открылся.{reason}\n\n" + checklist
                };
                return ("Не удалось открыть порт кольца", message);
            }

            case RingConnection.RingConnectStatus.NoResponse:
                return (
                    "Кольцо не ответило",
                    $"Порт {portName} открылся, но устройство на нём не ответило ни на одну команду кольца.\n\n" +
                    "Вероятно, выбран порт другого устройства или кольцо выключено.\n\n" + checklist);

            case RingConnection.RingConnectStatus.Rejected:
                return (
                    "Кольцо не опознано",
                    $"Устройство на порту {portName} ответило, но не так, как отвечает кольцо.\n\n" +
                    "Проверьте, что выбран порт кольца, а не другого устройства, и повторите подключение.");

            default:
                return ("Кольцо не подключено", $"Подключение к порту {portName} не выполнено: {result.Status}.");
        }
    }
}

