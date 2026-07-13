namespace LoopNext.Hardware;

/// <summary>
/// Defines the logical RES/CAP scan used by the ring verification workflow.
/// Logical CAP numbers are sent unchanged: the 4 Tx ring firmware owns the
/// documented physical shift from CAP 5..8 to positions 6..9.
/// </summary>
public static class RingMeasurementSequence
{
    public const int ExpressFullScanResistanceLimit = 2;

    public static bool UsesExpressGroups(bool expressMode, int resistanceIndex) =>
        expressMode && resistanceIndex > ExpressFullScanResistanceLimit;

    public static int GetNextLogicalCapacitance(
        bool expressMode,
        int resistanceIndex,
        int capacitanceIndex,
        int channelCount)
    {
        return UsesExpressGroups(expressMode, resistanceIndex)
            ? capacitanceIndex + GetExpressGroupSize(channelCount)
            : capacitanceIndex + 1;
    }

    public static IReadOnlyList<int> GetActiveChannels(
        bool expressMode,
        int resistanceIndex,
        int capacitanceIndex,
        int channelCount)
    {
        var start = capacitanceIndex - 1;
        if (start < 0 || start >= channelCount)
        {
            return Array.Empty<int>();
        }

        if (!UsesExpressGroups(expressMode, resistanceIndex))
        {
            return new[] { start };
        }

        var groupSize = GetExpressGroupSize(channelCount);
        var count = Math.Min(groupSize, channelCount - start);
        return Enumerable.Range(start, count).ToArray();
    }

    public static int GetExpressGroupSize(int channelCount) =>
        Math.Max(1, channelCount / 2);

    public static int GetExpectedPhysicalCapacitance(int channelCount, int logicalCapacitance) =>
        channelCount == 8 && logicalCapacitance >= 5
            ? logicalCapacitance + 1
            : logicalCapacitance;

    public static string GetStrategyName(bool expressMode, int resistanceIndex) =>
        UsesExpressGroups(expressMode, resistanceIndex) ? "express_groups" : "full_scan";
}
