using System.Globalization;
using System.IO.Compression;
using System.Text;
using System.Xml;
using System.Xml.Linq;
using LoopNext.Core.Models;

namespace LOOP.Next.Reporting;

public sealed record LoopReportCell(float? Value, MeasurementQuality Quality);

public sealed record LoopReportChannel(string Label, float FrequencyKHz, IReadOnlyList<LoopReportCell> Cells);

public sealed record LoopReport(
    string CompanyName,
    string Serial,
    string ToolType,
    string ToolSignature,
    string TransmitterCount,
    string Modification,
    string ToolNumber,
    string RxPosition,
    string RingSignature,
    int ThresholdPercent,
    int AveragingCount,
    DateTime CreatedAt,
    IReadOnlyList<float> LoopResistances,
    IReadOnlyList<int> SelectedResistanceIndexes,
    IReadOnlyList<LoopReportChannel> Channels);

/// <summary>
/// Формирует автономный корпоративный Excel-рапорт без зависимости от установленного Excel.
/// Книга создаётся как стандартный Office Open XML пакет и открывается в Excel/LibreOffice.
/// </summary>
public static class LoopReportExcelBuilder
{
    private const string WorksheetName = "Протокол проверки";

    private const int StyleTitle = 1;
    private const int StyleSubtitle = 2;
    private const int StyleSection = 3;
    private const int StyleLabel = 4;
    private const int StyleValue = 5;
    private const int StyleTableHeader = 6;
    private const int StyleText = 7;
    private const int StyleInteger = 8;
    private const int StyleQualityGood = 9;
    private const int StyleQualityWarning = 10;
    private const int StyleQualityOut = 11;
    private const int StyleQualityCritical = 12;
    private const int StyleSummaryLabel = 13;
    private const int StyleSummaryValue = 14;
    private const int StyleNote = 15;
    private const int StyleDateTime = 16;
    private const int StyleThreshold = 17;
    private const int StyleResistance = 18;
    private const int StyleStatusGood = 19;
    private const int StyleStatusWarning = 20;
    private const int StyleStatusOut = 21;
    private const int StyleStatusCritical = 22;

    private static readonly XNamespace SpreadsheetNs = "http://schemas.openxmlformats.org/spreadsheetml/2006/main";
    private static readonly XNamespace RelationshipsNs = "http://schemas.openxmlformats.org/officeDocument/2006/relationships";
    private static readonly XNamespace PackageRelationshipsNs = "http://schemas.openxmlformats.org/package/2006/relationships";
    private static readonly XNamespace ContentTypesNs = "http://schemas.openxmlformats.org/package/2006/content-types";

    public static byte[] Build(LoopReport report)
    {
        ArgumentNullException.ThrowIfNull(report);
        if (report.SelectedResistanceIndexes.Count == 0)
        {
            throw new ArgumentException("Для Excel-рапорта не выбрана ни одна мера R1..R8.", nameof(report));
        }

        if (report.Channels.Count == 0)
        {
            throw new ArgumentException("Для Excel-рапорта отсутствуют измерительные каналы.", nameof(report));
        }

        using var output = new MemoryStream();
        using (var archive = new ZipArchive(output, ZipArchiveMode.Create, leaveOpen: true))
        {
            var worksheet = CreateWorksheet(report, out var lastColumnName, out var lastRow);
            WriteXmlEntry(archive, "[Content_Types].xml", CreateContentTypes());
            WriteXmlEntry(archive, "_rels/.rels", CreatePackageRelationships());
            WriteXmlEntry(archive, "docProps/app.xml", CreateApplicationProperties());
            WriteXmlEntry(archive, "docProps/core.xml", CreateCoreProperties(report));
            WriteXmlEntry(archive, "xl/workbook.xml", CreateWorkbook(lastColumnName, lastRow));
            WriteXmlEntry(archive, "xl/_rels/workbook.xml.rels", CreateWorkbookRelationships());
            WriteXmlEntry(archive, "xl/styles.xml", CreateStyles());
            WriteXmlEntry(archive, "xl/worksheets/sheet1.xml", worksheet);
        }

        return output.ToArray();
    }

