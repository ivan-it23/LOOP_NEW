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
    ("4 Tx scan sends logical CAP and leaves physical shift to firmware", () =>
    {
        Require(CollectLogicalCapacitances(true, 1, 8).SequenceEqual(Enumerable.Range(1, 8)));
        Require(CollectLogicalCapacitances(true, 3, 8).SequenceEqual(new[] { 1, 5 }));
        Require(RingMeasurementSequence.GetActiveChannels(true, 3, 5, 8).SequenceEqual(new[] { 4, 5, 6, 7 }));
        Require(RingMeasurementSequence.GetExpectedPhysicalCapacitance(8, 4) == 4);
        Require(RingMeasurementSequence.GetExpectedPhysicalCapacitance(8, 5) == 6);
        Require(RingMeasurementSequence.GetExpectedPhysicalCapacitance(8, 8) == 9);
        Require(RingMeasurementSequence.GetExpectedPhysicalCapacitance(10, 5) == 5);
    }),
    ("Disabled express mode always scans channels point-by-point", () =>
    {
        Require(CollectLogicalCapacitances(false, 8, 10).SequenceEqual(Enumerable.Range(1, 10)));
        Require(RingMeasurementSequence.GetActiveChannels(false, 8, 6, 10).SequenceEqual(new[] { 5 }));
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
