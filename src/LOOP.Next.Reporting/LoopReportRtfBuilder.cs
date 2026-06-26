using System.Globalization;
using System.Text;
using LoopNext.Core.Models;

namespace LOOP.Next.Reporting;

public sealed record LoopReportCell(string Text, MeasurementQuality Quality);

public sealed record LoopReportChannel(string Label, float FrequencyKHz, IReadOnlyList<LoopReportCell> Cells);

public sealed record LoopReport(
    string Serial,
    DateTime CreatedAt,
    IReadOnlyList<float> LoopResistances,
    IReadOnlyList<LoopReportChannel> Channels);

public static class LoopReportRtfBuilder
{
    private static readonly CultureInfo RuCulture = CultureInfo.GetCultureInfo("ru-RU");

    public static string Build(LoopReport report)
    {
        var builder = new StringBuilder();
        builder.Append(@"{\rtf1\ansi\ansicpg1251\deff0");
        builder.Append(@"{\fonttbl{\f0 Microsoft Sans Serif;}}");
        builder.Append(@"{\colortbl ;\red0\green128\blue0;\red255\green204\blue0;\red255\green0\blue0;\red139\green0\blue0;\red0\green0\blue0;}");
        builder.AppendLine();
        builder.Append(@"\f0\fs40\b\cf3 ");
        builder.Append(Escape($"Протокол проверки прибора ЭМК-90 № {report.Serial}"));
        builder.AppendLine(@"\b0\cf5\fs20\par");
        builder.AppendLine(@"\par");
        builder.Append(Escape("Расчет НЕсимметризованных фаз и УЭС на компьютере"));
        builder.AppendLine(@"\par");
        builder.AppendLine(@"\par");
        builder.Append(Escape("ЗОНД        Погрешность %"));
        builder.AppendLine(@"\par");

        for (var resistanceIndex = 1; resistanceIndex <= MeasurementTable.ResistanceCount; resistanceIndex++)
        {
            var resistance = resistanceIndex - 1 < report.LoopResistances.Count
                ? report.LoopResistances[resistanceIndex - 1].ToString("F0", CultureInfo.InvariantCulture)
                : string.Empty;

            builder.AppendLine(@"\par");
            builder.Append(Escape($"Мера импеданса № {resistanceIndex}  {resistance} Ом"));
            builder.AppendLine(@"\par");

            var hasAnyValue = false;
            foreach (var channel in report.Channels)
            {
                var cell = channel.Cells.Count >= resistanceIndex
                    ? channel.Cells[resistanceIndex - 1]
                    : new LoopReportCell(string.Empty, MeasurementQuality.InTolerance);
                if (string.IsNullOrWhiteSpace(cell.Text))
                {
                    continue;
                }

                hasAnyValue = true;
                builder.Append(@"\cf");
                builder.Append(ColorIndex(cell.Quality).ToString(CultureInfo.InvariantCulture));
                builder.Append(' ');
                builder.Append(Escape($"{channel.Label}   {channel.FrequencyKHz:F0}kGz            {cell.Text}"));
                builder.AppendLine(@"\cf5\par");
            }

            if (!hasAnyValue)
            {
                builder.Append(Escape("Нет данных"));
                builder.AppendLine(@"\par");
            }
        }

        builder.AppendLine(@"\par\par\par");
        builder.Append(Escape("Заключение ____________________________________________________________________________"));
        builder.AppendLine(@"\par");
        builder.Append(Escape(" ______________________________________________________________________________________"));
        builder.AppendLine(@"\par");
        builder.Append(Escape(" ______________________________________________________________________________________"));
        builder.AppendLine(@"\par");
        builder.Append(Escape(" ______________________________________________________________________________________"));
        builder.AppendLine(@"\par");
        builder.Append(Escape("Ф И О, должность, подпись______________________________________________________"));
        builder.Append(Escape(report.CreatedAt.ToString("d MMMM yyyy г. H :mm", RuCulture)));
        builder.AppendLine(@"\par}");

        return builder.ToString();
    }

    private static int ColorIndex(MeasurementQuality quality) =>
        quality switch
        {
            MeasurementQuality.InTolerance => 1,
            MeasurementQuality.Warning => 2,
            MeasurementQuality.OutOfTolerance => 3,
            MeasurementQuality.Critical => 4,
            _ => 5
        };

    private static string Escape(string value)
    {
        var builder = new StringBuilder(value.Length);
        foreach (var ch in value)
        {
            switch (ch)
            {
                case '\\':
                    builder.Append(@"\\");
                    break;
                case '{':
                    builder.Append(@"\{");
                    break;
                case '}':
                    builder.Append(@"\}");
                    break;
                default:
                    if (ch <= 0x7f)
                    {
                        builder.Append(ch);
                    }
                    else
                    {
                        builder.Append(@"\u");
                        builder.Append(unchecked((short)ch).ToString(CultureInfo.InvariantCulture));
                        builder.Append('?');
                    }

                    break;
            }
        }

        return builder.ToString();
    }
}