    private static XDocument CreateWorksheet(LoopReport report, out string lastColumnName, out int lastRow)
    {
        var selectedCount = report.SelectedResistanceIndexes.Count;
        var firstResultColumn = 4; // D
        var lastMeasurementColumn = firstResultColumn + selectedCount - 1;
        var statusColumn = lastMeasurementColumn + 1;
        var lastColumn = Math.Max(8, statusColumn); // минимум H для аккуратного паспортного блока
        lastColumnName = ColumnName(lastColumn);
        var worksheetLastColumnName = lastColumnName;

        var rows = new List<XElement>();
        var merges = new List<string>();

        void AddRow(int index, double height, params XElement[] cells)
        {
            rows.Add(new XElement(
                SpreadsheetNs + "row",
                new XAttribute("r", index),
                new XAttribute("ht", height.ToString("0.##", CultureInfo.InvariantCulture)),
                new XAttribute("customHeight", 1),
                cells));
        }

        void AddMerge(string start, string end)
        {
            merges.Add($"{start}:{end}");
        }

        void AddMergedTextRow(int row, string text, int style, double height)
        {
            AddRow(row, height, InlineStringCell($"A{row}", text, style));
            AddMerge($"A{row}", $"{worksheetLastColumnName}{row}");
        }

        void AddInfoRow(
            int row,
            string leftLabel,
            XElement leftValue,
            string rightLabel,
            XElement rightValue)
        {
            AddRow(
                row,
                22,
                InlineStringCell($"A{row}", leftLabel, StyleLabel),
                leftValue,
                InlineStringCell($"E{row}", rightLabel, StyleLabel),
                rightValue);
            AddMerge($"A{row}", $"B{row}");
            AddMerge($"C{row}", $"D{row}");
            AddMerge($"E{row}", $"F{row}");
            AddMerge($"G{row}", $"{worksheetLastColumnName}{row}");
        }

        AddMergedTextRow(1, "ПРОТОКОЛ ПРОВЕРКИ ПРИБОРА", StyleTitle, 32);
        AddMergedTextRow(2, report.CompanyName, StyleSubtitle, 22);

        AddMergedTextRow(4, "СВЕДЕНИЯ О ПРОВЕРКЕ", StyleSection, 23);
        AddInfoRow(
            5,
            "Дата и время",
            NumberCell("C5", report.CreatedAt.ToOADate(), StyleDateTime),
            "Серийный №",
            InlineStringCell("G5", report.Serial, StyleValue));
        AddInfoRow(
            6,
            "Тип прибора",
            InlineStringCell("C6", report.ToolType, StyleValue),
            "Сигнатура прибора",
            InlineStringCell("G6", report.ToolSignature, StyleValue));
        AddInfoRow(
            7,
            "Количество TX",
            InlineStringCell("C7", report.TransmitterCount, StyleValue),
            "Позиция R1",
            InlineStringCell("G7", report.RxPosition, StyleValue));
        AddInfoRow(
            8,
            "Модификация / №",
            InlineStringCell("C8", $"{report.Modification} / {report.ToolNumber}", StyleValue),
            "Сигнатура кольца",
            InlineStringCell("G8", report.RingSignature, StyleValue));
        AddInfoRow(
            9,
            "Порог допуска",
            NumberCell("C9", report.ThresholdPercent, StyleThreshold),
            "Усреднение",
            NumberCell("G9", report.AveragingCount, StyleValue));

        AddMergedTextRow(11, "ПАРАМЕТРЫ МЕР ИМПЕДАНСА", StyleSection, 23);
        var measureHeaderCells = new List<XElement>
        {
            InlineStringCell("A12", "Мера импеданса", StyleTableHeader)
        };
        AddMerge("A12", "C12");
        for (var selectedIndex = 0; selectedIndex < selectedCount; selectedIndex++)
        {
            var resistanceIndex = report.SelectedResistanceIndexes[selectedIndex];
            measureHeaderCells.Add(InlineStringCell(
                $"{ColumnName(firstResultColumn + selectedIndex)}12",
                $"R{resistanceIndex}",
                StyleTableHeader));
        }
        AddRow(12, 24, measureHeaderCells.ToArray());

        var measureValueCells = new List<XElement>
        {
            InlineStringCell("A13", "Сопротивление, Ом", StyleSummaryLabel)
        };
        AddMerge("A13", "C13");
        for (var selectedIndex = 0; selectedIndex < selectedCount; selectedIndex++)
        {
            var resistanceIndex = report.SelectedResistanceIndexes[selectedIndex];
            var resistance = resistanceIndex - 1 < report.LoopResistances.Count
                ? report.LoopResistances[resistanceIndex - 1]
                : 0f;
            measureValueCells.Add(NumberCell(
                $"{ColumnName(firstResultColumn + selectedIndex)}13",
                resistance,
                StyleResistance));
        }
        AddRow(13, 22, measureValueCells.ToArray());

        AddMergedTextRow(15, "РЕЗУЛЬТАТЫ ПРОВЕРКИ — ОТКЛОНЕНИЕ ФАЗЫ, %", StyleSection, 23);
        var resultHeaderCells = new List<XElement>
        {
            InlineStringCell("A16", "№", StyleTableHeader),
            InlineStringCell("B16", "Канал", StyleTableHeader),
            InlineStringCell("C16", "Частота, кГц", StyleTableHeader)
        };
        for (var selectedIndex = 0; selectedIndex < selectedCount; selectedIndex++)
        {
            var resistanceIndex = report.SelectedResistanceIndexes[selectedIndex];
            resultHeaderCells.Add(InlineStringCell(
                $"{ColumnName(firstResultColumn + selectedIndex)}16",
                $"R{resistanceIndex}",
                StyleTableHeader));
        }
        resultHeaderCells.Add(InlineStringCell($"{ColumnName(statusColumn)}16", "Оценка канала", StyleTableHeader));
        AddRow(16, 30, resultHeaderCells.ToArray());

        var channelStatuses = new List<MeasurementQuality>(report.Channels.Count);
        var channelStatusLabels = new List<string>(report.Channels.Count);
        var firstDataRow = 17;
        for (var channelIndex = 0; channelIndex < report.Channels.Count; channelIndex++)
        {
            var rowIndex = firstDataRow + channelIndex;
            var channel = report.Channels[channelIndex];
            var dataCells = new List<XElement>
            {
                NumberCell($"A{rowIndex}", channelIndex + 1, StyleInteger),
                InlineStringCell($"B{rowIndex}", channel.Label, StyleText),
                NumberCell($"C{rowIndex}", channel.FrequencyKHz, StyleInteger)
            };

            var availableQualities = new List<MeasurementQuality>();
            for (var selectedIndex = 0; selectedIndex < selectedCount; selectedIndex++)
            {
                var cell = selectedIndex < channel.Cells.Count
                    ? channel.Cells[selectedIndex]
                    : new LoopReportCell(null, MeasurementQuality.Critical);
                var address = $"{ColumnName(firstResultColumn + selectedIndex)}{rowIndex}";
                if (cell.Value is float value && float.IsFinite(value))
                {
                    availableQualities.Add(cell.Quality);
                    dataCells.Add(NumberCell(address, value, QualityStyle(cell.Quality)));
                }
                else
                {
                    dataCells.Add(InlineStringCell(address, "—", StyleText));
                }
            }

            var rowQuality = availableQualities.Count == 0
                ? MeasurementQuality.Critical
                : availableQualities.Max();
            channelStatuses.Add(rowQuality);
            var cachedStatus = availableQualities.Count == 0 ? "Нет данных" : QualityLabel(rowQuality);
            channelStatusLabels.Add(cachedStatus);
            var measurementRange =
                $"{ColumnName(firstResultColumn)}{rowIndex}:{ColumnName(lastMeasurementColumn)}{rowIndex}";
            var statusFormula =
                $"IF(COUNTA({measurementRange})=0,\"Нет данных\"," +
                $"IF(MAX(MAX({measurementRange}),-MIN({measurementRange}))<=$C$9,\"В допуске\"," +
                $"IF(MAX(MAX({measurementRange}),-MIN({measurementRange}))<=2*$C$9,\"Предупреждение\"," +
                $"IF(MAX(MAX({measurementRange}),-MIN({measurementRange}))<=3*$C$9,\"За допуском\",\"Критично\"))))";
            dataCells.Add(FormulaCell(
                $"{ColumnName(statusColumn)}{rowIndex}",
                statusFormula,
                cachedStatus,
                availableQualities.Count == 0 ? StyleStatusOut : StatusStyle(rowQuality),
                cachedIsString: true));
            AddRow(rowIndex, 22, dataCells.ToArray());
        }

        var lastDataRow = firstDataRow + report.Channels.Count - 1;
        var statusRange = $"${ColumnName(statusColumn)}${firstDataRow}:${ColumnName(statusColumn)}${lastDataRow}";
        var summarySectionRow = lastDataRow + 2;
        AddMergedTextRow(summarySectionRow, "ИТОГОВАЯ ОЦЕНКА", StyleSection, 23);

        var summaryDefinitions = new[]
        {
            (Label: "Каналов в допуске", Status: "В допуске", Quality: MeasurementQuality.InTolerance),
            (Label: "Каналов с предупреждением", Status: "Предупреждение", Quality: MeasurementQuality.Warning),
            (Label: "Каналов за допуском", Status: "За допуском", Quality: MeasurementQuality.OutOfTolerance),
            (Label: "Критичных каналов", Status: "Критично", Quality: MeasurementQuality.Critical),
            (Label: "Каналов без данных", Status: "Нет данных", Quality: MeasurementQuality.Critical)
        };
        var statusCounts = summaryDefinitions.ToDictionary(
            definition => definition.Status,
            definition => channelStatusLabels.Count(label => label == definition.Status));

        for (var index = 0; index < summaryDefinitions.Length; index++)
        {
            var row = summarySectionRow + 1 + index;
            var definition = summaryDefinitions[index];
            AddRow(
                row,
                21,
                InlineStringCell($"A{row}", definition.Label, StyleSummaryLabel),
                FormulaCell(
                    $"D{row}",
                    $"COUNTIF({statusRange},\"{definition.Status}\")",
                    statusCounts[definition.Status].ToString(CultureInfo.InvariantCulture),
                    StyleSummaryValue));
            AddMerge($"A{row}", $"C{row}");
        }

        var warningCountRow = summarySectionRow + 2;
        var outCountRow = summarySectionRow + 3;
        var criticalCountRow = summarySectionRow + 4;
        var missingCountRow = summarySectionRow + 5;
        var overallRow = summarySectionRow + 7;
        var hasMissingData = channelStatusLabels.Any(label => label == "Нет данных");
        var overallQuality = hasMissingData
            ? MeasurementQuality.Critical
            : channelStatuses.Any(quality => quality is MeasurementQuality.OutOfTolerance or MeasurementQuality.Critical)
            ? MeasurementQuality.OutOfTolerance
            : channelStatuses.Any(quality => quality == MeasurementQuality.Warning)
                ? MeasurementQuality.Warning
                : MeasurementQuality.InTolerance;
        var overallText = hasMissingData
            ? "НЕПОЛНЫЕ ДАННЫЕ"
            : overallQuality switch
        {
            MeasurementQuality.InTolerance => "СООТВЕТСТВУЕТ",
            MeasurementQuality.Warning => "ТРЕБУЕТ ВНИМАНИЯ",
            _ => "НЕ СООТВЕТСТВУЕТ"
        };
        var overallFormula =
            $"IF(D{missingCountRow}>0,\"НЕПОЛНЫЕ ДАННЫЕ\"," +
            $"IF(D{outCountRow}+D{criticalCountRow}>0,\"НЕ СООТВЕТСТВУЕТ\"," +
            $"IF(D{warningCountRow}>0,\"ТРЕБУЕТ ВНИМАНИЯ\",\"СООТВЕТСТВУЕТ\")))";
        AddRow(
            overallRow,
            28,
            InlineStringCell($"A{overallRow}", "Итог", StyleSummaryLabel),
            FormulaCell(
                $"D{overallRow}",
                overallFormula,
                overallText,
                StatusStyle(overallQuality),
                cachedIsString: true));
        AddMerge($"A{overallRow}", $"C{overallRow}");
        AddMerge($"D{overallRow}", $"{lastColumnName}{overallRow}");

        var legendSectionRow = overallRow + 2;
        AddMergedTextRow(legendSectionRow, "ЛЕГЕНДА", StyleSection, 23);
        var legend = new[]
        {
            ("В допуске", $"|отклонение| ≤ {report.ThresholdPercent}%", MeasurementQuality.InTolerance),
            ("Предупреждение", $"{report.ThresholdPercent}% < |отклонение| ≤ {2 * report.ThresholdPercent}%", MeasurementQuality.Warning),
            ("За допуском", $"{2 * report.ThresholdPercent}% < |отклонение| ≤ {3 * report.ThresholdPercent}%", MeasurementQuality.OutOfTolerance),
            ("Критично", $"|отклонение| > {3 * report.ThresholdPercent}%", MeasurementQuality.Critical)
        };
        for (var index = 0; index < legend.Length; index++)
        {
            var row = legendSectionRow + 1 + index;
            AddRow(
                row,
                20,
                InlineStringCell($"A{row}", legend[index].Item1, QualityStyle(legend[index].Item3)),
                InlineStringCell($"C{row}", legend[index].Item2, StyleValue));
            AddMerge($"A{row}", $"B{row}");
            AddMerge($"C{row}", $"{lastColumnName}{row}");
        }

        var signatureSectionRow = legendSectionRow + 6;
        AddMergedTextRow(signatureSectionRow, "ПОДТВЕРЖДЕНИЕ ПРОВЕРКИ", StyleSection, 23);
        AddRow(
            signatureSectionRow + 1,
            26,
            InlineStringCell($"A{signatureSectionRow + 1}", "Проверку провёл: ______________________________", StyleValue),
            InlineStringCell($"E{signatureSectionRow + 1}", "Подпись: ____________________", StyleValue));
        AddMerge($"A{signatureSectionRow + 1}", $"D{signatureSectionRow + 1}");
        AddMerge($"E{signatureSectionRow + 1}", $"{lastColumnName}{signatureSectionRow + 1}");
        AddRow(
            signatureSectionRow + 2,
            26,
            InlineStringCell($"A{signatureSectionRow + 2}", "Должность: ___________________________________", StyleValue),
            InlineStringCell($"E{signatureSectionRow + 2}", "Дата: ______________________", StyleValue));
        AddMerge($"A{signatureSectionRow + 2}", $"D{signatureSectionRow + 2}");
        AddMerge($"E{signatureSectionRow + 2}", $"{lastColumnName}{signatureSectionRow + 2}");
        AddRow(
            signatureSectionRow + 4,
            28,
            InlineStringCell(
                $"A{signatureSectionRow + 4}",
                "Документ сформирован приложением «Кольца». Измерительные значения приведены в процентах фазового отклонения от расчётного эталона.",
                StyleNote));
        AddMerge($"A{signatureSectionRow + 4}", $"{lastColumnName}{signatureSectionRow + 4}");
        lastRow = signatureSectionRow + 4;

        var columns = new List<XElement>
        {
            ColumnDefinition(1, 1, 6),
            ColumnDefinition(2, 2, 19),
            ColumnDefinition(3, 3, 16)
        };
        if (selectedCount > 0)
        {
            columns.Add(ColumnDefinition(firstResultColumn, lastMeasurementColumn, 12));
        }
        columns.Add(ColumnDefinition(statusColumn, statusColumn, 22));
        if (lastColumn > statusColumn)
        {
            columns.Add(ColumnDefinition(statusColumn + 1, lastColumn, 13));
        }

        var worksheet = new XElement(
            SpreadsheetNs + "worksheet",
            new XAttribute(XNamespace.Xmlns + "r", RelationshipsNs),
            new XElement(
                SpreadsheetNs + "sheetPr",
                new XElement(SpreadsheetNs + "pageSetUpPr", new XAttribute("fitToPage", 1))),
            new XElement(SpreadsheetNs + "dimension", new XAttribute("ref", $"A1:{lastColumnName}{lastRow}")),
            new XElement(
                SpreadsheetNs + "sheetViews",
                new XElement(
                    SpreadsheetNs + "sheetView",
                    new XAttribute("workbookViewId", 0),
                    new XAttribute("showGridLines", 0),
                    new XAttribute("zoomScale", 90))),
            new XElement(SpreadsheetNs + "sheetFormatPr", new XAttribute("defaultRowHeight", 18)),
            new XElement(SpreadsheetNs + "cols", columns),
            new XElement(SpreadsheetNs + "sheetData", rows),
            new XElement(
                SpreadsheetNs + "autoFilter",
                new XAttribute("ref", $"A16:{ColumnName(statusColumn)}{lastDataRow}")),
            new XElement(
                SpreadsheetNs + "mergeCells",
                new XAttribute("count", merges.Count),
                merges.Select(range => new XElement(SpreadsheetNs + "mergeCell", new XAttribute("ref", range)))),
            new XElement(
                SpreadsheetNs + "pageMargins",
                new XAttribute("left", 0.3),
                new XAttribute("right", 0.3),
                new XAttribute("top", 0.5),
                new XAttribute("bottom", 0.5),
                new XAttribute("header", 0.2),
                new XAttribute("footer", 0.2)),
            new XElement(
                SpreadsheetNs + "pageSetup",
                new XAttribute("paperSize", 9),
                new XAttribute("orientation", "landscape"),
                new XAttribute("fitToWidth", 1),
                new XAttribute("fitToHeight", 0),
                new XAttribute("horizontalDpi", 300),
                new XAttribute("verticalDpi", 300)),
            new XElement(
                SpreadsheetNs + "headerFooter",
                new XElement(SpreadsheetNs + "oddFooter", $"&L{report.CompanyName}&RСтраница &P из &N")));

        return new XDocument(new XDeclaration("1.0", "UTF-8", "yes"), worksheet);
    }

