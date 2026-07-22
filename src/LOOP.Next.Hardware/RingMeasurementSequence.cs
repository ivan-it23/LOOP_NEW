namespace LoopNext.Hardware;

/// <summary>
/// Defines the logical RES/CAP scan used by the ring verification workflow.
/// Logical CAP numbers stay aligned with measurement channels. Before a point
/// is sent to the universal 5 Tx ring firmware, the application maps the
/// high-frequency group to the physical CAP 6..10 bank.
/// </summary>
public static class RingMeasurementSequence
{
    public const int ExpressFullScanResistanceLimit = 2;
    public const int RingPhysicalGroupSize = 5;

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

    public static int GetCommandCapacitance(int channelCount, int logicalCapacitance)
        => GetCommandCapacitance(
            channelCount,
            logicalCapacitance,
            RingCapacitanceMappingProfile.CompatibilityFallback);

    public static int GetCommandCapacitance(
        int channelCount,
        int logicalCapacitance,
        RingCapacitanceMappingProfile mappingProfile)
    {
        // Оба текущих профиля используют физические банки универсального
        // 5-TX кольца. Раздельные значения профиля оставлены намеренно:
        // сигнатура 151 выбирает подтверждённую ветку, fallback сохраняет
        // поведение ещё не промаркированных колец в период миграции.
        var transmitterCount = channelCount / 2;
        var hasTwoCompleteFrequencyGroups =
            channelCount > 0 &&
            channelCount % 2 == 0 &&
            transmitterCount is >= 3 and <= RingPhysicalGroupSize;

        if (!hasTwoCompleteFrequencyGroups || logicalCapacitance <= transmitterCount)
        {
            return logicalCapacitance;
        }

        return logicalCapacitance + RingPhysicalGroupSize - transmitterCount;
    }

    public static int GetExpectedPhysicalCapacitance(int channelCount, int logicalCapacitance) =>
        GetCommandCapacitance(channelCount, logicalCapacitance);

    public static string GetStrategyName(bool expressMode, int resistanceIndex) =>
        UsesExpressGroups(expressMode, resistanceIndex) ? "express_groups" : "full_scan";
}
