using System.IO.Compression;
using System.Runtime.InteropServices;
using System.Xml.Linq;
using LOOP.Next.Reporting;
using LoopNext.Core.Models;
using LoopNext.Core.Protocol;
using LoopNext.Hardware;
using LOOP_Next_App.Services;

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
    }),
    ("Сигнатуры старой прошивки разбираются как прежде и дают кадр 240 байт", () =>
    {
        // Значения из существующих проверок и реальные сигнатуры приборов в
        // эксплуатации: ни одна не должна уйти в ветку новой прошивки.
        foreach (var signature in new uint[] { 140000, 150000, 230000, 240000, 249000, 250000, 349000, 359000 })
        {
            var identity = ToolIdentity.Decode(signature);
            Require(identity.FrameSize == ToolIdentity.LegacyFrameSize, $"frame size for {signature}");
            Require(!identity.IsExtendedFirmware, $"firmware generation for {signature}");
        }

        // Сигнатура с мусором в старших битах: полезны только младшие 20 бит.
        var legacyWithNoise = ToolIdentity.Decode(0x3143ADD1);
        Require(legacyWithNoise.FrameSize == ToolIdentity.LegacyFrameSize, "noisy signature frame size");
        Require(legacyWithNoise.DeviceType == ToolIdentity.DeviceTypeLwd, "noisy signature device type");
        Require(legacyWithNoise.TransmitterCount == 4, "noisy signature transmitter count");
        Require(legacyWithNoise.Number == 105, "noisy signature number");
    }),
    ("Сигнатура новой прошивки даёт кадр 320 байт и прежние поля прибора", () =>
    {
        var identity = ToolIdentity.Decode(320151501);
        Require(identity.FrameSize == ToolIdentity.ExtendedFrameSize, "frame size");
        Require(identity.IsExtendedFirmware, "firmware generation");
        Require(identity.DeviceType == ToolIdentity.DeviceTypeAutonom, "device type");
        Require(identity.TransmitterCount == 5, "transmitter count");
        Require(identity.Modification == 1, "modification");
        Require(identity.Number == 501, "number");
        Require(identity.IsValid, "validity");
        Require(identity.Low20 / 1000u == 151, "tool code for palette");

        // Картограф в режиме LWD 4tx после перепрошивки остаётся той же веткой.
        var cartograph = ToolIdentity.Decode(320349102);
        Require(cartograph.FrameSize == ToolIdentity.ExtendedFrameSize, "cartograph frame size");
        Require(cartograph.IsCartographLwd4TxMode, "cartograph mode");
        Require(cartograph.TransmitterCount == 4, "cartograph transmitter count");
    }),
    ("Длина нагрузки ответа считается по размеру кадра", () =>
    {
        Require(
            ToolProtocolConstants.GetToolPayloadLength(ToolIdentity.LegacyFrameSize) ==
            ToolProtocolConstants.ToolPayloadLength,
            "legacy payload length");
        Require(
            ToolProtocolConstants.GetToolPayloadLength(ToolIdentity.ExtendedFrameSize) ==
            ToolProtocolConstants.ExtendedToolPayloadLength,
            "extended payload length");
        Require(ToolProtocolConstants.ExtendedToolPayloadLength == 322, "extended payload is 322 bytes");

        // Метрология в обоих поколениях занимает 240 байт.
        Require(ToolProtocolConstants.ToolMetrologySize == 240, "metrology size");
    }),
    ("Амплитудные поля новой прошивки читаются по своим смещениям", () =>
    {
        Require(ExtendedFrameFields.RhoAttSmtOffset == 240, "rho_att_smt offset");
        Require(ExtendedFrameFields.AttDbSmtOffset == 280, "att_dB_smt offset");
        Require(ExtendedFrameFields.RequiredFrameSize == ToolIdentity.ExtendedFrameSize, "required frame size");

        // Кадр заполняется так, чтобы каждое значение было опознаваемым:
        // rho_att = 100*freq + tx, att_dB = -(freq + 1) * (tx + 1).
        var frame = new byte[ToolIdentity.ExtendedFrameSize];
        for (var freq = 0; freq < 2; freq++)
        {
            for (var tx = 0; tx < 5; tx++)
            {
                var index = (freq * 5 + tx) * sizeof(float);
                BitConverter.TryWriteBytes(
                    frame.AsSpan(ExtendedFrameFields.RhoAttSmtOffset + index),
                    100f * freq + tx);
                BitConverter.TryWriteBytes(
                    frame.AsSpan(ExtendedFrameFields.AttDbSmtOffset + index),
                    -(freq + 1f) * (tx + 1f));
            }
        }

        for (var freq = 0; freq < 2; freq++)
        {
            for (var tx = 0; tx < 5; tx++)
            {
                Require(
                    ExtendedFrameFields.TryReadRhoAttSmt(frame, freq, tx, out var rho)
                    && Math.Abs(rho - (100f * freq + tx)) < 1e-6f,
                    $"rho_att F{freq}T{tx}");
                Require(
                    ExtendedFrameFields.TryReadAttDbSmt(frame, freq, tx, out var att)
                    && Math.Abs(att + (freq + 1f) * (tx + 1f)) < 1e-6f,
                    $"att_dB F{freq}T{tx}");
            }
        }

        // Кадр старой прошивки этих полей не содержит — чтение обязано отказать,
        // а не вернуть содержимое чужой памяти.
        var legacyFrame = new byte[ToolIdentity.LegacyFrameSize];
        Require(!ExtendedFrameFields.TryReadRhoAttSmt(legacyFrame, 0, 0, out _), "legacy rho_att");
        Require(!ExtendedFrameFields.TryReadAttDbSmt(legacyFrame, 0, 0, out _), "legacy att_dB");
        Require(!ExtendedFrameFields.TryReadRhoAttSmt(frame, 0, 5, out _), "transmitter out of range");
        Require(!ExtendedFrameFields.TryReadRhoAttSmt(frame, 2, 0, out _), "frequency out of range");
    }),
    ("Дамп обмена печатает адреса смещений", () =>
    {
        var bytes = new byte[] { 0xCE, 0x1F, 0x15, 0x13 };
        var dump = ToolSerialTransport.FormatHexDump("RX", bytes);
        Require(dump.Contains("RX (4 B):", StringComparison.Ordinal), "заголовок с размером");
        Require(dump.Contains("0000: CE 1F 15 13", StringComparison.Ordinal), "строка со смещением");
        Require(ToolSerialTransport.FormatHexDump("RX", []).Contains("(0 B)", StringComparison.Ordinal), "пустой буфер");
    }),
    ("Кадр новой прошивки разбирается по общей части", () =>
    {
        // Первые 240 байт кадра новой прошивки совпадают со старым, поэтому
        // разбор со смещения служебных байт даёт те же значения, а хвост с
        // амплитудными величинами этому приложению не нужен.
        var frame = default(GpToolData);
        frame.Signature = 320151501;
        frame.Temperature = 42.5f;
        frame.SetDeltaPhase(0, 2, 0.125f);
        frame.SetDeltaPhase(1, 4, -0.25f);

        var payload = new byte[ToolProtocolConstants.ExtendedToolPayloadLength];
        MemoryMarshal.Write(payload.AsSpan(ToolProtocolConstants.ToolServicePrefixLength), in frame);
        for (var i = ToolProtocolConstants.ToolServicePrefixLength + ToolProtocolConstants.ToolDataSize;
             i < payload.Length;
             i++)
        {
            payload[i] = 0xAB;
        }

        var parsed = MemoryMarshal.Read<GpToolData>(
            payload.AsSpan(ToolProtocolConstants.ToolServicePrefixLength));
        Require(parsed.Signature == frame.Signature, "signature survived");
        Require(Math.Abs(parsed.Temperature - 42.5f) < 1e-6f, "temperature survived");
        Require(Math.Abs(parsed.GetDeltaPhase(0, 2) - 0.125f) < 1e-6f, "delta phase F0T2 survived");
        Require(Math.Abs(parsed.GetDeltaPhase(1, 4) + 0.25f) < 1e-6f, "delta phase F1T4 survived");
    }),
    ("Новая структура занимает 320 байт, а старая остаётся 240-байтной", () =>
    {
        Require(Marshal.SizeOf<GpToolData>() == ToolIdentity.LegacyFrameSize, "legacy structure size");
        Require(Marshal.SizeOf<ExtendedGpToolData>() == ToolIdentity.ExtendedFrameSize, "extended structure size");

        var frame = default(ExtendedGpToolData);
        frame.Common.Signature = 320151502;
        frame.Common.SetDeltaPhase(1, 4, -0.125f);
        frame.SetRhoAttSmt(0, 3, 321.5f);
        frame.SetAttDbSmt(1, 4, -7.25f);

        var bytes = new byte[ToolIdentity.ExtendedFrameSize];
        MemoryMarshal.Write(bytes, in frame);
        Require(BitConverter.ToUInt32(bytes, 0) == 320151502, "common data starts at offset 0");
        Require(
            Math.Abs(BitConverter.ToSingle(bytes, ExtendedFrameFields.RhoAttSmtOffset + 3 * sizeof(float)) - 321.5f) < 1e-6f,
            "rho_att_smt starts at offset 240");
        Require(
            Math.Abs(BitConverter.ToSingle(bytes, ExtendedFrameFields.AttDbSmtOffset + 9 * sizeof(float)) + 7.25f) < 1e-6f,
            "att_dB_smt starts at offset 280");

        var parsed = MemoryMarshal.Read<ExtendedGpToolData>(bytes);
        Require(Math.Abs(parsed.Common.GetDeltaPhase(1, 4) + 0.125f) < 1e-6f, "common delta phase survived");
        Require(Math.Abs(parsed.GetRhoAttSmt(0, 3) - 321.5f) < 1e-6f, "rho_att survived");
        Require(Math.Abs(parsed.GetAttDbSmt(1, 4) + 7.25f) < 1e-6f, "att_dB survived");
    }),
    ("Запрос новой прошивки использует length-framed формат", () =>
    {
        var request = ToolSerialTransport.BuildFramedPacket(
            ToolProtocolConstants.CmdGetData,
            ReadOnlySpan<byte>.Empty);

        Require(request.SequenceEqual(Convert.FromHexString("1105002295")), "firmware request bytes");
        Require(
            ToolSerialTransport.ParseFramedPacket(request, out var command, out var payload) == ToolResponseCode.Ok,
            "request parses");
        Require(command == ToolProtocolConstants.CmdGetData, "request command");
        Require(payload.Length == 0, "request payload");
    }),
    ("320-байтный ответ читается по длине пакета и своей сигнатуре", () =>
    {
        var frame = default(ExtendedGpToolData);
        frame.Common.Signature = 320151502;
        frame.Common.Frame = 77;
        frame.Common.Temperature = 36.75f;
        frame.Common.SetDeltaPhase(0, 0, 0.25f);
        frame.SetRhoAttSmt(1, 4, 987.5f);

        var rawFrame = new byte[ToolIdentity.ExtendedFrameSize];
        MemoryMarshal.Write(rawFrame, in frame);
        var packet = ToolSerialTransport.BuildFramedPacket(ToolProtocolConstants.CmdGetData, rawFrame);

        Require(packet.Length == ToolIdentity.ExtendedFrameSize + 5, "wire packet size");
        Require(packet[1] == 0x45 && packet[2] == 0x01, "declared length is 325");
        Require(
            ToolSerialTransport.ParseFramedPacket(packet, out var command, out var payload) == ToolResponseCode.Ok,
            "response parses");
        Require(command == ToolProtocolConstants.CmdGetData, "response command");
        Require(payload.Length == ToolIdentity.ExtendedFrameSize, "response payload size");

        var parsed = MemoryMarshal.Read<ExtendedGpToolData>(payload);
        var identity = ToolIdentity.Decode(parsed.Common.Signature);
        Require(identity.IsValid && identity.IsExtendedFirmware, "embedded extended signature");
        Require(identity.FrameSize == payload.Length, "signature confirms payload size");
        Require(parsed.Common.Frame == 77, "frame number");
        Require(Math.Abs(parsed.Common.Temperature - 36.75f) < 1e-6f, "temperature");
        Require(Math.Abs(parsed.GetRhoAttSmt(1, 4) - 987.5f) < 1e-6f, "extended tail");
    }),
    ("Новая ветка отвергает неверную длину и CRC пакета", () =>
    {
        var valid = ToolSerialTransport.BuildFramedPacket(
            ToolProtocolConstants.CmdGetData,
            new byte[ToolIdentity.ExtendedFrameSize]);

        var invalidLength = valid.ToArray();
        invalidLength[1]--;
        Require(
            ToolSerialTransport.ParseFramedPacket(invalidLength, out _, out _) == ToolResponseCode.InvalidResponse,
            "invalid declared length");

        var invalidCrc = valid.ToArray();
        invalidCrc[^1] ^= 0x80;
        Require(
            ToolSerialTransport.ParseFramedPacket(invalidCrc, out _, out _) == ToolResponseCode.CrcError,
            "invalid CRC");
    }),
    ("Масштаб окна предлагает фиксированные относительные значения с дефолтом 100%", () =>
    {
        Require(WindowScaleService.DefaultScalePercent == 100, "default scale");
        Require(
            WindowScaleService.AvailableScalePercents.SequenceEqual(new[] { 80, 90, 100 }),
            "available scales");
        Require(WindowScaleService.NormalizeScalePercent(84) == 80, "normalize lower value");
        Require(WindowScaleService.NormalizeScalePercent(105) == 100, "normalize tie toward default");
        Require(WindowScaleService.NormalizeScalePercent(116) == 100, "normalize upper value");
        Require(WindowScaleService.MaximumScalePercent == 100, "no option may exceed the fitted width");
    }),
    ("Старая схема масштаба один раз сбрасывается на 100%, новая сохраняет выбор", () =>
    {
        Require(WindowScaleService.ResolveSavedScalePercent(null, 80) == 100, "1.1.3 migration");
        Require(WindowScaleService.ResolveSavedScalePercent(null, null) == 100, "fresh install");
        Require(
            WindowScaleService.ResolveSavedScalePercent(
                WindowScaleService.CurrentSettingsSchemaVersion,
                90) == 90,
            "current setting");
        Require(
            WindowScaleService.ResolveSavedScalePercent(
                WindowScaleService.CurrentSettingsSchemaVersion,
                87) == 90,
            "current setting normalization");

        // Значения выше 100% больше не предлагаются: сохранённый ранее выбор
        // подтягивается к 100%, иначе пользователь остался бы с обрезанной таблицей.
        Require(
            WindowScaleService.ResolveSavedScalePercent(
                WindowScaleService.CurrentSettingsSchemaVersion,
                110) == 100,
            "saved 110% falls back to 100%");
        Require(
            WindowScaleService.ResolveSavedScalePercent(
                WindowScaleService.CurrentSettingsSchemaVersion,
                120) == 100,
            "saved 120% falls back to 100%");
    }),
    ("Эталонный компьютер 1920x1080 при Windows 125% получает масштаб ровно 1.0", () =>
    {
        var host = HostEffectiveSize(1920, 1080, 1.25);
        Require(Math.Abs(host.Width - WindowScaleService.ReferenceContentWidth) < 0.5, "reference host width");
        Require(
            Math.Abs(WindowScaleService.GetEffectiveScaleFactor(100, host.Width, host.Height) - 1.0) < 0.000001,
            "reference render scale");
    }),
    ("Один экран даёт один физический размер при Windows от 100% до 200%", () =>
    {
        // Размеры рабочей области приходят в эффективных пикселях, поэтому
        // вписывание в окно само компенсирует системный DPI: на одном и том же
        // экране 1920x1080 физический размер элементов не зависит от масштаба Windows.
        foreach (var rasterizationScale in new[] { 1.0, 1.25, 1.5, 1.75, 2.0 })
        {
            var host = HostEffectiveSize(1920, 1080, rasterizationScale);
            var renderScale = WindowScaleService.GetEffectiveScaleFactor(100, host.Width, host.Height);
            Require(
                Math.Abs(renderScale * rasterizationScale - WindowScaleService.ReferenceRasterizationScale) < 0.000001,
                $"physical size at Windows {rasterizationScale * 100:F0}%");
        }
    }),
    ("Содержимое вписывается и в маленький ноутбук, и в большой монитор", () =>
    {
        var screens = new[]
        {
            (Width: 1366, Height: 768, Scale: 1.0),
            (Width: 1280, Height: 800, Scale: 1.0),
            (Width: 1440, Height: 900, Scale: 1.0),
            (Width: 1920, Height: 1080, Scale: 1.0),
            (Width: 1920, Height: 1080, Scale: 1.25),
            (Width: 1920, Height: 1200, Scale: 1.0),
            (Width: 2560, Height: 1440, Scale: 1.0),
            (Width: 3840, Height: 2160, Scale: 1.5),
            (Width: 3840, Height: 2160, Scale: 2.0)
        };

        foreach (var screen in screens)
        {
            var host = HostEffectiveSize(screen.Width, screen.Height, screen.Scale);
            var renderScale = WindowScaleService.GetEffectiveScaleFactor(100, host.Width, host.Height);

            // Ничего не обрезается: масштабированный эталонный кадр помещается в окно.
            Require(
                WindowScaleService.ReferenceContentWidth * renderScale <= host.Width + 0.5,
                $"width fits on {screen.Width}x{screen.Height} @ {screen.Scale * 100:F0}%");
            Require(
                WindowScaleService.ReferenceContentHeight * renderScale <= host.Height + 0.5,
                $"height fits on {screen.Width}x{screen.Height} @ {screen.Scale * 100:F0}%");

            // Окно заполняется хотя бы по одной стороне: пустого поля как на
            // непропорционально мелком виде не остаётся.
            var fillsWidth = WindowScaleService.ReferenceContentWidth * renderScale >= host.Width - 0.5;
            var fillsHeight = WindowScaleService.ReferenceContentHeight * renderScale >= host.Height - 0.5;
            Require(
                fillsWidth || fillsHeight,
                $"fills window on {screen.Width}x{screen.Height} @ {screen.Scale * 100:F0}%");
        }
    }),
    ("Пользовательские 80–100% применяются поверх вписывания в окно", () =>
    {
        var host = HostEffectiveSize(1920, 1080, 1.0);
        var autoFit = WindowScaleService.GetAutoFitFactor(host.Width, host.Height);
        Require(Math.Abs(autoFit - 1.25) < 0.000001, "auto fit at Windows 100%");

        foreach (var percent in WindowScaleService.AvailableScalePercents)
        {
            Require(
                Math.Abs(
                    WindowScaleService.GetEffectiveScaleFactor(percent, host.Width, host.Height)
                    - autoFit * percent / 100.0) < 0.000001,
                $"user percent {percent}");
        }
    }),
    ("Подсказки масштабируются размером шрифта вслед за содержимым", () =>
    {
        Require(
            Math.Abs(WindowScaleService.GetToolTipFontSize(1.0) - WindowScaleService.ToolTipBaseFontSize) < 0.000001,
            "reference machine keeps the system size");
        Require(
            Math.Abs(WindowScaleService.GetToolTipFontSize(1.25) - 15.0) < 0.000001,
            "Windows 100% enlarges the tooltip with the content");
        Require(
            Math.Abs(WindowScaleService.GetToolTipFontSize(0.8) - 9.6) < 0.000001,
            "user 80% shrinks the tooltip with the content");

        // Вырожденный коэффициент не должен схлопывать подсказку в ноль.
        Require(
            Math.Abs(WindowScaleService.GetToolTipFontSize(0) - WindowScaleService.ToolTipBaseFontSize) < 0.000001,
            "zero factor");
        Require(
            Math.Abs(WindowScaleService.GetToolTipFontSize(double.NaN) - WindowScaleService.ToolTipBaseFontSize) < 0.000001,
            "NaN factor");
    }),
    ("Вырожденные размеры окна не ломают масштаб", () =>
    {
        Require(Math.Abs(WindowScaleService.GetAutoFitFactor(0, 0) - 1.0) < 0.000001, "zero size");
        Require(Math.Abs(WindowScaleService.GetAutoFitFactor(-10, 100) - 1.0) < 0.000001, "negative size");
        Require(Math.Abs(WindowScaleService.GetAutoFitFactor(double.NaN, 100) - 1.0) < 0.000001, "NaN size");

        Require(
            Math.Abs(WindowScaleService.GetAutoFitFactor(1, 1) - WindowScaleService.MinimumAutoFitFactor) < 0.000001,
            "clamped below");
        Require(
            Math.Abs(WindowScaleService.GetAutoFitFactor(100000, 100000) - WindowScaleService.MaximumAutoFitFactor) < 0.000001,
            "clamped above");
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

// Размер узла масштабирования содержимого в эффективных пикселях для
// развёрнутого окна на экране заданного разрешения и системного масштаба.
// Из физической высоты вычитается панель задач, из эффективной - строка
// пользовательского заголовка окна.
static (double Width, double Height) HostEffectiveSize(
    int physicalWidth,
    int physicalHeight,
    double rasterizationScale)
{
    const double TaskbarEffectiveHeight = 40.0;
    const double TitleBarEffectiveHeight = 48.0;

    return (
        physicalWidth / rasterizationScale,
        physicalHeight / rasterizationScale - TaskbarEffectiveHeight - TitleBarEffectiveHeight);
}

static void Require(bool condition, string message = "assertion failed")
{
    if (!condition)
    {
        throw new InvalidOperationException(message);
    }
}
