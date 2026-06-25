namespace LoopNext.Core.Models;

// Категория качества ячейки таблицы по отклонению от эталона.
// Соответствует цветовой окраске do_graf (WorkForm.h:5653-5660):
// LimeGreen / Gold / Red / DarkRed. Конкретные цвета задаются в UI-слое.
public enum MeasurementQuality
{
    InTolerance,
    Warning,
    OutOfTolerance,
    Critical
}

// Классификация значения ячейки по порогу. Точный перенос порогов do_graf.
public static class MeasurementQualityClassifier
{
    public static MeasurementQuality Classify(float value, int threshold)
    {
        var magnitude = MathF.Abs(value);
        var t = (float)threshold;

        if (magnitude <= t)
        {
            return MeasurementQuality.InTolerance;
        }

        if (magnitude <= 2f * t)
        {
            return MeasurementQuality.Warning;
        }

        if (magnitude <= 3f * t)
        {
            return MeasurementQuality.OutOfTolerance;
        }

        return MeasurementQuality.Critical;
    }
}

// Ячейка таблицы: среднее отклонение в процентах и его категория качества.
public readonly record struct MeasurementCell(bool HasValue, float Value, MeasurementQuality Quality)
{
    public static readonly MeasurementCell Empty = new(false, 0f, MeasurementQuality.InTolerance);

    public static MeasurementCell Create(float value, int threshold) =>
        new(true, value, MeasurementQualityClassifier.Classify(value, threshold));
}

// Заголовок столбца канала: номер передатчика и частота.
// Перенос логики метки do_graf (WorkForm.h:5643-5649): label "T{Tx}  {freq}".
public readonly record struct MeasurementChannelHeader(int ChannelIndex, int TransmitterNumber, float FrequencyKHz)
{
    public string Label => $"T{TransmitterNumber}  {FrequencyKHz:F0}";
}

// Строка таблицы для одного значения сопротивления кольца (R1..R8).
public sealed class MeasurementTableRow
{
    public const int MaxChannels = 10;

    public int ResistanceIndex { get; }

    public MeasurementCell[] Cells { get; }

    public MeasurementTableRow(int resistanceIndex)
    {
        ResistanceIndex = resistanceIndex;
        Cells = new MeasurementCell[MaxChannels];
        Clear();
    }

    public void SetCell(int channelIndex, float value, int threshold)
    {
        Cells[channelIndex] = MeasurementCell.Create(value, threshold);
    }

    public void Clear()
    {
        for (var i = 0; i < Cells.Length; i++)
        {
            Cells[i] = MeasurementCell.Empty;
        }
    }
}

// Таблица измерений: 8 строк сопротивлений и заголовки каналов по параметрам зонда.
public sealed class MeasurementTable
{
    public const int ResistanceCount = 8;

    public MeasurementTableRow[] Rows { get; }

    public IReadOnlyList<MeasurementChannelHeader> Headers { get; private set; }

    public MeasurementTable()
    {
        Rows = new MeasurementTableRow[ResistanceCount];
        for (var i = 0; i < ResistanceCount; i++)
        {
            Rows[i] = new MeasurementTableRow(i + 1);
        }

        Headers = Array.Empty<MeasurementChannelHeader>();
    }

    // Построение заголовков из параметров зонда: номер Tx и частота канала.
    public void SetHeaders(IReadOnlyList<SondeParameter> sondeParameters)
    {
        var count = Math.Min(sondeParameters.Count, MeasurementTableRow.MaxChannels);
        var transmitterCount = count / 2;
        var headers = new List<MeasurementChannelHeader>(count);

        for (var i = 0; i < count; i++)
        {
            var channelNumber = i + 1;
            var transmitterNumber = channelNumber <= transmitterCount
                ? channelNumber
                : channelNumber - transmitterCount;
            var frequencyKHz = sondeParameters[i].Frequency / 1000f;
            headers.Add(new MeasurementChannelHeader(i, transmitterNumber, frequencyKHz));
        }

        Headers = headers;
    }

    public void Clear()
    {
        foreach (var row in Rows)
        {
            row.Clear();
        }
    }
}
