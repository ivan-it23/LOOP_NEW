using System.IO.Compression;
using System.Runtime.InteropServices;
using System.Xml.Linq;
using LOOP.Next.Reporting;
using LoopNext.Core.Models;
using LoopNext.Core.Protocol;
using LoopNext.Hardware;
using LoopNext.Metrology;
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
    ("Кольцо 241 несёт восемь ёмкостей и сдвига не требует", () =>
    {
        // Подтверждено разработчиком кольца и прогоном 22.09: у меры с
        // сигнатурой 241 восемь ёмкостей, банки C1..C4 и C5..C8. Девятой
        // позиции нет - команда CAP 9 квитируется, но реле не переключает,
        // и точка остаётся без отклика.
        var profile = RingSignatureCatalog.GetCapacitanceMappingProfile(241);
        Require(profile == RingCapacitanceMappingProfile.CompactFourTransmitter, "профиль кольца 241");

        for (var logical = 1; logical <= 8; logical++)
        {
            Require(
                RingMeasurementSequence.GetCommandCapacitance(8, logical, profile) == logical,
                $"логическая C{logical} без сдвига");
        }

        // Ячейка реактивности совпадает с каналом один в один.
        for (var channel = 0; channel < 8; channel++)
        {
            Require(
                RingMeasurementSequence.GetChannelCellIndex(8, channel, profile) == channel,
                $"канал {channel} читает свою ячейку");
        }
    }),
    ("Кольцо 151 несёт десять ёмкостей и требует сдвига для 3 и 4 Tx", () =>
    {
        var profile = RingSignatureCatalog.GetCapacitanceMappingProfile(151);
        Require(profile == RingCapacitanceMappingProfile.UniversalFiveTransmitter, "профиль кольца 151");

        // Четыре передатчика: логические C5..C8 уходят в банк C6..C9.
        int[] expectedFourTx = [1, 2, 3, 4, 6, 7, 8, 9];
        for (var logical = 1; logical <= 8; logical++)
        {
            Require(
                RingMeasurementSequence.GetCommandCapacitance(8, logical, profile) == expectedFourTx[logical - 1],
                $"4 Tx: логическая C{logical}");
            Require(
                RingMeasurementSequence.GetChannelCellIndex(8, logical - 1, profile) == expectedFourTx[logical - 1] - 1,
                $"4 Tx: ячейка канала {logical - 1}");
        }

        // Три передатчика: логические C4..C6 уходят в банк C6..C8.
        int[] expectedThreeTx = [1, 2, 3, 6, 7, 8];
        for (var logical = 1; logical <= 6; logical++)
        {
            Require(
                RingMeasurementSequence.GetCommandCapacitance(6, logical, profile) == expectedThreeTx[logical - 1],
                $"3 Tx: логическая C{logical}");
        }

        // Пять передатчиков занимают обе банки целиком, сдвигать нечего.
        for (var logical = 1; logical <= 10; logical++)
        {
            Require(
                RingMeasurementSequence.GetCommandCapacitance(10, logical, profile) == logical,
                $"5 Tx: логическая C{logical}");
        }
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
    ("Диапазон записи покрывает ячейку Z8 четырёхзондового прибора на универсальном кольце", () =>
    {
        // Каналы 4 Tx на кольце со смещением занимают Z0..Z3 и Z5..Z8:
        // девять позиций, а не восемь. Подсчёт по числу каналов оставлял Z8
        // ненаписанной, и высокочастотный T4 навсегда сохранял чужую
        // реактивность.
        Require(RingParameterLayout.GetRequiredWriteCellCount(
            8,
            RingCapacitanceMappingProfile.UniversalFiveTransmitter) == 9);

        // На кольце с восемью ёмкостями смещения нет, и каналы ложатся подряд.
        Require(RingParameterLayout.GetRequiredWriteCellCount(
            8,
            RingCapacitanceMappingProfile.CompactFourTransmitter) == 8);

        // Трёхзондовый прибор занимает Z0..Z2 и Z5..Z7 - восьми ячеек хватает.
        Require(RingParameterLayout.GetRequiredWriteCellCount(
            6,
            RingCapacitanceMappingProfile.UniversalFiveTransmitter) == 8);

        // Пятизондовый прибор занимает весь набор.
        Require(RingParameterLayout.GetRequiredWriteCellCount(
            10,
            RingCapacitanceMappingProfile.UniversalFiveTransmitter) == 10);

        // Сопротивления R1..R8 требуют Z0..Z7 даже без единого канала.
        Require(RingParameterLayout.GetRequiredWriteCellCount(
            0,
            RingCapacitanceMappingProfile.UniversalFiveTransmitter) == 8);
    }),
    ("Каждая ячейка канала попадает в диапазон записи", () =>
    {
        // Прямая проверка свойства, ради которого диапазон и считается:
        // ни один канал не должен отображаться за пределы записи.
        foreach (var profile in new[]
                 {
                     RingCapacitanceMappingProfile.UniversalFiveTransmitter,
                     RingCapacitanceMappingProfile.CompactFourTransmitter,
                     RingCapacitanceMappingProfile.CompatibilityFallback
                 })
        {
            foreach (var channelCount in new[] { 6, 8, 10 })
            {
                var writeCount = RingParameterLayout.GetRequiredWriteCellCount(channelCount, profile);
                for (var channel = 0; channel < channelCount; channel++)
                {
                    var cellIndex = RingMeasurementSequence.GetChannelCellIndex(channelCount, channel, profile);
                    Require(cellIndex >= 0 && cellIndex < writeCount);
                }
            }
        }
    }),
    ("Кольцо на восемь ёмкостей: 4 Tx читает реактивности подряд", () =>
    {
        var cells = CreateRingCells(
            resistanceCount: 8,
            reactanceCount: 8,
            returnedCellCount: 10);
        var layout = RingParameterLayout.BuildParameterSet(
            cells,
            8,
            RingCapacitanceMappingProfile.CompactFourTransmitter);

        Require(layout.IsComplete);
        Require(layout.RequiredCellCount == 8);
        Require(layout.CompleteCellCount == 8);
        Require(layout.Parameters.Count == 8);

        // Реактивность канала берётся из его собственной ячейки: Z_i = i + 0,5.
        for (var channel = 0; channel < 8; channel++)
        {
            Require(
                NearlyEqual(layout.Parameters[channel].Reactance, channel + 0.5f),
                $"канал {channel} читает Z{channel}");
        }
    }),
    ("Кольцо на десять ёмкостей: 4 Tx читает высокочастотную группу со сдвигом", () =>
    {
        // Регрессия на прогон 22.09: приложение командовало физической ёмкостью
        // со сдвигом, а эталонную реактивность брало по несдвинутому индексу,
        // и все каналы 2001 кГц сравнивались с чужой мерой.
        var cells = CreateRingCells(
            resistanceCount: 8,
            reactanceCount: 10,
            returnedCellCount: 10);
        var layout = RingParameterLayout.BuildParameterSet(
            cells,
            8,
            RingCapacitanceMappingProfile.UniversalFiveTransmitter);

        Require(layout.IsComplete);
        Require(layout.Parameters.Count == 8);

        // Каналы 0..3 читают Z0..Z3, каналы 4..7 - Z5..Z8.
        int[] expectedCells = [0, 1, 2, 3, 5, 6, 7, 8];
        for (var channel = 0; channel < 8; channel++)
        {
            Require(
                NearlyEqual(layout.Parameters[channel].Reactance, expectedCells[channel] + 0.5f),
                $"канал {channel} читает Z{expectedCells[channel]}");
        }

        // Сопротивления по-прежнему берутся по индексу набора: R1..R8 в Z0..Z7.
        for (var index = 0; index < 8; index++)
        {
            Require(NearlyEqual(layout.Parameters[index].Resistance, index + 1f), $"R{index + 1} в Z{index}");
        }
    }),
    ("Кольцо на десять ёмкостей: 3 Tx читает высокочастотную группу со сдвигом", () =>
    {
        var cells = CreateRingCells(
            resistanceCount: 8,
            reactanceCount: 10,
            returnedCellCount: 10);
        var layout = RingParameterLayout.BuildParameterSet(
            cells,
            6,
            RingCapacitanceMappingProfile.UniversalFiveTransmitter);

        Require(layout.IsComplete);
        Require(layout.RequiredCellCount == 8);
        Require(layout.Parameters.Count == 8);

        // Каналы 0..2 читают Z0..Z2, каналы 3..5 - Z5..Z7.
        int[] expectedCells = [0, 1, 2, 5, 6, 7];
        for (var channel = 0; channel < 6; channel++)
        {
            Require(
                NearlyEqual(layout.Parameters[channel].Reactance, expectedCells[channel] + 0.5f),
                $"канал {channel} читает Z{expectedCells[channel]}");
        }
    }),
    ("Обратное отображение ячейки на канал защищает запись в кольцо", () =>
    {
        // Запись обязана класть реактивность канала в ячейку его физической
        // ёмкости. Позиции, которые прибору не принадлежат, относятся к пятому
        // передатчику: рассчитать их нечем, и перезаписывать нельзя.
        var universal = RingCapacitanceMappingProfile.UniversalFiveTransmitter;
        int[] expectedOwners = [0, 1, 2, 3, -1, 4, 5, 6, 7, -1];
        for (var cell = 0; cell < 10; cell++)
        {
            Require(
                RingMeasurementSequence.GetCellChannelIndex(8, cell, universal) == expectedOwners[cell],
                $"универсальное кольцо, 4 Tx: владелец Z{cell}");
        }

        // Три передатчика занимают ещё меньше позиций.
        int[] expectedOwnersThreeTx = [0, 1, 2, -1, -1, 3, 4, 5, -1, -1];
        for (var cell = 0; cell < 10; cell++)
        {
            Require(
                RingMeasurementSequence.GetCellChannelIndex(6, cell, universal) == expectedOwnersThreeTx[cell],
                $"универсальное кольцо, 3 Tx: владелец Z{cell}");
        }

        // На компактном кольце и на пятипередатчиковом приборе соответствие прямое.
        for (var cell = 0; cell < 8; cell++)
        {
            Require(
                RingMeasurementSequence.GetCellChannelIndex(
                    8,
                    cell,
                    RingCapacitanceMappingProfile.CompactFourTransmitter) == cell,
                $"компактное кольцо: владелец Z{cell}");
        }

        for (var cell = 0; cell < 10; cell++)
        {
            Require(
                RingMeasurementSequence.GetCellChannelIndex(10, cell, universal) == cell,
                $"5 Tx: владелец Z{cell}");
        }
    }),
    ("Кольцо на восемь ёмкостей не считается полным по правилам универсального", () =>
    {
        // Восемь заполненных ячеек достаточно только для компактного кольца.
        // Универсальное требует ещё и Z8 под четвёртый высокочастотный канал.
        var cells = CreateRingCells(
            resistanceCount: 8,
            reactanceCount: 8,
            returnedCellCount: 10);

        Require(
            RingParameterLayout.BuildParameterSet(
                cells,
                8,
                RingCapacitanceMappingProfile.CompactFourTransmitter).IsComplete,
            "компактное кольцо полно");
        Require(
            !RingParameterLayout.BuildParameterSet(
                cells,
                8,
                RingCapacitanceMappingProfile.UniversalFiveTransmitter).IsComplete,
            "универсальному кольцу не хватает Z8");
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
    ("Манифест обновления из двух строк разбирается как прежде", () =>
    {
        Require(UpdateManifest.TryParse("1.1.5\r\nKoltsa-Setup-1.1.5.exe\r\n", out var manifest, out var error));
        Require(error is null && manifest is not null);
        Require(manifest!.Version == new Version(1, 1, 5), "version");
        Require(manifest.InstallerFileName == "Koltsa-Setup-1.1.5.exe", "installer");
        Require(manifest.Sha256 is null, "no hash in the two-line form");
        Require(manifest.VersionText == "1.1.5", "version text");
    }),
    ("Манифест принимает необязательную третью строку с SHA-256", () =>
    {
        var hash = new string('a', 64);
        Require(UpdateManifest.TryParse($"1.2.0\nKoltsa-Setup-1.2.0.exe\n{hash}\n", out var manifest, out var error));
        Require(error is null && manifest is not null);
        Require(manifest!.Sha256 == hash.ToUpperInvariant(), "hash is normalised to upper case");

        Require(!UpdateManifest.TryParse("1.2.0\nKoltsa-Setup-1.2.0.exe\nZZZZ", out _, out var badHashError));
        Require(!string.IsNullOrWhiteSpace(badHashError), "short or non-hex hash is rejected");
    }),
    ("Манифест отвергает имя установщика с путём", () =>
    {
        // Имя приходит по незащищённому HTTP и подставляется и в адрес загрузки,
        // и в путь сохранения, поэтому выход за каталог должен быть невозможен.
        Require(!UpdateManifest.IsSafeInstallerFileName("..\\..\\evil.exe"), "parent traversal");
        Require(!UpdateManifest.IsSafeInstallerFileName("sub/dir/setup.exe"), "forward slash");
        Require(!UpdateManifest.IsSafeInstallerFileName("C:\\Windows\\evil.exe"), "absolute path");
        Require(!UpdateManifest.IsSafeInstallerFileName("setup.bat"), "not an exe");
        Require(!UpdateManifest.IsSafeInstallerFileName(""), "empty");
        Require(UpdateManifest.IsSafeInstallerFileName("Koltsa-Setup-1.1.5.exe"), "plain file name");

        Require(!UpdateManifest.TryParse("1.1.5\n..\\evil.exe", out _, out var error));
        Require(!string.IsNullOrWhiteSpace(error));
    }),
    ("Версия сборки и версия сервера сравниваются по трём разрядам", () =>
    {
        // Сборка объявляет 1.1.4.0, сервер пишет 1.1.4: без нормализации
        // Version("1.1.4") имеет Revision -1 и считается младше собственной сборки.
        Require(!UpdateManifest.IsNewerVersion(new Version("1.1.4"), new Version("1.1.4.0")), "same release");
        Require(UpdateManifest.IsNewerVersion(new Version("1.1.5"), new Version("1.1.4.0")), "newer build");
        Require(UpdateManifest.IsNewerVersion(new Version("1.2.0"), new Version("1.1.9.0")), "newer minor");
        Require(!UpdateManifest.IsNewerVersion(new Version("1.1.3"), new Version("1.1.4.0")), "older build");
        Require(UpdateManifest.FormatVersion(new Version("1.1.4.0")) == "1.1.4", "three-part text");
    }),
    ("Аргументы тихой установки не содержат отвергаемых Inno ключей", () =>
    {
        var arguments = UpdateManifest.BuildSilentInstallArguments(@"C:\Temp\koltsa_setup.log");

        Require(arguments.Contains("/VERYSILENT", StringComparison.Ordinal), "silent");
        Require(arguments.Contains("/SUPPRESSMSGBOXES", StringComparison.Ordinal), "no message boxes");
        Require(arguments.Contains("/CLOSEAPPLICATIONS", StringComparison.Ordinal), "close running copies");
        Require(arguments.Contains("/RESTARTAPP=1", StringComparison.Ordinal), "relaunch flag for the [Run] entry");
        Require(arguments.Contains(@"/LOG=""C:\Temp\koltsa_setup.log""", StringComparison.Ordinal), "setup log");

        // /CURRENTUSER Inno принимает только при PrivilegesRequiredOverridesAllowed=commandline,
        // иначе прекращает установку, а с /SUPPRESSMSGBOXES делает это молча.
        Require(!arguments.Contains("/CURRENTUSER", StringComparison.Ordinal), "no privilege override switch");
        // Перезапуск выполняет запись [Run]; второй механизм открыл бы две копии.
        Require(!arguments.Contains("/RESTARTAPPLICATIONS", StringComparison.Ordinal), "no restart manager relaunch");
    }),
    ("Имя временного файла обновления остаётся в пределах каталога", () =>
    {
        Require(UpdateManifest.TryParse("2.0.0\nKoltsa-Setup-2.0.0.exe", out var manifest, out _));
        var fileName = UpdateManifest.GetDownloadFileName(manifest!);

        Require(fileName == "koltsa_update_Koltsa-Setup-2.0.0.exe", "prefixed name");
        Require(Path.GetFileName(fileName) == fileName, "no directory part");

        var uri = manifest!.BuildDownloadUri("http://example.test/files/");
        Require(uri.ToString() == "http://example.test/files/Koltsa-Setup-2.0.0.exe", "download url");
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
    }),
    ("Незаполненные L1/L2 признаются непригодной геометрией", () =>
    {
        Require(new SondeParameter { L1 = 0.345f, L2 = 0.599f, Frequency = 401000f }.HasUsableGeometry, "исправный канал");
        Require(!new SondeParameter { L1 = 0f, L2 = 0f, Frequency = 401000f }.HasUsableGeometry, "нули из метрологии");
        Require(!new SondeParameter { L1 = 0.345f, L2 = 0f, Frequency = 401000f }.HasUsableGeometry, "нет L2");
        Require(!new SondeParameter { L1 = 0.599f, L2 = 0.345f, Frequency = 401000f }.HasUsableGeometry, "L2 не больше L1");
        Require(!new SondeParameter { L1 = 0.345f, L2 = 0.599f, Frequency = 0f }.HasUsableGeometry, "нет частоты");
        Require(!new SondeParameter { L1 = float.NaN, L2 = 0.599f, Frequency = 401000f }.HasUsableGeometry, "NaN в геометрии");
    }),
    ("Прямая задача на вырожденной геометрии отдаёт NaN, а не число из неопределённости", () =>
    {
        var loop = new LoopParameter { Resistance = 1f, Reactance = 0.9018f, Radius = 0.5f, AdditionalWireResistance = 0.5f };
        var degenerate = new SondeParameter { L1 = 0f, L2 = 0f, Frequency = 401000f };
        var usable = new SondeParameter { L1 = 0.345f, L2 = 0.599f, Frequency = 401000f };

        Require(float.IsNaN(LoopResistivity.DfiFromLoop(degenerate, loop)), "вырожденный канал");
        Require(float.IsFinite(LoopResistivity.DfiFromLoop(usable, loop)), "исправный канал считается по-прежнему");
    }),
    ("Обратная задача завершается на вырожденной геометрии и на NaN-фазе", () =>
    {
        // Регрессия на зависание интерфейса: обе точки останова золотого сечения -
        // сравнения с невязкой, а любое сравнение с NaN ложно, поэтому прежний
        // do...while не завершался никогда и вешал поток, из которого его вызвали.
        var degenerate = new SondeParameter { L1 = 0f, L2 = 0f, Frequency = 401000f };
        var usable = new SondeParameter { L1 = 0.345f, L2 = 0.599f, Frequency = 401000f };

        RequireCompletes(
            () => Require(float.IsNaN(LoopResistivity.RoDfi(degenerate, 0.87f)), "вырожденная геометрия"),
            "RoDfi на вырожденной геометрии");
        RequireCompletes(
            () => Require(float.IsNaN(LoopResistivity.RoDfi(usable, float.NaN)), "NaN-фаза"),
            "RoDfi на NaN-фазе");
        RequireCompletes(
            () => Require(float.IsFinite(LoopResistivity.RoDfi(usable, 0.01f)), "исправный вход"),
            "RoDfi на исправном входе");
    }),
    ("Эталоны прибора 320151303 считаются по заполненным каналам и NaN по пустым", () =>
    {
        // Реальная метрология прибора Автономный 5 Tx № 303: сигнатура объявляет
        // пять передатчиков, а L1/L2 для T5 не заданы. Каналы 4 и 9 обязаны
        // остаться без эталона, остальные восемь - посчитаться как прежде.
        ushort[] l1 = [345, 720, 1100, 1480, 0];
        ushort[] l2 = [599, 974, 1354, 1734, 0];
        ushort[] frequencies = [401, 2001];

        var sondeParameters = new List<SondeParameter>();
        foreach (var frequency in frequencies)
        {
            for (var tx = 0; tx < 5; tx++)
            {
                sondeParameters.Add(new SondeParameter
                {
                    L1 = l1[tx] / 1000f,
                    L2 = l2[tx] / 1000f,
                    Frequency = frequency * 1000f
                });
            }
        }

        float[] resistances = [1f, 5f, 20f, 50f, 100f, 428f, 814f, 1500f, 0f, 0f];
        float[] reactances = [0.9018f, 1.5395f, 1.8522f, 2.0095f, 4.5004f, 7.6801f, 9.2408f, 10.02f, 9.842f, 10.258f];
        var loopParameters = new List<LoopParameter>();
        for (var i = 0; i < 10; i++)
        {
            loopParameters.Add(new LoopParameter
            {
                Resistance = resistances[i],
                Reactance = reactances[i],
                Radius = 0.5f,
                AdditionalWireResistance = 0.5f
            });
        }

        RequireCompletes(
            () =>
            {
                var phases = LoopResistivity.CalcPhaseFromLoop(sondeParameters, loopParameters);
                var resistivities = LoopResistivity.CalcRoFromLoop(sondeParameters, phases);

                Require(phases.Count == MeasurementTable.ResistanceCount + 1, "число наборов фаз");
                Require(resistivities.Count == phases.Count, "число наборов УЭС");

                for (var resistance = 1; resistance <= MeasurementTable.ResistanceCount; resistance++)
                {
                    foreach (var channel in new[] { 4, 9 })
                    {
                        Require(float.IsNaN(phases[resistance].GetValue(channel)), $"R{resistance} канал {channel}: фаза");
                        Require(float.IsNaN(resistivities[resistance].GetValue(channel)), $"R{resistance} канал {channel}: УЭС");
                    }

                    foreach (var channel in new[] { 0, 1, 2, 3, 5, 6, 7, 8 })
                    {
                        Require(float.IsFinite(phases[resistance].GetValue(channel)), $"R{resistance} канал {channel}: фаза");
                        Require(float.IsFinite(resistivities[resistance].GetValue(channel)), $"R{resistance} канал {channel}: УЭС");
                    }
                }
            },
            "расчёт эталонов на неполной метрологии");
    }),
    ("Ёмкость вырожденного канала не подменяется нижней границей поиска", () =>
    {
        // Прежде тернарный поиск на нулевой геометрии сползал на CapacitanceMin и
        // выдавал 500,408 пФ с отрицательной реактивностью — значения, по виду
        // неотличимые от расчётных и негодные к записи в кольцо.
        var degenerate = RingCalibrationCalculator.Calculate(
            new SondeParameter { L1 = 0f, L2 = 0f, Frequency = 401000f });
        Require(float.IsNaN(degenerate.CapacitanceFarads), "ёмкость вырожденного канала");
        Require(float.IsNaN(degenerate.Reactance), "реактивность вырожденного канала");
        Require(
            double.IsNaN(RingCalibrationCalculator.CMaxFi(
                401000,
                RingCalibrationCalculator.DefaultLoopRadius,
                RingCalibrationCalculator.DefaultLoopInductance,
                0,
                0)),
            "прямой вызов C_max_fi");

        var usable = RingCalibrationCalculator.Calculate(
            new SondeParameter { L1 = 0.345f, L2 = 0.599f, Frequency = 401000f });
        Require(float.IsFinite(usable.CapacitanceFarads) && usable.CapacitanceFarads > 0f, "ёмкость исправного канала");
        Require(float.IsFinite(usable.Reactance) && usable.Reactance > 0f, "реактивность исправного канала");
    }),
    ("Оба поколения отвечают одинаково устроенным length-framed пакетом", () =>
    {
        // Старая прошивка ставит в поле длины постоянные 245 (main_LWD_1DDS,
        // GetDataCommand), новая вычисляет struct_size + 5 = 325. Разбор один.
        var legacyFrame = new byte[ToolProtocolConstants.ToolDataSize];
        BitConverter.GetBytes(0x000386BEu).CopyTo(legacyFrame, 0);
        var legacyPacket = ToolSerialTransport.BuildFramedPacket(ToolProtocolConstants.CmdGetData, legacyFrame);
        Require(legacyPacket.Length == 245, "длина пакета старой прошивки");
        Require(legacyPacket[1] == 245 && legacyPacket[2] == 0, "поле длины старой прошивки");

        var extendedFrame = new byte[ToolProtocolConstants.ExtendedToolDataSize];
        BitConverter.GetBytes(320151303u).CopyTo(extendedFrame, 0);
        var extendedPacket = ToolSerialTransport.BuildFramedPacket(ToolProtocolConstants.CmdGetData, extendedFrame);
        Require(extendedPacket.Length == 325, "длина пакета новой прошивки");
        Require(extendedPacket[1] == 0x45 && extendedPacket[2] == 0x01, "поле длины новой прошивки");

        foreach (var packet in new[] { legacyPacket, extendedPacket })
        {
            Require(
                ToolSerialTransport.ParseFramedPacket(packet, out var cmd, out var payload) == ToolResponseCode.Ok,
                "разбор пакета");
            Require(cmd == ToolProtocolConstants.CmdGetData, "код команды");
            Require(ToolProtocolConstants.IsKnownFrameSize(payload.Length), "размер кадра известен");
        }

        Require(!ToolProtocolConstants.IsKnownFrameSize(242), "242 не является размером кадра");
        Require(!ToolProtocolConstants.IsKnownFrameSize(0), "нулевой размер кадра");
    }),
    ("Ответ метрологии совпадает у обоих транспортов побайтово", () =>
    {
        // Транспорт фиксированной длины - это тот же пакет, у которого поле длины
        // отдано в нагрузку. Проверка фиксирует это как контракт: прежняя ветка
        // читала LEN как два служебных байта и работала только поэтому.
        var metrology = new byte[ToolProtocolConstants.ToolMetrologySize];
        BitConverter.GetBytes(0x13151F07u).CopyTo(metrology, 0);
        var packet = ToolSerialTransport.BuildFramedPacket(ToolProtocolConstants.CmdGetMetrology, metrology);

        Require(packet.Length == 245, "длина пакета метрологии");
        Require(packet[1] == 0xF5 && packet[2] == 0x00, "поле длины метрологии");

        // Прочтение «фиксированной длиной»: нагрузка 242 = 2 служебных + 240.
        var fixedLengthPayload = packet.AsSpan(1, ToolProtocolConstants.ToolPayloadLength);
        Require(
            fixedLengthPayload[..ToolProtocolConstants.ToolServicePrefixLength].SequenceEqual(packet.AsSpan(1, 2)),
            "служебные байты - это поле длины");
        Require(
            fixedLengthPayload[ToolProtocolConstants.ToolServicePrefixLength..].SequenceEqual(metrology),
            "структура совпадает при обоих способах чтения");
    }),
    ("Знак ориентации фазы задаётся чётностью Tx и не зависит от Rx_Position", () =>
    {
        // Все три исходника прошивки (main_AUTO_5Tx.txt, main_LWD_1DDS,
        // LWD_1_DDS_ATT_UES (2).c) берут Dir = (Tx % 2 == 0) ? +1 : -1, а
        // Rx_Position обслуживают перестановкой каналов АЦП до расчёта DELTA_PH.
        int[] expected = [1, -1, 1, -1, 1];
        for (uint tx = 0; tx < 5; tx++)
        {
            Require(PhaseMath.TxPhaseOrientationSign(tx) == expected[tx], $"знак T{tx + 1}");
        }

        // Прежний расчёт совпадал с прошивкой только при Rx_Position == 0.
        for (uint tx = 0; tx < 5; tx++)
        {
            Require(
                PhaseMath.LegacyRxPhaseOrientationSign(tx, 0) == expected[tx],
                $"прежний знак при Rx_Position=0, T{tx + 1}");
            Require(
                PhaseMath.LegacyRxPhaseOrientationSign(tx, 1) == -expected[tx],
                $"прежний знак при Rx_Position=1 переворачивался, T{tx + 1}");
        }

        // Применение знака к сырой разности фаз.
        Require(Math.Abs(PhaseMath.ApplyTxPhaseOrientation(0.5f, 0f, 0) - 0.5f) < 1e-6f, "T1 без переворота");
        Require(Math.Abs(PhaseMath.ApplyTxPhaseOrientation(0.5f, 0f, 1) + 0.5f) < 1e-6f, "T2 с переворотом");
    }),
    ("Байт состояния разбирается одинаково в обоих поколениях", () =>
    {
        // 00000000 00000000 00054321 00054321: биты 8..12 - 400 кГц, 0..4 - 2000 кГц.
        const uint allActive4Tx = 0x00000F0Fu;
        for (var tx = 0; tx < 4; tx++)
        {
            Require(GpToolData.IsTransmitterActive(allActive4Tx, 0, tx), $"400 кГц T{tx + 1}");
            Require(GpToolData.IsTransmitterActive(allActive4Tx, 1, tx), $"2000 кГц T{tx + 1}");
        }

        Require(!GpToolData.IsTransmitterActive(allActive4Tx, 0, 4), "400 кГц T5 отсутствует");
        Require(!GpToolData.IsTransmitterActive(allActive4Tx, 1, 4), "2000 кГц T5 отсутствует");

        Require(GpToolData.GetConditionByte(0x00001F1Fu, 0) == 0x1F, "байт 400 кГц");
        Require(GpToolData.GetConditionByte(0x00001F1Fu, 1) == 0x1F, "байт 2000 кГц");

        // Отказ третьего передатчика на 2000 кГц при исправных 400 кГц.
        const uint thirdDeadOnHighFrequency = 0x00001F1Bu;
        Require(GpToolData.IsTransmitterActive(thirdDeadOnHighFrequency, 0, 2), "400 кГц T3 работает");
        Require(!GpToolData.IsTransmitterActive(thirdDeadOnHighFrequency, 1, 2), "2000 кГц T3 молчит");
    }),
    ("Поля метрологии новой прошивки читаются по документированным смещениям", () =>
    {
        // Байты взяты из metrology_32023.bin реального прибора 320151303.
        var metrology = new byte[ExtendedMetrologyFields.MetrologySize];
        BitConverter.GetBytes(0x13151F07u).CopyTo(metrology, 0);
        BitConverter.GetBytes(-14.065260f).CopyTo(metrology, ExtendedMetrologyFields.AirAttenuationDbOffset);
        BitConverter.GetBytes(-4.033413f).CopyTo(metrology, ExtendedMetrologyFields.AirAttenuationDbOffset + (9 * 4));
        BitConverter.GetBytes(1790064966L).CopyTo(metrology, ExtendedMetrologyFields.WriteTimestampOffset);

        Require(ExtendedMetrologyFields.AirAttenuationDbOffset == 84, "смещение поправок затухания");
        Require(ExtendedMetrologyFields.WriteTimestampOffset == 124, "смещение отметки времени");

        Require(ExtendedMetrologyFields.TryReadAirAttenuationDb(metrology, 0, 0, out var first), "F1T1 читается");
        Require(Math.Abs(first + 14.065260f) < 1e-4f, "F1T1 значение");
        Require(ExtendedMetrologyFields.TryReadAirAttenuationDb(metrology, 1, 4, out var last), "F2T5 читается");
        Require(Math.Abs(last + 4.033413f) < 1e-4f, "F2T5 значение");

        Require(ExtendedMetrologyFields.TryReadWriteTimestamp(metrology, out var timestamp), "отметка читается");
        Require(timestamp.ToUnixTimeSeconds() == 1790064966L, "отметка совпадает");

        // Неразмеченная память и метрология до появления поля отметки не дают.
        var blank = new byte[ExtendedMetrologyFields.MetrologySize];
        Require(!ExtendedMetrologyFields.TryReadWriteTimestamp(blank, out _), "нули отметки не дают");
        var erased = new byte[ExtendedMetrologyFields.MetrologySize];
        erased.AsSpan(ExtendedMetrologyFields.WriteTimestampOffset, 8).Fill(0xFF);
        Require(!ExtendedMetrologyFields.TryReadWriteTimestamp(erased, out _), "0xFF отметки не дают");
    }),
    ("Запись метрологии новой прошивки идёт отдельным кодом команды", () =>
    {
        // У старой прошивки отдельного кода записи нет: запись идёт тем же 0x13,
        // что и чтение. Новая развела их, и отправка 0x13 с нагрузкой в новый
        // прибор обработчика не найдёт.
        Require(ToolProtocolConstants.CmdSetMetrologyExtended == 0x12, "код записи метрологии");
        Require(ToolProtocolConstants.CmdGetMetrology == 0x13, "код чтения метрологии");
        Require(
            ToolProtocolConstants.CmdSetMetrologyExtended != ToolProtocolConstants.CmdGetMetrology,
            "коды чтения и записи различаются");
    }),
    ("Точка с малым эталоном не объявляется молчащей", () =>
    {
        // Порог обнаружения не опускается ниже 100 мГ. На больших сопротивлениях
        // сам эталон падает до десятков мГ, и безупречное измерение его не
        // превысит. Такие точки обязаны оставаться неопределёнными, иначе
        // исправный прогон будет прерван как обрыв связи.
        var monitor = new RingFirmwarePointMonitor();
        monitor.Observe(1, 1000, 40, 46, false);
        monitor.Observe(2, 2000, 45, 46, false);
        monitor.Observe(3, 3000, 44, 46, true);
        monitor.Observe(4, 4000, 43, 46, true);

        Require(
            monitor.BuildSummary().State == RingFirmwareRetentionState.Inconclusive,
            "эталон ниже порога обнаружения");

        // Эталон с запасом над порогом по-прежнему классифицируется.
        var confident = new RingFirmwarePointMonitor();
        confident.Observe(1, 1000, 20, 1000, false);
        confident.Observe(2, 2000, 30, 1000, true);
        confident.Observe(3, 3000, 25, 1000, true);
        Require(
            confident.BuildSummary().State == RingFirmwareRetentionState.NoResponse,
            "молчащее кольцо при большом эталоне");
    }),
    ("Реальная точка R1 C1 прибора 320151303 признаётся удержанной", () =>
    {
        // Единственная точка, взятая до обрыва связи 22.09: эталон 49942 мГ,
        // измерено 48290 мГ, отклонение -2%. Классификация обязана быть held,
        // иначе новое прерывание прогона срежет исправные точки.
        var monitor = new RingFirmwarePointMonitor();
        monitor.Observe(1, 1000, 47800, 49942.120, false);
        monitor.Observe(2, 2000, 48100, 49942.120, false);
        monitor.Observe(3, 3000, 48290, 49942.120, true);
        monitor.Observe(4, 4000, 48210, 49942.120, true);
        monitor.Observe(5, 5000, 48350, 49942.120, true);

        var summary = monitor.BuildSummary();
        Require(summary.State == RingFirmwareRetentionState.Held, "точка удержана");
        Require(summary.RetentionRatio >= 0.5, "отношение удержания");
    }),
    ("Сброс локального воздуха не ждёт кадра прибора", () =>
    {
        // Регрессия 22.09: опрос был на паузе, сброс оставался отложенным, и
        // каждое нажатие кнопки снова уходило в ветвь сброса. Кнопка выглядела
        // неотвечающей: 48 нажатий подряд, все с currently_set=True.
        var corrector = new LocalAirCorrector();
        corrector.Begin(2);

        var phase = default(V10);
        phase.Clear();
        for (var channel = 0; channel < 8; channel++)
        {
            phase.SetValue(channel, 0.01f * (channel + 1));
        }

        // Стабилизационные кадры, затем образцы: воздух становится заданным.
        for (var frame = 0; frame < LocalAirCorrector.SettleFrames + 2; frame++)
        {
            corrector.ProcessFrame(phase, 8);
        }

        Require(corrector.IsSet, "воздух задан");

        // Кадры прекратились. Отложенный сброс сам по себе состояние не меняет.
        corrector.RequestReset("test");
        Require(corrector.IsSet, "до применения воздух ещё задан");
        Require(corrector.ResetRequested, "сброс запрошен");

        Require(corrector.ApplyPendingReset(8), "сброс применён вне кадрового пути");
        Require(!corrector.IsSet, "воздух сброшен");
        Require(!corrector.ResetRequested, "запрос снят");
        Require(!corrector.Collecting, "сбор остановлен");

        // Повторное применение без запроса ничего не делает.
        Require(!corrector.ApplyPendingReset(8), "без запроса сбрасывать нечего");
    }),
    ("Устаревший первый кадр не выдаётся за отклик кольца", () =>
    {
        // Реальная точка R1/C8 Картографа 22.09: первый кадр повторил состояние
        // предыдущей точки C7, кадры 2..10 - шум. Кольцо меру не включало, и
        // объявлять её удержанной и отпущенной нельзя.
        var monitor = new RingFirmwarePointMonitor();
        double[] measuredPhasesMg =
        [
            110298.024, 27.682, -7.042, -0.520, -48.440,
            1.646, 9.391, -23.599, -21.980, 24.359
        ];
        for (var index = 0; index < measuredPhasesMg.Length; index++)
        {
            monitor.Observe(index + 1, (index + 1) * 1000.0, measuredPhasesMg[index], 107443.688, index + 1 > 5);
        }

        var summary = monitor.BuildSummary();
        Require(summary.State == RingFirmwareRetentionState.NoResponse, "отклика не было");
        Require(summary.PeakFrame != 1, "первый кадр не может быть пиком");

        // Отклик со второго кадра уликой является: он снят заведомо после команды.
        var genuine = new RingFirmwarePointMonitor();
        double[] droppedPhasesMg =
        [
            -7.691, 38270.855, 733.620, 17.773, 12.147,
            10.382, -1.059, -25.661, -2.097, 9.132
        ];
        for (var index = 0; index < droppedPhasesMg.Length; index++)
        {
            genuine.Observe(index + 1, (index + 1) * 1000.0, droppedPhasesMg[index], 53578.801, index + 1 > 5);
        }

        Require(
            genuine.BuildSummary().State == RingFirmwareRetentionState.Dropped,
            "отклик со второго кадра остаётся сорванной мерой");
    }),
    ("Реальные точки R1 C3..C5 признаются молчащими", () =>
    {
        // Те же прогон и прибор: эталоны десятки тысяч мГ, измерено единицы.
        (double Reference, double Measured)[] points =
        [
            (45782.497, 11.012),
            (39620.154, -89.755),
            (7994.514, 329.322)
        ];

        foreach (var (reference, measured) in points)
        {
            var monitor = new RingFirmwarePointMonitor();
            monitor.Observe(1, 1000, measured * 0.8, reference, false);
            monitor.Observe(2, 2000, measured, reference, false);
            monitor.Observe(3, 3000, measured * 0.9, reference, true);
            monitor.Observe(4, 4000, measured * 1.1, reference, true);

            Require(
                monitor.BuildSummary().State == RingFirmwareRetentionState.NoResponse,
                $"эталон {reference:F0} мГ, измерено {measured:F0} мГ");
        }
    }),
    ("Порог предупреждения о медленном открытии порта кольца", () =>
    {
        // Исправный порт открывается за десятки миллисекунд; 22.09 COM12
        // открывался 4184 мс и оборвался посреди прогона.
        Require(RingConnection.SlowPortOpenWarningMs == 1000, "порог предупреждения");

        var healthy = new RingConnectResult(RingConnection.RingConnectStatus.Connected, RingDeviceInfo.Legacy, 37);
        Require(healthy.IsConnected && !healthy.IsLinkSlow, "здоровый канал");

        var slow = new RingConnectResult(RingConnection.RingConnectStatus.Connected, RingDeviceInfo.Legacy, 4184);
        Require(slow.IsConnected && slow.IsLinkSlow, "канал 22.09 признаётся нездоровым");

        // Прежняя форма результата без времени открытия не должна считаться медленной.
        var legacyShape = new RingConnectResult(RingConnection.RingConnectStatus.Connected, RingDeviceInfo.Legacy);
        Require(!legacyShape.IsLinkSlow, "нет сведений о времени открытия");
    }),
    ("Транспорт выбирается по поколению прошивки, а не по типу прибора", () =>
    {
        // Поле длины отсутствует только у Картографа LWD 4tx старой прошивки.
        var legacyCartograph = ToolIdentity.Decode(349001u);
        Require(legacyCartograph.IsCartographLwd4TxMode, "старый Картограф распознан");
        Require(!legacyCartograph.IsExtendedFirmware, "старый Картограф: кадр 240");
        Require(ToolPort.UsesFixedLengthTransport(in legacyCartograph), "старый Картограф: фиксированная длина");

        // Перепрошитый Картограф отвечает как все остальные приборы.
        var extendedCartograph = ToolIdentity.Decode(320349001u);
        Require(extendedCartograph.IsCartographLwd4TxMode, "новый Картограф распознан");
        Require(extendedCartograph.IsExtendedFirmware, "новый Картограф: кадр 320");
        Require(!ToolPort.UsesFixedLengthTransport(in extendedCartograph), "новый Картограф: поле длины");

        foreach (var signature in new[] { 0x00024FCEu, 0x3143ADD1u, 320151303u, 320242107u })
        {
            var identity = ToolIdentity.Decode(signature);
            Require(
                !ToolPort.UsesFixedLengthTransport(in identity),
                $"0x{signature:X8}: Автоном и LWD всегда с полем длины");
        }
    }),
    ("Кадр Картографа 320349001 разбирается как length-framed", () =>
    {
        // Первые байты реального ответа прибора, снятые 22.09:
        //   11 45 01 | 49 23 18 13 | 0F 0F 00 00 | 57 00 00 00 | 40 9A B5 41
        //   CMD LEN    сигнатура     condition     frame=87      t=22,7 °C
        var frame = new byte[ToolProtocolConstants.ExtendedToolDataSize];
        Convert.FromHexString("492318130F0F00005700000040 9AB541".Replace(" ", "")).CopyTo(frame, 0);
        var packet = ToolSerialTransport.BuildFramedPacket(ToolProtocolConstants.CmdGetData, frame);

        Require(packet.Length == 325, "длина пакета");
        Require(packet[0] == 0x11 && packet[1] == 0x45 && packet[2] == 0x01, "заголовок 11 45 01");
        Require(
            ToolSerialTransport.ParseFramedPacket(packet, out var cmd, out var payload) == ToolResponseCode.Ok,
            "пакет разбирается");
        Require(cmd == ToolProtocolConstants.CmdGetData, "код команды");
        Require(payload.Length == ToolProtocolConstants.ExtendedToolDataSize, "размер кадра");

        var data = MemoryMarshal.Read<GpToolData>(payload);
        Require(data.Signature == 320349001u, "сигнатура кадра");
        Require(data.Condition == 0x00000F0Fu, "байт состояния");
        Require(data.Frame == 87u, "номер кадра");
        Require(Math.Abs(data.Temperature - 22.7f) < 0.05f, "температура");

        // Все четыре передатчика Картографа объявлены рабочими на обеих частотах.
        for (var tx = 0; tx < 4; tx++)
        {
            Require(GpToolData.IsTransmitterActive(data.Condition, 0, tx), $"400 кГц T{tx + 1}");
            Require(GpToolData.IsTransmitterActive(data.Condition, 1, tx), $"2000 кГц T{tx + 1}");
        }

        Require(!GpToolData.IsTransmitterActive(data.Condition, 0, 4), "T5 у Картографа отсутствует");

        // Прежнее чтение фиксированной длиной обрывало пакет на 245 байтах и
        // сравнивало как CRC середину кадра - отсюда были CrcError и тайм-ауты.
        var truncated = packet.AsSpan(0, ToolProtocolConstants.ToolPacketLength);
        var wrongCrc = (ushort)(truncated[^2] | (truncated[^1] << 8));
        Require(Crc16.Compute(packet, 243) != wrongCrc, "усечённое чтение обязано не сходиться по CRC");
    }),
    ("Новая прошивка общая: Картограф разбирается на тех же условиях, что LWD", () =>
    {
        // Картограф в режиме LWD перепрошит тем же файлом, что Автономный и LWD.
        // get_sonde_id новой прошивки модификацию в выборе ветки не использует,
        // поэтому для нового поколения она не должна решать ничего и у нас.
        foreach (var modification in new uint[] { 1, 2, 9 })
        {
            var signature = 320_000_000u + (3u * 100_000u) + (4u * 10_000u) + (modification * 1_000u) + 7u;
            var identity = ToolIdentity.Decode(signature);
            Require(identity.IsValid, $"новый Картограф 4 Tx мод.{modification} поддержан");
            Require(identity.TransmitterCount == 4, $"число передатчиков, мод.{modification}");
            Require(identity.IsCartographLwd4TxMode, $"режим LWD 4tx, мод.{modification}");
            Require(!ToolPort.UsesFixedLengthTransport(in identity), $"поле длины, мод.{modification}");
        }

        // Пять передатчиков у новой прошивки берутся буквально: прошивка
        // опрашивает ровно N_Tx из сигнатуры, и подгонять 5 под 4 нельзя.
        var extendedFiveTx = ToolIdentity.Decode(320359007u);
        Require(extendedFiveTx.IsValid, "новый Картограф 5 Tx поддержан");
        Require(extendedFiveTx.TransmitterCount == 5, "пять передатчиков не подгоняются под четыре");
        Require(!extendedFiveTx.IsCartographLwd4TxMode, "это не режим LWD 4tx");

        // Старое поколение сохраняет исторические правила без изменений.
        var legacyFourTx = ToolIdentity.Decode(349007u);
        Require(legacyFourTx.IsValid && legacyFourTx.TransmitterCount == 4, "старый Картограф 349");
        var legacyFiveTx = ToolIdentity.Decode(359007u);
        Require(legacyFiveTx.IsValid && legacyFiveTx.TransmitterCount == 4, "старый Картограф 359 -> 4 Tx");
        var legacyWrongModification = ToolIdentity.Decode(341007u);
        Require(!legacyWrongModification.IsValid, "старый Картограф без модификации 9 не поддержан");
        Require(
            !legacyWrongModification.IsCartographLwd4TxMode,
            "неподдержанный прибор не уводит обмен в транспорт фиксированной длины");
    }),
    ("Ответ «Z8 -nan -nan» распознаётся как пустая ячейка, а не как сбой связи", () =>
    {
        Require(RingProtocolParser.IsEmptyLoopParameterCell(8, "Z8 -nan -nan"), "стёртая флеш-память");
        Require(RingProtocolParser.IsEmptyLoopParameterCell(8, "READ Z8 nan nan"), "вариант с префиксом READ");
        Require(!RingProtocolParser.IsEmptyLoopParameterCell(8, "Z8 1.000000 2.000000"), "обычные числа");
        Require(!RingProtocolParser.IsEmptyLoopParameterCell(8, "Z9 -nan -nan"), "чужой номер ячейки");
        Require(!RingProtocolParser.IsEmptyLoopParameterCell(8, "Unknown command!"), "отказ команды");
        Require(!RingProtocolParser.IsEmptyLoopParameterCell(8, ""), "пустой ответ - это нет ответа");
        Require(!RingProtocolParser.TryParseLoopParameterCell(8, "Z8 -nan -nan", out _), "NaN не принимается за значение");
    }),
    ("Кольцо коллег 23.09: пустая Z8 и сдвинутая высокочастотная группа", () =>
    {
        // Память кольца 151 до перезаписи и расчёт для LWD 4Tx № 42023 - с фото
        // вкладки «Поверка кольца».
        var audit = RingMemoryAudit.Evaluate(
            ColleagueRingCells(z8: null),
            8,
            RingCapacitanceMappingProfile.UniversalFiveTransmitter,
            ColleagueCalculatedReactances());

        Require(!audit.IsComplete, "без Z8 набор неполный");
        var blocking = audit.Blocking.ToArray();
        Require(blocking.Length == 1, $"одна блокирующая находка, получено {blocking.Length}");
        Require(blocking[0].Kind == RingCellIssueKind.ReactanceEmpty, "Z8 пуста, а не непрочитана");
        Require(blocking[0].CellIndex == 8 && blocking[0].ChannelIndex == 7, "Z8 нужна каналу T4 2001");

        var mismatches = audit.Mismatches.OrderBy(issue => issue.ChannelIndex).ToArray();
        Require(mismatches.Length == 3, $"сдвинуты три канала, получено {mismatches.Length}");
        Require(mismatches.Select(issue => issue.ChannelIndex).SequenceEqual(new[] { 4, 5, 6 }), "T1..T3 2001");
        Require(mismatches.Select(issue => issue.NearestChannelIndex).SequenceEqual(new[] { 5, 6, 7 }), "каждый похож на соседа");

        // Порог 25 % в одиночку пропустил бы T2 (+20 %) и T3 (+8 %).
        Require(mismatches[1].RelativeDeviation < RingMemoryAudit.MismatchRelativeLimit, "T2 ниже порога");
        Require(mismatches[2].RelativeDeviation < RingMemoryAudit.MismatchRelativeLimit, "T3 ниже порога");

        // Z4 и Z9 принадлежат пятому передатчику и этому прибору не нужны.
        Require(audit.Issues.All(issue => issue.CellIndex is not 4 and not 9), "Z4 и Z9 не упоминаются");
        Require(audit.Issues.All(issue => issue.ChannelIndex is not (0 or 1 or 2 or 3)), "низкая частота записана верно");
    }),
    ("Кольцо коллег после перезаписи 23.09 проходит проверку", () =>
    {
        // Записано 15:58: Z0..Z3 и Z5..Z8 - расчёт, Z4 - прежнее значение T5.
        var calculated = ColleagueCalculatedReactances();
        float[] resistances = { 1, 5, 20, 50, 100, 428, 814, 1500, 0, 0 };
        float[] reactances =
        {
            calculated[0], calculated[1], calculated[2], calculated[3],
            4.5004f,
            calculated[4], calculated[5], calculated[6], calculated[7],
            float.NaN
        };
        var cells = Enumerable.Range(0, 10)
            .Select(index => float.IsNaN(reactances[index])
                ? new RingLoopParameterCell(index, 0f, 0f, false, RingCellReadStatus.EmptyInMemory)
                : new RingLoopParameterCell(index, resistances[index], reactances[index], true, RingCellReadStatus.Parsed))
            .ToArray();

        var audit = RingMemoryAudit.Evaluate(
            cells,
            8,
            RingCapacitanceMappingProfile.UniversalFiveTransmitter,
            calculated);
        Require(audit.IsComplete && !audit.HasMismatches, $"находок быть не должно: {audit.Fingerprint}");
    }),
    ("Кольцо 241 на восемь ёмкостей: пустые Z8 и Z9 не ошибка", () =>
    {
        var calculated = ColleagueCalculatedReactances();
        var cells = Enumerable.Range(0, 10)
            .Select(index => index < 8
                ? new RingLoopParameterCell(index, 1f + index, calculated[index], true, RingCellReadStatus.Parsed)
                : new RingLoopParameterCell(index, 0f, 0f, false, RingCellReadStatus.EmptyInMemory))
            .ToArray();
        var audit = RingMemoryAudit.Evaluate(
            cells,
            8,
            RingCapacitanceMappingProfile.CompactFourTransmitter,
            calculated);
        Require(audit.IsComplete && !audit.HasMismatches, $"на кольце 241 Z8/Z9 физически нет: {audit.Fingerprint}");
    }),
    ("Пятизондовый прибор на кольце коллег требует и Z8, и Z9", () =>
    {
        var audit = RingMemoryAudit.Evaluate(
            ColleagueRingCells(z8: null),
            10,
            RingCapacitanceMappingProfile.UniversalFiveTransmitter);
        var empty = audit.Blocking.Select(issue => issue.CellIndex).OrderBy(index => index).ToArray();
        Require(empty.SequenceEqual(new[] { 8, 9 }), $"пусты Z8 и Z9, получено [{string.Join(",", empty)}]");
    }),
    ("Нет ответа и пустая ячейка различаются", () =>
    {
        var cells = ColleagueRingCells(z8: null);
        cells[8] = new RingLoopParameterCell(8, 0f, 0f, false, RingCellReadStatus.NoAnswer);
        var audit = RingMemoryAudit.Evaluate(cells, 8, RingCapacitanceMappingProfile.UniversalFiveTransmitter);
        var issue = audit.Blocking.Single();
        Require(issue.Kind == RingCellIssueKind.ReactanceUnreadable, "таймаут - это не пустая память");
        Require(audit.HasUnreadable, "итог знает о сбое связи");
    }),
    ("Нули после ZERASE и пустое сопротивление считаются незаполненными", () =>
    {
        var cells = ColleagueRingCells(z8: 9.842f);
        cells[3] = new RingLoopParameterCell(3, 0f, 0f, true, RingCellReadStatus.Parsed);
        var audit = RingMemoryAudit.Evaluate(cells, 8, RingCapacitanceMappingProfile.UniversalFiveTransmitter);
        var kinds = audit.Blocking
            .Where(issue => issue.CellIndex == 3)
            .Select(issue => issue.Kind)
            .OrderBy(kind => kind)
            .ToArray();
        Require(
            kinds.SequenceEqual(new[] { RingCellIssueKind.ResistanceEmpty, RingCellIssueKind.ReactanceEmpty }),
            "в Z3 нет ни R4, ни реактивности T4 401");
        Require(audit.Blocking.Single(issue => issue.Kind == RingCellIssueKind.ResistanceEmpty).ResistanceIndex == 3, "это R4");
    }),
    ("Кольцо без ответа: ни одной ячейки", () =>
    {
        var audit = RingMemoryAudit.Evaluate(
            Array.Empty<RingLoopParameterCell>(),
            8,
            RingCapacitanceMappingProfile.UniversalFiveTransmitter);
        Require(audit.NothingRead && !audit.IsComplete && audit.HasUnreadable, "пустое чтение - сбой связи");
    }),
    ("Без прибора проверяются только сопротивления", () =>
    {
        var audit = RingMemoryAudit.Evaluate(
            ColleagueRingCells(z8: null),
            0,
            RingCapacitanceMappingProfile.UniversalFiveTransmitter);
        Require(audit.IsComplete, "пустая Z8 без прибора никому не нужна");
    }),
    ("Шум: прогон коллег 23.09 выделяет ровно недостоверные ячейки", () =>
    {
        // Эталоны и шум воздуха - из журнала прогона 16:00 (усреднение 5,
        // воздух 20 кадров, допуск 6 %).
        var references = ColleaguePhaseReferencesMg().Select(ToV10Radians).ToArray();
        var airStdDev = ToV10Radians(new[] { 80.821, 26.892, 41.990, 66.396, 17.142, 17.703, 30.875, 44.957 });
        var cells = MeasurementNoiseFloor.Evaluate(references, in airStdDev, 8, 20, 5, 6, new[] { 5, 10, 15, 20 });

        var flagged = cells.Select(cell => (cell.ResistanceIndex, cell.ChannelIndex)).ToHashSet();
        var expected = new HashSet<(int, int)>
        {
            (5, 3),
            (6, 0), (6, 1), (6, 2), (6, 3),
            (7, 0), (7, 1), (7, 2), (7, 3), (7, 7),
            (8, 0), (8, 1), (8, 2), (8, 3), (8, 6), (8, 7)
        };
        Require(
            flagged.SetEquals(expected),
            $"выделено: {string.Join(" ", flagged.OrderBy(cell => cell).Select(cell => $"R{cell.Item1}:{cell.Item2}"))}");

        var recoverable = cells
            .Where(cell => cell.RequiredAveraging > 0)
            .Select(cell => (cell.ResistanceIndex, cell.ChannelIndex, cell.RequiredAveraging))
            .OrderBy(cell => cell)
            .ToArray();
        Require(
            recoverable.SequenceEqual(new[] { (5, 3, 10), (6, 1, 10), (7, 7, 10), (8, 6, 10) }),
            $"достижимо усреднением: {string.Join(" ", recoverable)}");

        // Систематические отклонения - не шум: R1–R4 не выделяются.
        Require(!flagged.Any(cell => cell.Item1 <= 4), "R1–R4 шумом не объясняются");
    }),
    ("Шум: без воздуха оценка не выполняется", () =>
    {
        var references = ColleaguePhaseReferencesMg().Select(ToV10Radians).ToArray();
        var airStdDev = ToV10Radians(new double[8]);
        Require(
            MeasurementNoiseFloor.Evaluate(references, in airStdDev, 8, 0, 5, 6, new[] { 5, 10, 15, 20 }).Count == 0,
            "воздух не собран");
    }),
    ("Ячейка таблицы не показывает «−0,0 %»", () =>
    {
        // R2 T4 401 прогона 25.09: маленькое отрицательное отклонение
        // округлялось до нуля со знаком.
        var cell = MeasurementCell.Create(-0.04f, 6);
        Require(cell.Value == 0f && !float.IsNegative(cell.Value), "отрицательный ноль нормализуется");
        Require(cell.Value.ToString("F1", System.Globalization.CultureInfo.InvariantCulture) == "0.0", "в тексте нет минуса");
        Require(MeasurementCell.Create(-0.05f, 6).Value == -0.1f, "−0,05 округляется от нуля");
        Require(MeasurementCell.Create(9.24f, 6).Value == 9.2f, "одна цифра после запятой");

        // Цвет считается по тому же числу, что видит пользователь.
        Require(MeasurementCell.Create(6.04f, 6).Quality == MeasurementQuality.InTolerance, "«6,0 %» при допуске 6 % - в норме");
        Require(MeasurementCell.Create(6.06f, 6).Quality == MeasurementQuality.Warning, "«6,1 %» - предупреждение");
    }),
    ("Причина отказа открытия порта кольца распознаётся", () =>
    {
        // ERROR_SEM_TIMEOUT - так 25.09 не открылся COM9.
        var semaphoreTimeout = new IOException("Превышен таймаут семафора. : 'COM9'.", unchecked((int)0x80070079));
        Require(
            RingConnection.ClassifyPortOpenFailure(semaphoreTimeout) == RingPortOpenFailure.DeviceNotResponding,
            "таймаут семафора - устройство не отвечает");
        Require(
            RingConnection.ClassifyPortOpenFailure(new UnauthorizedAccessException()) == RingPortOpenFailure.Busy,
            "отказ в доступе - порт занят");
        Require(
            RingConnection.ClassifyPortOpenFailure(new FileNotFoundException()) == RingPortOpenFailure.NotFound,
            "порта нет");
        Require(
            RingConnection.ClassifyPortOpenFailure(new IOException("other", unchecked((int)0x8007001F))) == RingPortOpenFailure.Other,
            "прочее");
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

// Память кольца 151 коллег до перезаписи 23.09 (фото вкладки «Поверка кольца»).
// z8: null - ячейка стёрта (кольцо отвечает «Z8 -nan -nan»).
static RingLoopParameterCell[] ColleagueRingCells(float? z8)
{
    float[] resistances = { 1, 5, 20, 50, 100, 428, 814, 1500 };
    float[] reactances = { 0.9018f, 1.5395f, 1.8522f, 2.0095f, 4.5004f, 7.6801f, 9.2408f, 10.02f };
    var cells = new List<RingLoopParameterCell>();
    for (var index = 0; index < 8; index++)
    {
        cells.Add(new RingLoopParameterCell(index, resistances[index], reactances[index], true, RingCellReadStatus.Parsed));
    }

    cells.Add(z8 is float value
        ? new RingLoopParameterCell(8, 0f, value, true, RingCellReadStatus.Parsed)
        : new RingLoopParameterCell(8, 0f, 0f, false, RingCellReadStatus.EmptyInMemory));
    cells.Add(new RingLoopParameterCell(9, 0f, 0f, false, RingCellReadStatus.EmptyInMemory));
    return cells.ToArray();
}

// «Реакт. расч.» для LWD 4Tx № 42023 - с того же фото.
static float[] ColleagueCalculatedReactances() =>
    new[] { 0.8891f, 1.5445f, 1.8553f, 2.0071f, 4.4313f, 7.7127f, 9.2592f, 10.0124f };

// Эталонные фазы R0..R8 прогона коллег 23.09 16:00, мГ (журнал, REFERENCE_SET).
static double[][] ColleaguePhaseReferencesMg() => new[]
{
    new double[8],
    new[] { 49894.230, 53487.488, 46654.199, 39670.641, 127138.438, 130701.234, 123598.414, 114677.969 },
    new[] { 13018.803, 14091.959, 12075.805, 10112.518, 57504.180, 61449.004, 53917.355, 46094.973 },
    new[] { 3412.688, 3696.558, 3163.715, 2646.619, 16747.971, 18119.748, 15540.983, 13023.805 },
    new[] { 1377.629, 1492.285, 1277.079, 1068.277, 6839.374, 7407.133, 6341.200, 5305.978 },
    new[] { 690.900, 748.406, 640.469, 535.748, 3439.754, 3725.872, 3188.808, 2667.613 },
    new[] { 161.798, 175.265, 149.988, 125.463, 806.986, 874.154, 748.083, 625.766 },
    new[] { 85.101, 92.185, 78.890, 65.990, 424.552, 459.890, 393.562, 329.212 },
    new[] { 46.189, 50.034, 42.818, 35.817, 230.456, 249.638, 213.634, 178.703 }
};

static V10 ToV10Radians(double[] milligrad)
{
    var result = new V10();
    for (var channel = 0; channel < milligrad.Length; channel++)
    {
        result.SetValue(channel, (float)(milligrad[channel] / PhysicalConstants.Milligrad));
    }

    return result;
}

static void Require(bool condition, string message = "assertion failed")
{
    if (!condition)
    {
        throw new InvalidOperationException(message);
    }
}

// Проверка на незавершаемость: расчёт обязан вернуть управление. Без ограничения
// по времени регрессия «цикл не сходится» повесила бы сам набор проверок вместо
// того, чтобы его провалить.
static void RequireCompletes(Action action, string message, int timeoutSeconds = 10)
{
    var task = Task.Run(action);
    if (!task.Wait(TimeSpan.FromSeconds(timeoutSeconds)))
    {
        throw new InvalidOperationException($"{message}: расчёт не завершился за {timeoutSeconds} с");
    }

    task.GetAwaiter().GetResult();
}
