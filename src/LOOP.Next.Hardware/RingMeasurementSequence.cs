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

    /// <summary>
    /// Физическая ёмкость, которую нужно назвать кольцу для логической точки.
    /// </summary>
    /// <remarks>
    /// Сдвиг существует только у колец с десятью ёмкостями: прибор на три или
    /// четыре передатчика занимает не весь первый банк, и его высокочастотная
    /// группа начинается с первой позиции второго банка (C6).
    ///
    /// У кольца на восемь ёмкостей банки короче и совпадают с каналами прибора
    /// один в один, поэтому сдвигать нечего: девятой позиции там нет, и команда
    /// <c>CAP 9</c> была бы подтверждена, но ни одного реле не переключила бы.
    /// </remarks>
    public static int GetCommandCapacitance(
        int channelCount,
        int logicalCapacitance,
        RingCapacitanceMappingProfile mappingProfile)
    {
        if (mappingProfile == RingCapacitanceMappingProfile.CompactFourTransmitter)
        {
            return logicalCapacitance;
        }

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

    /// <summary>
    /// Индекс ячейки <c>Z</c>, в которой лежит реактивность канала.
    /// </summary>
    /// <remarks>
    /// Ячейки кольца пронумерованы по физическим ёмкостям, а не по каналам
    /// прибора. Значит, эталонную реактивность канала нужно брать по тому же
    /// правилу, по которому каналу назначается ёмкость: иначе приложение
    /// командует правильной мерой и сравнивает её с чужим эталоном.
    ///
    /// Логическая ёмкость канала равна его номеру, считая с единицы.
    /// </remarks>
    public static int GetChannelCellIndex(
        int channelCount,
        int channelIndex,
        RingCapacitanceMappingProfile mappingProfile) =>
        GetCommandCapacitance(channelCount, channelIndex + 1, mappingProfile) - 1;

    /// <summary>
    /// Канал, которому принадлежит ячейка <c>Z</c>, либо -1.
    /// </summary>
    /// <remarks>
    /// На универсальном кольце прибор с тремя или четырьмя передатчиками
    /// занимает не все позиции: свободные относятся к пятому передатчику и
    /// этому прибору неизвестны. Их нельзя ни рассчитать, ни перезаписать - при
    /// записи такие ячейки обязаны сохранить прежнее значение кольца.
    /// </remarks>
    public static int GetCellChannelIndex(
        int channelCount,
        int cellIndex,
        RingCapacitanceMappingProfile mappingProfile)
    {
        for (var channel = 0; channel < channelCount; channel++)
        {
            if (GetChannelCellIndex(channelCount, channel, mappingProfile) == cellIndex)
            {
                return channel;
            }
        }

        return -1;
    }

    public static int GetExpectedPhysicalCapacitance(int channelCount, int logicalCapacitance) =>
        GetCommandCapacitance(channelCount, logicalCapacitance);

    public static string GetStrategyName(bool expressMode, int resistanceIndex) =>
        UsesExpressGroups(expressMode, resistanceIndex) ? "express_groups" : "full_scan";
}
