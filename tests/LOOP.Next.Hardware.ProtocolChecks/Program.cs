using LoopNext.Hardware;

var checks = new List<(string Name, Action Run)>
{
    ("SGNREAD parses uint32", () =>
    {
        Require(RingProtocolParser.TryParseSignature("SGN=4294967295\r\n", out var signature));
        Require(signature == uint.MaxValue);
    }),
    ("Unknown command identifies legacy firmware", () =>
        Require(RingProtocolParser.IsUnknownCommand("Unknown command!\r\n"))),
    ("Legacy ZREAD response parses", () =>
    {
        Require(RingProtocolParser.TryParseLoopParameterCell(0, "Z0 1.000000 0.964000\r\n", out var cell));
        Require(cell.HasValue && NearlyEqual(cell.Resistance, 1f) && NearlyEqual(cell.Reactance, 0.964f));
    }),
    ("Extended ZREAD response parses", () =>
    {
        Require(RingProtocolParser.TryParseLoopParameterCell(31, "READ Z31 1.0 2.0\r\n", out var cell));
        Require(cell.HasValue && NearlyEqual(cell.Resistance, 1f) && NearlyEqual(cell.Reactance, 2f));
    }),
    ("ZREAD index mismatch is rejected", () =>
        Require(!RingProtocolParser.TryParseLoopParameterCell(1, "READ Z10 1.0 2.0", out _))),
    ("Non-finite ZREAD values are rejected", () =>
        Require(!RingProtocolParser.TryParseLoopParameterCell(0, "READ Z0 NaN 2.0", out _))),
    ("Legacy and extended RES/CAP replies are accepted", () =>
    {
        Require(RingProtocolParser.IsResistanceReply(8, "RES=8 OK"));
        Require(RingProtocolParser.IsResistanceReply(8, "RES=8 OK!"));
        Require(RingProtocolParser.IsCapacitanceReply(5, "CAP=5 OK!"));
    }),
    ("ZWRITE, ZERASE and SGNWRITE replies parse", () =>
    {
        Require(RingProtocolParser.IsWriteLoopParameterReply(9, "WRITE Z9 OK!"));
        Require(RingProtocolParser.IsEraseReply("ERASE OK!"));
        Require(RingProtocolParser.IsWriteSignatureReply(42, "WRITE SGN=42 OK!"));
    }),
    ("Firmware profiles retain 10 working cells and expose 32 extended cells", () =>
    {
        Require(RingProtocolConstants.WorkingParameterCellCount == 10);
        Require(RingDeviceInfo.Legacy.ImpedanceCellCount == 10);
        Require(RingDeviceInfo.Extended(42).ImpedanceCellCount == 32);
    }),
    ("Ring parameter layout writes eight cells for 3 Tx and 4 Tx", () =>
    {
        Require(RingParameterLayout.GetActiveReactanceCellCount(6) == 6);
        Require(RingParameterLayout.GetRequiredWorkingCellCount(6) == 8);
        Require(RingParameterLayout.GetRequiredWorkingCellCount(8) == 8);
        Require(RingParameterLayout.GetRequiredWorkingCellCount(10) == 10);
    }),
    ("Fresh 4 Tx ring accepts eight resistances and eight reactances", () =>
    {
        var cells = CreateRingCells(
            resistanceCount: 8,
            reactanceCount: 8,
            returnedCellCount: 10);
        var layout = RingParameterLayout.BuildParameterSet(cells, 8);

        Require(layout.IsComplete);
        Require(layout.RequiredCellCount == 8);
        Require(layout.CompleteCellCount == 8);
        Require(layout.Parameters.Count == 8);
    }),
    ("Fresh 3 Tx ring accepts R7 and R8 with inactive zero reactance", () =>
    {
        var cells = CreateRingCells(
            resistanceCount: 8,
            reactanceCount: 6,
            returnedCellCount: 10);
        var layout = RingParameterLayout.BuildParameterSet(cells, 6);

        Require(layout.IsComplete);
        Require(layout.RequiredCellCount == 8);
        Require(layout.CompleteCellCount == 8);
        Require(layout.Parameters.Count == 8);
        Require(NearlyEqual(layout.Parameters[6].Reactance, 0f));
        Require(NearlyEqual(layout.Parameters[7].Reactance, 0f));
    }),
    ("Fresh 5 Tx ring requires ten reactances but only eight resistances", () =>
    {
        var cells = CreateRingCells(
            resistanceCount: 8,
            reactanceCount: 10,
            returnedCellCount: 10);
        var layout = RingParameterLayout.BuildParameterSet(cells, 10);

        Require(layout.IsComplete);
        Require(layout.RequiredCellCount == 10);
        Require(layout.CompleteCellCount == 10);
        Require(layout.Parameters.Count == 10);
    }),
    ("Empty 4 Tx ring remains incomplete", () =>
    {
        var cells = CreateRingCells(
            resistanceCount: 0,
            reactanceCount: 0,
            returnedCellCount: 10);
        var layout = RingParameterLayout.BuildParameterSet(cells, 8);

        Require(!layout.IsComplete);
        Require(layout.CompleteCellCount == 0);
        Require(layout.InvalidResistanceIndexes.SequenceEqual(Enumerable.Range(0, 8)));
        Require(layout.InvalidReactanceIndexes.SequenceEqual(Enumerable.Range(0, 8)));
    }),
    ("Memory writes use bounded retries", () =>
    {
        Require(RingProtocolConstants.MemoryCommandMaxAttempts == 3);
        Require(RingProtocolConstants.RetryDelayMs > 0);
    }),
    ("5 Tx express scan keeps R1 and R2 point-by-point", () =>
    {
        Require(CollectLogicalCapacitances(true, 1, 10).SequenceEqual(Enumerable.Range(1, 10)));
        Require(CollectLogicalCapacitances(true, 2, 10).SequenceEqual(Enumerable.Range(1, 10)));
        Require(RingMeasurementSequence.GetActiveChannels(true, 1, 6, 10).SequenceEqual(new[] { 5 }));
    }),
    ("5 Tx express scan groups only R3 through R8", () =>
    {
        Require(CollectLogicalCapacitances(true, 3, 10).SequenceEqual(new[] { 1, 6 }));
        Require(RingMeasurementSequence.GetActiveChannels(true, 3, 1, 10).SequenceEqual(new[] { 0, 1, 2, 3, 4 }));
        Require(RingMeasurementSequence.GetActiveChannels(true, 3, 6, 10).SequenceEqual(new[] { 5, 6, 7, 8, 9 }));
    }),
    ("3 Tx scan maps its high-frequency group to physical CAP 6 through 8", () =>
    {
        Require(CollectLogicalCapacitances(true, 1, 6).SequenceEqual(Enumerable.Range(1, 6)));
        Require(CollectLogicalCapacitances(true, 3, 6).SequenceEqual(new[] { 1, 4 }));
        Require(RingMeasurementSequence.GetActiveChannels(true, 3, 4, 6).SequenceEqual(new[] { 3, 4, 5 }));
        Require(RingMeasurementSequence.GetCommandCapacitance(6, 3) == 3);
        Require(RingMeasurementSequence.GetCommandCapacitance(6, 4) == 6);
        Require(RingMeasurementSequence.GetCommandCapacitance(6, 6) == 8);
    }),
    ("4 Tx scan maps its high-frequency group to physical CAP 6 through 9", () =>
    {
        Require(CollectLogicalCapacitances(true, 1, 8).SequenceEqual(Enumerable.Range(1, 8)));
        Require(CollectLogicalCapacitances(true, 3, 8).SequenceEqual(new[] { 1, 5 }));
        Require(RingMeasurementSequence.GetActiveChannels(true, 3, 5, 8).SequenceEqual(new[] { 4, 5, 6, 7 }));
        Require(RingMeasurementSequence.GetCommandCapacitance(8, 4) == 4);
        Require(RingMeasurementSequence.GetCommandCapacitance(8, 5) == 6);
        Require(RingMeasurementSequence.GetCommandCapacitance(8, 8) == 9);
    }),
    ("5 Tx CAP commands remain unchanged", () =>
    {
        Require(Enumerable.Range(0, 11).All(capacitanceIndex =>
            RingMeasurementSequence.GetCommandCapacitance(10, capacitanceIndex) == capacitanceIndex));
    }),
    ("Disabled express mode always scans channels point-by-point", () =>
    {
        Require(CollectLogicalCapacitances(false, 8, 10).SequenceEqual(Enumerable.Range(1, 10)));
        Require(RingMeasurementSequence.GetActiveChannels(false, 8, 6, 10).SequenceEqual(new[] { 5 }));
    }),
    ("Firmware monitor detects a retained physical response", () =>
    {
        var monitor = new RingFirmwarePointMonitor();
        monitor.Observe(1, 1000, 50, 1000, false);
        monitor.Observe(2, 2000, 1050, 1000, false);
        monitor.Observe(3, 3000, 980, 1000, true);
        monitor.Observe(4, 4000, 940, 1000, true);
        var summary = monitor.BuildSummary();

        Require(summary.State == RingFirmwareRetentionState.Held);
        Require(summary.PeakFrame == 2);
        Require(summary.RetentionRatio > 0.5);
    }),
    ("Firmware monitor detects an acknowledged point that drops before averaging", () =>
    {
        var monitor = new RingFirmwarePointMonitor();
        monitor.Observe(1, 1000, 20, 1000, false);
        monitor.Observe(2, 2000, 1200, 1000, false);
        monitor.Observe(3, 3000, 300, 1000, false);
        monitor.Observe(4, 4000, 25, 1000, true);
        monitor.Observe(5, 5000, -15, 1000, true);
        var summary = monitor.BuildSummary();

        Require(summary.State == RingFirmwareRetentionState.Dropped);
        Require(summary.FirstResponseFrame == 2);
        Require(summary.RetentionRatio < 0.1);
    }),
    ("Firmware monitor distinguishes no physical response from a dropped response", () =>
    {
        var monitor = new RingFirmwarePointMonitor();
        monitor.Observe(1, 1000, 20, 1000, false);
        monitor.Observe(2, 2000, -30, 1000, true);
        monitor.Observe(3, 3000, 15, 1000, true);
        var summary = monitor.BuildSummary();

        Require(summary.State == RingFirmwareRetentionState.NoResponse);
        Require(summary.FirstResponseFrame == 0);
    }),
    ("Firmware monitor classifies the observed R1 C1 hardware trace as dropped", () =>
    {
        var monitor = new RingFirmwarePointMonitor();
        var measuredPhasesMg = new[]
        {
            -7.691, 38270.855, 733.620, 17.773, 12.147,
            10.382, -1.059, -25.661, -2.097, 9.132
        };
        for (var index = 0; index < measuredPhasesMg.Length; index++)
        {
            monitor.Observe(
                index + 1,
                (index + 1) * 1070.0,
                measuredPhasesMg[index],
                53578.801,
                index + 1 > 5);
        }

        var summary = monitor.BuildSummary();
        Require(summary.State == RingFirmwareRetentionState.Dropped);
        Require(summary.PeakFrame == 2);
        Require(summary.FirstResponseFrame == 2);
        Require(summary.RetentionRatio < 0.001);
    })
};

