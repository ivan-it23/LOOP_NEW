using LoopNext.Core.Models;

namespace LoopNext.Hardware;

// Describes the part of Z0..Z9 required by the connected tool. Every ring has
// eight resistor measures, while channel reactances number 6, 8 or 10.
public static class RingParameterLayout
{
    private const float MinimumUsableValue = 0.000001f;

    public static int GetActiveReactanceCellCount(int channelCount) =>
        Math.Clamp(channelCount, 0, RingProtocolConstants.WorkingParameterCellCount);

    public static int GetRequiredWorkingCellCount(int channelCount) =>
        Math.Max(
            MeasurementTable.ResistanceCount,
            GetActiveReactanceCellCount(channelCount));

    /// <summary>
    /// Сколько ячеек кольца обязана охватить запись.
    /// </summary>
    /// <remarks>
    /// Набор параметров пронумерован по каналам прибора, а ячейки - по
    /// физическим ёмкостям, и на кольце с десятью ёмкостями эти нумерации
    /// расходятся. Считать по числу каналов здесь нельзя: прибор на четыре
    /// передатчика занимает Z0..Z3 и Z5..Z8, то есть девять позиций, а не
    /// восемь. Ячейка Z8 при подсчёте по каналам выпадала из записи, и
    /// высокочастотный канал T4 навсегда оставался с чужой реактивностью -
    /// именно он и расходился с остальными в прогоне 22.09.
    ///
    /// Поэтому диапазон равен объединению двух требований: ячейки
    /// сопротивлений R1..R8 плюс все ячейки, в которые отображаются каналы.
    /// </remarks>
    public static int GetRequiredWriteCellCount(
        int channelCount,
        RingCapacitanceMappingProfile mappingProfile)
    {
        var required = MeasurementTable.ResistanceCount;
        var activeChannels = GetActiveReactanceCellCount(channelCount);
        for (var channel = 0; channel < activeChannels; channel++)
        {
            var cellIndex = RingMeasurementSequence.GetChannelCellIndex(
                channelCount,
                channel,
                mappingProfile);
            if (cellIndex >= 0 && cellIndex + 1 > required)
            {
                required = cellIndex + 1;
            }
        }

        return Math.Min(required, RingProtocolConstants.WorkingParameterCellCount);
    }

    public static bool IsResistanceRequired(int index) =>
        index is >= 0 and < MeasurementTable.ResistanceCount;

    public static bool IsReactanceRequired(int index, int channelCount) =>
        index >= 0 && index < GetActiveReactanceCellCount(channelCount);

    /// <summary>
    /// Ячейка, из которой берётся реактивность канала с этим индексом.
    /// </summary>
    /// <remarks>
    /// Для позиций вне активных каналов отображать нечего, и берётся та же
    /// ячейка: эти позиции в расчёте не участвуют, а совпадение индексов
    /// сохраняет прежнее поведение проверок полноты набора.
    /// </remarks>
    public static int GetReactanceCellIndex(
        int index,
        int channelCount,
        RingCapacitanceMappingProfile mappingProfile)
    {
        if (!IsReactanceRequired(index, channelCount))
        {
            return index;
        }

        var cellIndex = RingMeasurementSequence.GetChannelCellIndex(channelCount, index, mappingProfile);
        return cellIndex >= 0 && cellIndex < RingProtocolConstants.WorkingParameterCellCount
            ? cellIndex
            : index;
    }

    public static RingParameterLayoutResult BuildParameterSet(
        IReadOnlyList<RingLoopParameterCell> cells,
        int channelCount) =>
        BuildParameterSet(cells, channelCount, RingCapacitanceMappingProfile.CompatibilityFallback);

    /// <summary>
    /// Собирает рабочий набор параметров кольца из прочитанных ячеек.
    /// </summary>
    /// <remarks>
    /// Индекс набора имеет двойной смысл, унаследованный от эталона: по нему
    /// берётся и сопротивление <c>R1..R8</c>, и реактивность канала. Но ячейки
    /// кольца пронумерованы по физическим ёмкостям, а каналы прибора — подряд,
    /// и на кольце с десятью ёмкостями эти нумерации расходятся. Поэтому
    /// сопротивление берётся по индексу набора, а реактивность — по той
    /// физической ёмкости, которую приложение и назначит этому каналу командой
    /// <c>CAP</c>.
    /// </remarks>
    public static RingParameterLayoutResult BuildParameterSet(
        IReadOnlyList<RingLoopParameterCell> cells,
        int channelCount,
        RingCapacitanceMappingProfile mappingProfile)
    {
        ArgumentNullException.ThrowIfNull(cells);

        var requiredCellCount = GetRequiredWorkingCellCount(channelCount);
        var cellsByIndex = cells
            .Where(cell => cell.Index is >= 0 and < RingProtocolConstants.WorkingParameterCellCount)
            .GroupBy(cell => cell.Index)
            .ToDictionary(group => group.Key, group => group.Last());
        var parameters = new List<LoopParameter>(requiredCellCount);
        var invalidResistanceIndexes = new List<int>();
        var invalidReactanceIndexes = new List<int>();
        var completeCellCount = 0;

        for (var index = 0; index < requiredCellCount; index++)
        {
            var hasCell = cellsByIndex.TryGetValue(index, out var cell) && cell.HasValue;
            var reactanceCellIndex = GetReactanceCellIndex(index, channelCount, mappingProfile);
            var hasReactanceCell =
                cellsByIndex.TryGetValue(reactanceCellIndex, out var reactanceCell) && reactanceCell.HasValue;

            var resistanceRequired = IsResistanceRequired(index);
            var reactanceRequired = IsReactanceRequired(index, channelCount);
            var resistanceValid = !resistanceRequired ||
                                  hasCell && float.IsFinite(cell.Resistance) && cell.Resistance > MinimumUsableValue;
            var reactanceValid = !reactanceRequired ||
                                 hasReactanceCell && float.IsFinite(reactanceCell.Reactance) &&
                                 reactanceCell.Reactance > MinimumUsableValue;

            if (!resistanceValid)
            {
                invalidResistanceIndexes.Add(index);
            }

            if (!reactanceValid)
            {
                invalidReactanceIndexes.Add(index);
            }

            if (resistanceValid && reactanceValid)
            {
                completeCellCount++;
            }

            parameters.Add(new LoopParameter
            {
                Resistance = hasCell && float.IsFinite(cell.Resistance) ? cell.Resistance : 0f,
                Reactance = hasReactanceCell && float.IsFinite(reactanceCell.Reactance) ? reactanceCell.Reactance : 0f,
                Radius = 0.5f,
                AdditionalWireResistance = 0.5f
            });
        }

        return new RingParameterLayoutResult(
            parameters,
            requiredCellCount,
            completeCellCount,
            invalidResistanceIndexes,
            invalidReactanceIndexes);
    }
}

public readonly record struct RingParameterLayoutResult(
    IReadOnlyList<LoopParameter> Parameters,
    int RequiredCellCount,
    int CompleteCellCount,
    IReadOnlyList<int> InvalidResistanceIndexes,
    IReadOnlyList<int> InvalidReactanceIndexes)
{
    public bool IsComplete =>
        CompleteCellCount == RequiredCellCount &&
        InvalidResistanceIndexes.Count == 0 &&
        InvalidReactanceIndexes.Count == 0;
}