    private static XDocument CreateContentTypes() => new(
        new XDeclaration("1.0", "UTF-8", "yes"),
        new XElement(
            ContentTypesNs + "Types",
            new XElement(ContentTypesNs + "Default", new XAttribute("Extension", "rels"), new XAttribute("ContentType", "application/vnd.openxmlformats-package.relationships+xml")),
            new XElement(ContentTypesNs + "Default", new XAttribute("Extension", "xml"), new XAttribute("ContentType", "application/xml")),
            new XElement(ContentTypesNs + "Override", new XAttribute("PartName", "/xl/workbook.xml"), new XAttribute("ContentType", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml")),
            new XElement(ContentTypesNs + "Override", new XAttribute("PartName", "/xl/worksheets/sheet1.xml"), new XAttribute("ContentType", "application/vnd.openxmlformats-officedocument.spreadsheetml.worksheet+xml")),
            new XElement(ContentTypesNs + "Override", new XAttribute("PartName", "/xl/styles.xml"), new XAttribute("ContentType", "application/vnd.openxmlformats-officedocument.spreadsheetml.styles+xml")),
            new XElement(ContentTypesNs + "Override", new XAttribute("PartName", "/docProps/core.xml"), new XAttribute("ContentType", "application/vnd.openxmlformats-package.core-properties+xml")),
            new XElement(ContentTypesNs + "Override", new XAttribute("PartName", "/docProps/app.xml"), new XAttribute("ContentType", "application/vnd.openxmlformats-officedocument.extended-properties+xml"))));

    private static XDocument CreatePackageRelationships() => new(
        new XDeclaration("1.0", "UTF-8", "yes"),
        new XElement(
            PackageRelationshipsNs + "Relationships",
            Relationship("rId1", "http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument", "xl/workbook.xml"),
            Relationship("rId2", "http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties", "docProps/core.xml"),
            Relationship("rId3", "http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties", "docProps/app.xml")));

    private static XDocument CreateWorkbook(string lastColumnName, int lastRow) => new(
        new XDeclaration("1.0", "UTF-8", "yes"),
        new XElement(
            SpreadsheetNs + "workbook",
            new XAttribute(XNamespace.Xmlns + "r", RelationshipsNs),
            new XElement(SpreadsheetNs + "bookViews", new XElement(SpreadsheetNs + "workbookView", new XAttribute("activeTab", 0))),
            new XElement(
                SpreadsheetNs + "sheets",
                new XElement(
                    SpreadsheetNs + "sheet",
                    new XAttribute("name", WorksheetName),
                    new XAttribute("sheetId", 1),
                    new XAttribute(RelationshipsNs + "id", "rId1"))),
            new XElement(
                SpreadsheetNs + "definedNames",
                new XElement(
                    SpreadsheetNs + "definedName",
                    new XAttribute("name", "_xlnm.Print_Area"),
                    new XAttribute("localSheetId", 0),
                    $"'{WorksheetName}'!$A$1:${lastColumnName}${lastRow}"),
                new XElement(
                    SpreadsheetNs + "definedName",
                    new XAttribute("name", "_xlnm.Print_Titles"),
                    new XAttribute("localSheetId", 0),
                    $"'{WorksheetName}'!$16:$16")),
            new XElement(
                SpreadsheetNs + "calcPr",
                new XAttribute("calcId", 191029),
                new XAttribute("calcMode", "auto"),
                new XAttribute("fullCalcOnLoad", 1),
                new XAttribute("forceFullCalc", 1))));

    private static XDocument CreateWorkbookRelationships() => new(
        new XDeclaration("1.0", "UTF-8", "yes"),
        new XElement(
            PackageRelationshipsNs + "Relationships",
            Relationship("rId1", "http://schemas.openxmlformats.org/officeDocument/2006/relationships/worksheet", "worksheets/sheet1.xml"),
            Relationship("rId2", "http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles", "styles.xml")));

    private static XDocument CreateApplicationProperties()
    {
        XNamespace propertiesNs = "http://schemas.openxmlformats.org/officeDocument/2006/extended-properties";
        XNamespace vtNs = "http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes";
        return new XDocument(
            new XDeclaration("1.0", "UTF-8", "yes"),
            new XElement(
                propertiesNs + "Properties",
                new XAttribute(XNamespace.Xmlns + "vt", vtNs),
                new XElement(propertiesNs + "Application", "Кольца"),
                new XElement(propertiesNs + "DocSecurity", 0),
                new XElement(propertiesNs + "ScaleCrop", false),
                new XElement(
                    propertiesNs + "HeadingPairs",
                    new XElement(
                        vtNs + "vector",
                        new XAttribute("size", 2),
                        new XAttribute("baseType", "variant"),
                        new XElement(vtNs + "variant", new XElement(vtNs + "lpstr", "Листы")),
                        new XElement(vtNs + "variant", new XElement(vtNs + "i4", 1)))),
                new XElement(
                    propertiesNs + "TitlesOfParts",
                    new XElement(
                        vtNs + "vector",
                        new XAttribute("size", 1),
                        new XAttribute("baseType", "lpstr"),
                        new XElement(vtNs + "lpstr", WorksheetName))),
                new XElement(propertiesNs + "Company", "ООО НПФ «АМК ГОРИЗОНТ»"),
                new XElement(propertiesNs + "AppVersion", "1.0")));
    }

    private static XDocument CreateCoreProperties(LoopReport report)
    {
        XNamespace coreNs = "http://schemas.openxmlformats.org/package/2006/metadata/core-properties";
        XNamespace dcNs = "http://purl.org/dc/elements/1.1/";
        XNamespace dctermsNs = "http://purl.org/dc/terms/";
        XNamespace xsiNs = "http://www.w3.org/2001/XMLSchema-instance";
        var timestamp = report.CreatedAt.ToUniversalTime().ToString("yyyy-MM-dd'T'HH:mm:ss'Z'", CultureInfo.InvariantCulture);
        return new XDocument(
            new XDeclaration("1.0", "UTF-8", "yes"),
            new XElement(
                coreNs + "coreProperties",
                new XAttribute(XNamespace.Xmlns + "dc", dcNs),
                new XAttribute(XNamespace.Xmlns + "dcterms", dctermsNs),
                new XAttribute(XNamespace.Xmlns + "xsi", xsiNs),
                new XElement(dcNs + "title", $"Протокол проверки прибора № {report.Serial}"),
                new XElement(dcNs + "creator", report.CompanyName),
                new XElement(dcNs + "subject", "Проверка прибора с использованием RLC-кольца"),
                new XElement(dcNs + "description", "Корпоративный Excel-рапорт приложения «Кольца»"),
                new XElement(dctermsNs + "created", new XAttribute(xsiNs + "type", "dcterms:W3CDTF"), timestamp),
                new XElement(dctermsNs + "modified", new XAttribute(xsiNs + "type", "dcterms:W3CDTF"), timestamp)));
    }

    private static XDocument CreateStyles()
    {
        XElement Font(string name, double size, string color, bool bold = false, bool italic = false) =>
            new(
                SpreadsheetNs + "font",
                bold ? new XElement(SpreadsheetNs + "b") : null,
                italic ? new XElement(SpreadsheetNs + "i") : null,
                new XElement(SpreadsheetNs + "sz", new XAttribute("val", size.ToString(CultureInfo.InvariantCulture))),
                new XElement(SpreadsheetNs + "color", new XAttribute("rgb", color)),
                new XElement(SpreadsheetNs + "name", new XAttribute("val", name)),
                new XElement(SpreadsheetNs + "family", new XAttribute("val", 2)));

        XElement Fill(string color) =>
            new(
                SpreadsheetNs + "fill",
                new XElement(
                    SpreadsheetNs + "patternFill",
                    new XAttribute("patternType", "solid"),
                    new XElement(SpreadsheetNs + "fgColor", new XAttribute("rgb", color)),
                    new XElement(SpreadsheetNs + "bgColor", new XAttribute("indexed", 64))));

        XElement CellXf(
            int fontId,
            int fillId,
            int borderId,
            int numFmtId = 0,
            string horizontal = "left",
            bool wrap = false) =>
            new(
                SpreadsheetNs + "xf",
                new XAttribute("numFmtId", numFmtId),
                new XAttribute("fontId", fontId),
                new XAttribute("fillId", fillId),
                new XAttribute("borderId", borderId),
                new XAttribute("xfId", 0),
                new XAttribute("applyFont", 1),
                new XAttribute("applyFill", 1),
                new XAttribute("applyBorder", 1),
                numFmtId == 0 ? null : new XAttribute("applyNumberFormat", 1),
                new XAttribute("applyAlignment", 1),
                new XElement(
                    SpreadsheetNs + "alignment",
                    new XAttribute("horizontal", horizontal),
                    new XAttribute("vertical", "center"),
                    wrap ? new XAttribute("wrapText", 1) : null));

        var fonts = new XElement(
            SpreadsheetNs + "fonts",
            new XAttribute("count", 6),
            Font("Segoe UI", 10, "FF1F2937"),
            Font("Segoe UI", 16, "FFFFFFFF", bold: true),
            Font("Segoe UI", 10, "FFFFFFFF", bold: true),
            Font("Segoe UI", 10, "FF1F2937", bold: true),
            Font("Segoe UI", 10, "FFFFFFFF", bold: true),
            Font("Segoe UI", 9, "FF64748B", italic: true));

        var fills = new XElement(
            SpreadsheetNs + "fills",
            new XAttribute("count", 11),
            new XElement(SpreadsheetNs + "fill", new XElement(SpreadsheetNs + "patternFill", new XAttribute("patternType", "none"))),
            new XElement(SpreadsheetNs + "fill", new XElement(SpreadsheetNs + "patternFill", new XAttribute("patternType", "gray125"))),
            Fill("FF17365D"),
            Fill("FF2F75B5"),
            Fill("FFD9EAF7"),
            Fill("FFF3F6F9"),
            Fill("FFE2F0D9"),
            Fill("FFFFF2CC"),
            Fill("FFF4CCCC"),
            Fill("FFC00000"),
            Fill("FFEAF2F8"));

        var borders = new XElement(
            SpreadsheetNs + "borders",
            new XAttribute("count", 3),
            new XElement(
                SpreadsheetNs + "border",
                new XElement(SpreadsheetNs + "left"),
                new XElement(SpreadsheetNs + "right"),
                new XElement(SpreadsheetNs + "top"),
                new XElement(SpreadsheetNs + "bottom"),
                new XElement(SpreadsheetNs + "diagonal")),
            new XElement(
                SpreadsheetNs + "border",
                new XElement(SpreadsheetNs + "left", new XAttribute("style", "thin"), new XElement(SpreadsheetNs + "color", new XAttribute("rgb", "FFD5DEE8"))),
                new XElement(SpreadsheetNs + "right", new XAttribute("style", "thin"), new XElement(SpreadsheetNs + "color", new XAttribute("rgb", "FFD5DEE8"))),
                new XElement(SpreadsheetNs + "top", new XAttribute("style", "thin"), new XElement(SpreadsheetNs + "color", new XAttribute("rgb", "FFD5DEE8"))),
                new XElement(SpreadsheetNs + "bottom", new XAttribute("style", "thin"), new XElement(SpreadsheetNs + "color", new XAttribute("rgb", "FFD5DEE8"))),
                new XElement(SpreadsheetNs + "diagonal")),
            new XElement(
                SpreadsheetNs + "border",
                new XElement(SpreadsheetNs + "left"),
                new XElement(SpreadsheetNs + "right"),
                new XElement(SpreadsheetNs + "top"),
                new XElement(SpreadsheetNs + "bottom", new XAttribute("style", "medium"), new XElement(SpreadsheetNs + "color", new XAttribute("rgb", "FF2F75B5"))),
                new XElement(SpreadsheetNs + "diagonal")));

        var cellFormats = new XElement(
            SpreadsheetNs + "cellXfs",
            new XAttribute("count", 23),
            CellXf(0, 0, 0),
            CellXf(1, 2, 0, horizontal: "center"),
            CellXf(2, 2, 0, horizontal: "center"),
            CellXf(2, 3, 0),
            CellXf(3, 5, 1),
            CellXf(0, 0, 1),
            CellXf(2, 3, 1, horizontal: "center", wrap: true),
            CellXf(0, 0, 1),
            CellXf(0, 0, 1, 1, "center"),
            CellXf(3, 6, 1, 164, "center"),
            CellXf(3, 7, 1, 164, "center"),
            CellXf(3, 8, 1, 164, "center"),
            CellXf(4, 9, 1, 164, "center"),
            CellXf(3, 4, 1),
            CellXf(3, 10, 1, 1, "center"),
            CellXf(5, 0, 0, horizontal: "left", wrap: true),
            CellXf(0, 0, 1, 167),
            CellXf(0, 0, 1, 165, "left"),
            CellXf(0, 0, 1, 1, "center"),
            CellXf(3, 6, 1, horizontal: "center", wrap: true),
            CellXf(3, 7, 1, horizontal: "center", wrap: true),
            CellXf(3, 8, 1, horizontal: "center", wrap: true),
            CellXf(4, 9, 1, horizontal: "center", wrap: true));

        return new XDocument(
            new XDeclaration("1.0", "UTF-8", "yes"),
            new XElement(
                SpreadsheetNs + "styleSheet",
                new XElement(
                    SpreadsheetNs + "numFmts",
                    new XAttribute("count", 4),
                    new XElement(SpreadsheetNs + "numFmt", new XAttribute("numFmtId", 164), new XAttribute("formatCode", "0.0\"%\"")),
                    new XElement(SpreadsheetNs + "numFmt", new XAttribute("numFmtId", 165), new XAttribute("formatCode", "0\"%\"")),
                    new XElement(SpreadsheetNs + "numFmt", new XAttribute("numFmtId", 166), new XAttribute("formatCode", "0.###")),
                    new XElement(SpreadsheetNs + "numFmt", new XAttribute("numFmtId", 167), new XAttribute("formatCode", "dd.mm.yyyy hh:mm"))),
                fonts,
                fills,
                borders,
                new XElement(SpreadsheetNs + "cellStyleXfs", new XAttribute("count", 1), new XElement(SpreadsheetNs + "xf", new XAttribute("numFmtId", 0), new XAttribute("fontId", 0), new XAttribute("fillId", 0), new XAttribute("borderId", 0))),
                cellFormats,
                new XElement(SpreadsheetNs + "cellStyles", new XAttribute("count", 1), new XElement(SpreadsheetNs + "cellStyle", new XAttribute("name", "Обычный"), new XAttribute("xfId", 0), new XAttribute("builtinId", 0))),
                new XElement(SpreadsheetNs + "dxfs", new XAttribute("count", 0)),
                new XElement(SpreadsheetNs + "tableStyles", new XAttribute("count", 0), new XAttribute("defaultTableStyle", "TableStyleMedium2"), new XAttribute("defaultPivotStyle", "PivotStyleLight16"))));
    }

    private static XElement Relationship(string id, string type, string target) =>
        new(
            PackageRelationshipsNs + "Relationship",
            new XAttribute("Id", id),
            new XAttribute("Type", type),
            new XAttribute("Target", target));

    private static XElement ColumnDefinition(int min, int max, double width) =>
        new(
            SpreadsheetNs + "col",
            new XAttribute("min", min),
            new XAttribute("max", max),
            new XAttribute("width", width.ToString("0.##", CultureInfo.InvariantCulture)),
            new XAttribute("customWidth", 1));

    private static XElement InlineStringCell(string reference, string? value, int style)
    {
        var text = new XElement(SpreadsheetNs + "t", value ?? string.Empty);
        if (!string.IsNullOrEmpty(value) &&
            (char.IsWhiteSpace(value[0]) || char.IsWhiteSpace(value[^1])))
        {
            text.SetAttributeValue(XNamespace.Xml + "space", "preserve");
        }

        return new XElement(
            SpreadsheetNs + "c",
            new XAttribute("r", reference),
            new XAttribute("s", style),
            new XAttribute("t", "inlineStr"),
            new XElement(SpreadsheetNs + "is", text));
    }

    private static XElement NumberCell(string reference, double value, int style) =>
        new(
            SpreadsheetNs + "c",
            new XAttribute("r", reference),
            new XAttribute("s", style),
            new XElement(SpreadsheetNs + "v", value.ToString("G17", CultureInfo.InvariantCulture)));

    private static XElement FormulaCell(
        string reference,
        string formula,
        string cachedValue,
        int style,
        bool cachedIsString = false) =>
        new(
            SpreadsheetNs + "c",
            new XAttribute("r", reference),
            new XAttribute("s", style),
            cachedIsString ? new XAttribute("t", "str") : null,
            new XElement(SpreadsheetNs + "f", formula),
            new XElement(SpreadsheetNs + "v", cachedValue));

    private static int QualityStyle(MeasurementQuality quality) => quality switch
    {
        MeasurementQuality.InTolerance => StyleQualityGood,
        MeasurementQuality.Warning => StyleQualityWarning,
        MeasurementQuality.OutOfTolerance => StyleQualityOut,
        MeasurementQuality.Critical => StyleQualityCritical,
        _ => StyleQualityCritical
    };

    private static int StatusStyle(MeasurementQuality quality) => quality switch
    {
        MeasurementQuality.InTolerance => StyleStatusGood,
        MeasurementQuality.Warning => StyleStatusWarning,
        MeasurementQuality.OutOfTolerance => StyleStatusOut,
        MeasurementQuality.Critical => StyleStatusCritical,
        _ => StyleStatusCritical
    };

    private static string QualityLabel(MeasurementQuality quality) => quality switch
    {
        MeasurementQuality.InTolerance => "В допуске",
        MeasurementQuality.Warning => "Предупреждение",
        MeasurementQuality.OutOfTolerance => "За допуском",
        MeasurementQuality.Critical => "Критично",
        _ => "Критично"
    };

    private static string ColumnName(int oneBasedIndex)
    {
        var value = oneBasedIndex;
        var result = string.Empty;
        while (value > 0)
        {
            value--;
            result = (char)('A' + value % 26) + result;
            value /= 26;
        }

        return result;
    }

    private static void WriteXmlEntry(ZipArchive archive, string path, XDocument document)
    {
        var entry = archive.CreateEntry(path, CompressionLevel.Optimal);
        using var stream = entry.Open();
        using var writer = XmlWriter.Create(
            stream,
            new XmlWriterSettings
            {
                Encoding = new UTF8Encoding(encoderShouldEmitUTF8Identifier: false),
                Indent = false,
                CloseOutput = false
            });
        document.Save(writer);
    }
}
