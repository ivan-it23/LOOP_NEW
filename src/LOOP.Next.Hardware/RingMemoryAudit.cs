using LoopNext.Core.Models;

namespace LoopNext.Hardware;

/// <summary>
/// Что не так с ячейкой памяти кольца.
/// </summary>
public enum RingCellIssueKind
{
    /// <summary>Сопротивление R1..R8 не записано.</summary>
    ResistanceEmpty,

    /// <summary>Ячейку с сопротивлением не удалось прочитать.</summary>
    ResistanceUnreadable,

    /// <summary>Реактивность канала не записана.</summary>
    ReactanceEmpty,

    /// <summary>Ячейку с реактивностью канала не удалось прочитать.</summary>
    ReactanceUnreadable,

    /// <summary>Реактивность записана, но не соответствует подключённому прибору.</summary>
    ReactanceMismatch
}

/// <summary>
/// Одна находка проверки памяти кольца.
/// </summary>
/// <param name="ChannelIndex">Канал прибора, которому нужна ячейка, или -1 для сопротивления.</param>
/// <param name="ResistanceIndex">Номер сопротивления с нуля (R1 = 0), или -1 для реактивности.</param>
/// <param name="NearestChannelIndex">
/// Для расхождения: канал той же частоты, на расчёт которого записанное значение
/// похоже больше, чем на свой, или -1.
/// </param>
public readonly record struct RingCellIssue(
    RingCellIssueKind Kind,
    int CellIndex,
    int ChannelIndex,
    int ResistanceIndex,
    float StoredReactance,
    float CalculatedReactance,
    int NearestChannelIndex)
{
    /// <summary>
    /// Без этой ячейки эталоны не рассчитываются, и заполнение таблицы невозможно.
    /// </summary>
    public bool IsBlocking => Kind != RingCellIssueKind.ReactanceMismatch;

    public bool IsUnreadable =>
        Kind is RingCellIssueKind.ResistanceUnreadable or RingCellIssueKind.ReactanceUnreadable;

    /// <summary>
    /// Относительное расхождение записанного с расчётом.
    /// </summary>
    public float RelativeDeviation =>
        CalculatedReactance > 0f && float.IsFinite(StoredReactance)
            ? (StoredReactance - CalculatedReactance) / CalculatedReactance
            : float.NaN;
}

/// <summary>
/// Итог проверки памяти кольца для подключённого прибора.
/// </summary>
public sealed record RingMemoryAuditResult(IReadOnlyList<RingCellIssue> Issues, bool NothingRead, int ChannelCount)
{
    public static RingMemoryAuditResult NotEvaluated { get; } = new(Array.Empty<RingCellIssue>(), false, 0);

    /// <summary>Эталоны можно рассчитать: нет ни пустых, ни непрочитанных нужных ячеек.</summary>
    public bool IsComplete => !NothingRead && Issues.All(issue => !issue.IsBlocking);

    public bool HasMismatches => Issues.Any(issue => issue.Kind == RingCellIssueKind.ReactanceMismatch);

    public bool HasUnreadable => NothingRead || Issues.Any(issue => issue.IsUnreadable);

    public IEnumerable<RingCellIssue> Blocking => Issues.Where(issue => issue.IsBlocking);

    public IEnumerable<RingCellIssue> Mismatches =>
        Issues.Where(issue => issue.Kind == RingCellIssueKind.ReactanceMismatch);

    /// <summary>
    /// Отпечаток набора находок: одинаковый отпечаток - то же самое сообщение.
    /// </summary>
    public string Fingerprint =>
        NothingRead
            ? "nothing_read"
            : string.Join(";", Issues.Select(issue => $"{issue.Kind}:{issue.CellIndex}:{issue.ChannelIndex}"));
}

/// <summary>
/// Проверяет, пригодна ли память кольца для подключённого прибора.
/// </summary>
/// <remarks>
/// Единственное место, где решается, чего в кольце не хватает. Прежде полнота
/// набора выяснялась в трёх местах, и сообщали о ней они по-разному, а
/// «Режим АВТО» не сообщал вовсе: коллеги 23.09 нажали его 13 раз, прежде чем
/// причину - пустую ячейку Z8 - нашли по журналу.
///
/// Проверяются только ячейки, нужные подключённому прибору. Пустые Z4 и Z9 у
/// прибора на четыре передатчика на кольце 151 принадлежат пятому передатчику,
/// а на кольце 241 ячеек Z8 и Z9 физически нет - ни то ни другое не ошибка.
/// </remarks>
public static class RingMemoryAudit
{
    /// <summary>
    /// Относительное расхождение с расчётом, выше которого значение считается чужим.
    /// </summary>
    public const float MismatchRelativeLimit = 0.25f;

    private const float MinimumUsableValue = 0.000001f;