var failures = new List<string>();
foreach (var check in checks)
{
    try
    {
        check.Run();
        Console.WriteLine($"PASS: {check.Name}");
    }
    catch (Exception ex)
    {
        failures.Add($"{check.Name}: {ex.Message}");
        Console.WriteLine($"FAIL: {check.Name}: {ex.Message}");
    }
}

Console.WriteLine($"Protocol checks: {checks.Count - failures.Count}/{checks.Count} passed.");
return failures.Count == 0 ? 0 : 1;

static bool NearlyEqual(float left, float right) => MathF.Abs(left - right) < 0.000001f;

static IReadOnlyList<RingLoopParameterCell> CreateRingCells(
    int resistanceCount,
    int reactanceCount,
    int returnedCellCount)
{
    return Enumerable.Range(0, returnedCellCount)
        .Select(index => new RingLoopParameterCell(
            index,
            index < resistanceCount ? index + 1f : 0f,
            index < reactanceCount ? index + 0.5f : 0f,
            true))
        .ToArray();
}

static IReadOnlyList<int> CollectLogicalCapacitances(bool expressMode, int resistanceIndex, int channelCount)
{
    var result = new List<int>();
    for (var capacitanceIndex = 1;
         capacitanceIndex <= channelCount;
         capacitanceIndex = RingMeasurementSequence.GetNextLogicalCapacitance(
             expressMode,
             resistanceIndex,
             capacitanceIndex,
             channelCount))
    {
        result.Add(capacitanceIndex);
    }

    return result;
}

static void Require(bool condition)
{
    if (!condition)
    {
        throw new InvalidOperationException("assertion failed");
    }
}
