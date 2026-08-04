using System.IO.Compression;
using System.Xml.Linq;
using LOOP.Next.Reporting;
using LoopNext.Core.Models;
using LoopNext.Hardware;

var checks = new List<(string Name, Action Run)>
{
    ("SGNREAD parses uint32", () =>
    {
        Require(RingProtocolParser.TryParseSignature("SGN=4294967295\r\n", out var signature));
        Require(signature == uint.MaxValue);
    }),
    ("SGREAD compatibility response parses uint32", () =>
    {
        Require(RingProtocolParser.TryParseSignature("SG=151\r\n", out var signature));
        Require(signature == RingSignatureCatalog.UniversalAutonomousSignature);
        Require(RingProtocolConstants.ReadCompatibleSignatureCommand == "SGREAD\r\n");
        Require(RingProtocolConstants.WriteCompatibleSignatureCommandFormat == "SGWRITE {0}\r\n");
    }),
    ("Signature 151 exposes the documented universal compatibility", () =>
    {
        Require(
            RingSignatureCatalog.GetSupportedToolTypes(151) ==
            "Автономный 4-5 TX, LWD 3-4 TX, Картограф->LWD-4TX");
        Require(RingSignatureCatalog.TryParseEditableSignature("151", out var signature, out var error));
        Require(signature == 151 && error is null);
    }),
    ("Signature 151 selects the universal five-transmitter capacitance profile", () =>
    {
        Require(
            RingSignatureCatalog.GetCapacitanceMappingProfile(151) ==
            RingCapacitanceMappingProfile.UniversalFiveTransmitter);
        Require(
            RingSignatureCatalog.GetCapacitanceMappingProfile(123) ==
            RingCapacitanceMappingProfile.CompatibilityFallback);
        Require(
            RingMeasurementSequence.GetCommandCapacitance(
                8,
                5,
                RingCapacitanceMappingProfile.UniversalFiveTransmitter) == 6);
    }),
    ("Signature 151 accepts only its documented tool families", () =>
    {
        var autonomous4 = ToolIdentity.Decode(140000);
        var autonomous5 = ToolIdentity.Decode(150000);
        var lwd3 = ToolIdentity.Decode(230000);
        var lwd4 = ToolIdentity.Decode(240000);
        var cartographLwd4 = ToolIdentity.Decode(349000);
        var autonomous3 = ToolIdentity.Decode(130000);
        var lwd5 = ToolIdentity.Decode(250000);

        Require(RingSignatureCatalog.GetToolCompatibility(151, in autonomous4) == RingToolCompatibility.Supported);
        Require(RingSignatureCatalog.GetToolCompatibility(151, in autonomous5) == RingToolCompatibility.Supported);
        Require(RingSignatureCatalog.GetToolCompatibility(151, in lwd3) == RingToolCompatibility.Supported);
        Require(RingSignatureCatalog.GetToolCompatibility(151, in lwd4) == RingToolCompatibility.Supported);
        Require(RingSignatureCatalog.GetToolCompatibility(151, in cartographLwd4) == RingToolCompatibility.Supported);
        Require(RingSignatureCatalog.GetToolCompatibility(151, in autonomous3) == RingToolCompatibility.Unsupported);
        Require(RingSignatureCatalog.GetToolCompatibility(151, in lwd5) == RingToolCompatibility.Unsupported);
        Require(RingSignatureCatalog.GetToolCompatibility(123, in lwd5) == RingToolCompatibility.Unverified);
    }),
    ("Signature 241 exposes the LWD four-transmitter compatibility", () =>
    {
        Require(RingSignatureCatalog.LwdFourTransmitterSignature == 241);
        Require(
            RingSignatureCatalog.GetSupportedToolTypes(241) ==
            "LWD 4 TX, Картограф->LWD-4TX");
        Require(RingSignatureCatalog.IsKnownSignature(241));
        Require(!RingSignatureCatalog.IsKnownSignature(242));
        Require(RingSignatureCatalog.TryParseEditableSignature("241", out var signature, out var error));
        Require(signature == 241 && error is null);
    }),
    ("Signature 241 keeps the universal five-transmitter capacitance grid", () =>
    {
        Require(
            RingSignatureCatalog.GetCapacitanceMappingProfile(241) ==
            RingCapacitanceMappingProfile.UniversalFiveTransmitter);
        // Прибор LWD 4 Tx: логические C5..C8 уходят в физический банк C6..C9.
        Require(
            RingMeasurementSequence.GetCommandCapacitance(
                8,
                5,
                RingSignatureCatalog.GetCapacitanceMappingProfile(241)) == 6);
        Require(
            RingMeasurementSequence.GetCommandCapacitance(
                8,
                8,
                RingSignatureCatalog.GetCapacitanceMappingProfile(241)) == 9);
    }),
    ("Signature 241 accepts only four-transmitter LWD family tools", () =>
    {
        var lwd4 = ToolIdentity.Decode(240000);
        var lwd4Modified = ToolIdentity.Decode(243000);
        var cartographLwd4 = ToolIdentity.Decode(349000);
        var cartographLwd4Alt = ToolIdentity.Decode(359000);
        var lwd3 = ToolIdentity.Decode(230000);
        var lwd5 = ToolIdentity.Decode(250000);
        var autonomous4 = ToolIdentity.Decode(140000);
        var autonomous5 = ToolIdentity.Decode(150000);

        // Поддерживаются: LWD 24n и картограф 34n в режиме LWD 4tx.
        Require(RingSignatureCatalog.GetToolCompatibility(241, in lwd4) == RingToolCompatibility.Supported);
        Require(RingSignatureCatalog.GetToolCompatibility(241, in lwd4Modified) == RingToolCompatibility.Supported);
        Require(RingSignatureCatalog.GetToolCompatibility(241, in cartographLwd4) == RingToolCompatibility.Supported);
        Require(RingSignatureCatalog.GetToolCompatibility(241, in cartographLwd4Alt) == RingToolCompatibility.Supported);

        // Не поддерживаются: другое число передатчиков и всё семейство Автономный.
        Require(RingSignatureCatalog.GetToolCompatibility(241, in lwd3) == RingToolCompatibility.Unsupported);
        Require(RingSignatureCatalog.GetToolCompatibility(241, in lwd5) == RingToolCompatibility.Unsupported);
        Require(RingSignatureCatalog.GetToolCompatibility(241, in autonomous4) == RingToolCompatibility.Unsupported);
        Require(RingSignatureCatalog.GetToolCompatibility(241, in autonomous5) == RingToolCompatibility.Unsupported);
    }),
    ("Editable signature rejects values outside the three-digit schema", () =>
    {
        Require(!RingSignatureCatalog.TryParseEditableSignature("15", out _, out var shortError));
        Require(!string.IsNullOrWhiteSpace(shortError));
        Require(!RingSignatureCatalog.TryParseEditableSignature("1234", out _, out var longError));
        Require(!string.IsNullOrWhiteSpace(longError));
        Require(!RingSignatureCatalog.TryParseEditableSignature("1A1", out _, out var letterError));
        Require(!string.IsNullOrWhiteSpace(letterError));
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
        Require(RingProtocolConstants.EraseLoopParametersCommand == "ZERASE\r\n");
        Require(RingProtocolParser.IsWriteLoopParameterReply(9, "WRITE Z9 OK!"));
        Require(RingProtocolParser.IsEraseReply("ERASE OK!"));
        Require(RingProtocolParser.IsWriteSignatureReply(42, "WRITE SGN=42 OK!"));
    }),
    ("Firmware profiles retain 10 working cells and expose 32 extended cells", () =>
    {
        Require(RingProtocolConstants.WorkingParameterCellCount == 10);
        Require(RingDeviceInfo.Legacy.ImpedanceCellCount == 10);
        Require(RingDeviceInfo.Extended(42).ImpedanceCellCount == 32);
        Require(RingDeviceInfo.Extended(151, RingSignatureCommandSet.Sg).SignatureCommandSet == RingSignatureCommandSet.Sg);
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
    }),
    ("Excel report is a complete Open XML workbook", () =>
    {
        var workbookBytes = LoopReportExcelBuilder.Build(CreateSampleReport());
        using var stream = new MemoryStream(workbookBytes);
        using var archive = new ZipArchive(stream, ZipArchiveMode.Read);

        Require(workbookBytes.Length > 5_000, $"workbook is unexpectedly small: {workbookBytes.Length} bytes");
        Require(archive.GetEntry("[Content_Types].xml") is not null, "content types entry is missing");
        Require(archive.GetEntry("xl/workbook.xml") is not null, "workbook entry is missing");
        Require(archive.GetEntry("xl/styles.xml") is not null, "styles entry is missing");
        Require(archive.GetEntry("xl/worksheets/sheet1.xml") is not null, "worksheet entry is missing");
    }),
    ("Excel report contains corporate sections, values and formulas", () =>
    {
        var workbookBytes = LoopReportExcelBuilder.Build(CreateSampleReport());
        using var stream = new MemoryStream(workbookBytes);
        using var archive = new ZipArchive(stream, ZipArchiveMode.Read);
        using var reader = new StreamReader(
            archive.GetEntry("xl/worksheets/sheet1.xml")!.Open());
        var worksheetXml = reader.ReadToEnd();
        using var stylesReader = new StreamReader(
            archive.GetEntry("xl/styles.xml")!.Open());
        var stylesXml = stylesReader.ReadToEnd();
        XNamespace spreadsheetNs = "http://schemas.openxmlformats.org/spreadsheetml/2006/main";
        var worksheetDocument = XDocument.Parse(worksheetXml);
        var styleFormats = XDocument.Parse(stylesXml)
            .Root!
            .Element(spreadsheetNs + "cellXfs")!
            .Elements(spreadsheetNs + "xf")
            .ToArray();

        Require(worksheetXml.Contains("ПРОТОКОЛ ПРОВЕРКИ ПРИБОРА", StringComparison.Ordinal), "title is missing");
        Require(!worksheetXml.Contains("ЭМК-90", StringComparison.Ordinal), "obsolete device name is present in title");
        Require(!worksheetXml.Contains("Протокол №", StringComparison.Ordinal), "redundant protocol number is present");
        Require(!worksheetDocument.Descendants(spreadsheetNs + "pane").Any(), "worksheet contains a frozen pane");
        Require(
            worksheetDocument.Descendants(spreadsheetNs + "c")
                .Any(cell => cell.Attribute("r")?.Value == "C9" && cell.Attribute("s")?.Value == "17"),
            "threshold cell style is incorrect");
        Require(
            worksheetDocument.Descendants(spreadsheetNs + "c")
                .Any(cell => cell.Attribute("r")?.Value == "G9" && cell.Attribute("s")?.Value == "5"),
            "averaging cell is not left-aligned");
        Require(
            worksheetDocument.Descendants(spreadsheetNs + "mergeCell")
                .Any(merge => merge.Attribute("ref")?.Value == "A28:C28"),
            "summary labels are not wide enough");
        Require(worksheetXml.Contains("ИТОГОВАЯ ОЦЕНКА", StringComparison.Ordinal), "summary is missing");
        Require(worksheetXml.Contains("Каналов без данных", StringComparison.Ordinal), "missing-data control is missing");
        Require(worksheetXml.Contains("НЕПОЛНЫЕ ДАННЫЕ", StringComparison.Ordinal), "incomplete-data outcome is missing");
        Require(worksheetXml.Contains("R3", StringComparison.Ordinal), "R3 is missing");
        Require(worksheetXml.Contains("R4", StringComparison.Ordinal), "R4 is missing");
        Require(worksheetXml.Contains("R5", StringComparison.Ordinal), "R5 is missing");
        Require(worksheetXml.Contains("COUNTIF", StringComparison.Ordinal), "COUNTIF formula is missing");
        Require(worksheetXml.Contains("MAX(MAX(", StringComparison.Ordinal), "status formula is missing");
        Require(worksheetXml.Contains("$C$9", StringComparison.Ordinal), "threshold reference is incorrect");
        Require(styleFormats[17].Element(spreadsheetNs + "alignment")?.Attribute("horizontal")?.Value == "left", "threshold value is not left-aligned");
        Require(styleFormats[18].Attribute("numFmtId")?.Value == "1", "resistance values are not integer-formatted");
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
if (failures.Count == 0 && args is ["--sample-report", var sampleReportPath])
{
    var sampleReportDirectory = Path.GetDirectoryName(Path.GetFullPath(sampleReportPath));
    if (!string.IsNullOrEmpty(sampleReportDirectory))
    {
        Directory.CreateDirectory(sampleReportDirectory);
    }

    File.WriteAllBytes(sampleReportPath, LoopReportExcelBuilder.Build(CreateSampleReport()));
    Console.WriteLine($"Sample Excel report: {Path.GetFullPath(sampleReportPath)}");
}

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

static LoopReport CreateSampleReport()
{
    var selectedResistanceIndexes = new[] { 3, 4, 5 };
    var channelLabels = new[]
    {
        "T1 401", "T2 401", "T3 401", "T4 401",
        "T1 2000", "T2 2000", "T3 2000", "T4 2000"
    };
    var channels = channelLabels
        .Select((label, index) => new LoopReportChannel(
            label,
            index < 4 ? 401f : 2000f,
            new[]
            {
                SampleCell(index, 3),
                SampleCell(index, 4),
                SampleCell(index, 5)
            }))
        .ToArray();

    return new LoopReport(
        "ООО НПФ \"АМК ГОРИЗОНТ\"",
        "52025",
        "Картограф->LWD-4TX",
        "0x0005534A",
        "4",
        "9",
        "2",
        "R1 -> T1",
        "151",
        6,
        5,
        new DateTime(2026, 7, 23, 11, 9, 0),
        new[] { 66f, 1234f, 20f, 50f, 100f, 148f, 80f, 66f },
        selectedResistanceIndexes,
        channels);
}

static LoopReportCell SampleCell(int channelIndex, int resistanceIndex)
{
    var resistanceOffset = resistanceIndex switch
    {
        3 => 0,
        4 => 1,
        5 => 2,
        _ => throw new ArgumentOutOfRangeException(nameof(resistanceIndex))
    };
    float[,] deviations =
    {
        { -4, -2, 2 },
        { -7, 1, 5 },
        { 2, 13, -5 },
        { 19, -6, -2 },
        { -5, -3, 1 },
        { -8, 0, 4 },
        { 1, 3, -14 },
        { 4, 20, -3 }
    };
    var deviation = deviations[channelIndex, resistanceOffset];
    return new LoopReportCell(deviation, MeasurementQualityClassifier.Classify(deviation, 6));
}

static void Require(bool condition, string message = "assertion failed")
{
    if (!condition)
    {
        throw new InvalidOperationException(message);
    }
}