    /// <param name="calculatedReactances">
    /// Расчётная реактивность по каналам прибора; NaN или отсутствие - расчёта нет,
    /// и сверка по этому каналу не выполняется.
    /// </param>
    public static RingMemoryAuditResult Evaluate(
        IReadOnlyList<RingLoopParameterCell> cells,
        int channelCount,
        RingCapacitanceMappingProfile mappingProfile,
        IReadOnlyList<float>? calculatedReactances = null)
    {
        ArgumentNullException.ThrowIfNull(cells);

        if (cells.Count == 0)
        {
            return new RingMemoryAuditResult(Array.Empty<RingCellIssue>(), true, channelCount);
        }

        var cellsByIndex = cells
            .GroupBy(cell => cell.Index)
            .ToDictionary(group => group.Key, group => group.Last());
        var issues = new List<RingCellIssue>();

        for (var resistance = 0; resistance < MeasurementTable.ResistanceCount; resistance++)
        {
            var state = Classify(cellsByIndex, resistance, cell => cell.Resistance);
            if (state != CellState.Usable)
            {
                issues.Add(new RingCellIssue(
                    state == CellState.Empty ? RingCellIssueKind.ResistanceEmpty : RingCellIssueKind.ResistanceUnreadable,
                    resistance,
                    -1,
                    resistance,
                    float.NaN,
                    float.NaN,
                    -1));
            }
        }

        var activeChannels = RingParameterLayout.GetActiveReactanceCellCount(channelCount);
        for (var channel = 0; channel < activeChannels; channel++)
        {
            var cellIndex = RingMeasurementSequence.GetChannelCellIndex(channelCount, channel, mappingProfile);
            var calculated = GetCalculated(calculatedReactances, channel);
            var state = Classify(cellsByIndex, cellIndex, cell => cell.Reactance);
            if (state != CellState.Usable)
            {
                issues.Add(new RingCellIssue(
                    state == CellState.Empty ? RingCellIssueKind.ReactanceEmpty : RingCellIssueKind.ReactanceUnreadable,
                    cellIndex,
                    channel,
                    -1,
                    float.NaN,
                    calculated,
                    -1));
                continue;
            }

            var stored = cellsByIndex[cellIndex].Reactance;
            if (!(calculated > MinimumUsableValue))
            {
                continue;
            }

            var nearest = FindCloserChannel(stored, calculated, channel, channelCount, calculatedReactances);
            var deviation = MathF.Abs(stored - calculated) / calculated;
            if (deviation > MismatchRelativeLimit || nearest >= 0)
            {
                issues.Add(new RingCellIssue(
                    RingCellIssueKind.ReactanceMismatch,
                    cellIndex,
                    channel,
                    -1,
                    stored,
                    calculated,
                    nearest));
            }
        }

        return new RingMemoryAuditResult(issues, false, channelCount);
    }

    /// <summary>
    /// Канал той же частоты, на расчёт которого записанное значение похоже больше, чем на свой.
    /// </summary>
    /// <remarks>
    /// Одного порога недостаточно. На кольце коллег от 23.09 высокочастотная
    /// группа была записана со сдвигом на одну ячейку, и расхождения с расчётом
    /// составили 73, 20 и 8 %: порог 25 % пропустил бы два канала из трёх. Но
    /// каждое из этих значений отличалось от расчёта соседнего канала менее чем
    /// на 0,5 %. Такое сравнение не зависит от масштаба и указывает на сдвиг
    /// прямо, тогда как правильно записанные ячейки лежат рядом со своим
    /// расчётом и далеко от соседних.
    /// </remarks>
    private static int FindCloserChannel(
        float stored,
        float calculated,
        int channel,
        int channelCount,
        IReadOnlyList<float>? calculatedReactances)
    {
        var bankSize = channelCount > 0 && channelCount % 2 == 0 ? channelCount / 2 : channelCount;
        if (bankSize <= 1)
        {
            return -1;
        }

        var bankStart = channel / bankSize * bankSize;
        var ownDistance = MathF.Abs(stored - calculated);
        var nearest = -1;
        var nearestDistance = ownDistance;
        for (var other = bankStart; other < bankStart + bankSize && other < channelCount; other++)
        {
            if (other == channel)
            {
                continue;
            }

            var otherCalculated = GetCalculated(calculatedReactances, other);
            if (!(otherCalculated > MinimumUsableValue))
            {
                continue;
            }

            var distance = MathF.Abs(stored - otherCalculated);
            if (distance < nearestDistance)
            {
                nearest = other;
                nearestDistance = distance;
            }
        }

        return nearest;
    }

    private static float GetCalculated(IReadOnlyList<float>? calculatedReactances, int channel) =>
        calculatedReactances is not null && channel >= 0 && channel < calculatedReactances.Count
            ? calculatedReactances[channel]
            : float.NaN;

    private enum CellState
    {
        Usable,
        Empty,
        Unreadable
    }

    private static CellState Classify(
        IReadOnlyDictionary<int, RingLoopParameterCell> cellsByIndex,
        int index,
        Func<RingLoopParameterCell, float> value)
    {
        if (!cellsByIndex.TryGetValue(index, out var cell))
        {
            return CellState.Unreadable;
        }

        if (cell.HasValue)
        {
            // Нули оставляет ZERASE: кольцо ответило, но значения в ячейке нет.
            var number = value(cell);
            return float.IsFinite(number) && number > MinimumUsableValue
                ? CellState.Usable
                : CellState.Empty;
        }

        return cell.ReadStatus == RingCellReadStatus.EmptyInMemory
            ? CellState.Empty
            : CellState.Unreadable;
    }
}
