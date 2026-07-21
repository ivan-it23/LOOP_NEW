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

    public static bool IsResistanceRequired(int index) =>
        index is >= 0 and < MeasurementTable.ResistanceCount;

    public static bool IsReactanceRequired(int index, int channelCount) =>
        index >= 0 && index < GetActiveReactanceCellCount(channelCount);

    public static RingParameterLayoutResult BuildParameterSet(
        IReadOnlyList<RingLoopParameterCell> cells,
        int channelCount)
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
            var resistanceRequired = IsResistanceRequired(index);
            var reactanceRequired = IsReactanceRequired(index, channelCount);
            var resistanceValid = !resistanceRequired ||
                                  hasCell && float.IsFinite(cell.Resistance) && cell.Resistance > MinimumUsableValue;
            var reactanceValid = !reactanceRequired ||
                                 hasCell && float.IsFinite(cell.Reactance) && cell.Reactance > MinimumUsableValue;

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
                Reactance = hasCell && float.IsFinite(cell.Reactance) ? cell.Reactance : 0f,
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
