using LoopNext.Core.Models;

namespace LoopNext.Metrology;

/// <summary>
/// Ячейка таблицы, отклонение в которой определяется шумом, а не кольцом.
/// </summary>
/// <param name="ResistanceIndex">Номер сопротивления, начиная с единицы (R1 = 1).</param>
/// <param name="RelativeNoise">Шум усреднённого значения в долях эталонной фазы.</param>
/// <param name="RequiredAveraging">
/// Наименьшее из доступных усреднений, при котором шум уложится в допуск, или 0,
/// если не хватает и наибольшего.
/// </param>
public readonly record struct NoisyMeasurementCell(
    int ResistanceIndex,
    int ChannelIndex,
    double ReferencePhaseMg,
    double NoiseMg,
    double RelativeNoise,
    int RequiredAveraging);

/// <summary>
/// Определяет ячейки таблицы, которые при текущем шуме нельзя сравнить с допуском.
/// </summary>
/// <remarks>
/// При больших сопротивлениях эталонная фаза на 401 кГц падает до десятков мГ, а
/// шум одного кадра в воздухе у тех же каналов - 30…80 мГ. Такая ячейка
/// окрашивается как «критическая» по случайности, и отклонение в ней ничего не
/// говорит ни о приборе, ни о кольце.
///
/// Шум усреднённого значения складывается из шума самого измерения (усреднение
/// по <c>averaging</c> кадрам) и погрешности воздуха, который вычитается из
/// каждого кадра (усреднение по <c>airSamples</c> кадрам):
/// <c>σ·√(1/averaging + 1/airSamples)</c>. Ячейка неразличима, если этот шум
/// превышает допуск, выраженный в мГ: <c>threshold·|эталон|</c>.
///
/// Проверено по прогону коллег 23.09 (LWD 4Tx № 42023, усреднение 5, воздух 20,
/// допуск 6 %). Правило выделило R5 T4 и все каналы R6–R8 на 401 кГц, R7 T4 и
/// R8 T3/T4 на 2001 кГц: в 15 из 16 этих ячеек таблица показала от −37 до
/// +158 %, а шестнадцатая (R8 T3, −0,6 % при шуме 49 %) попала в допуск по
/// случайности. Систематические отклонения - высокочастотная группа при R1–R3,
/// постоянные +7…8 % у T1/T2 на 401 кГц - шумом не объясняются, и правило их
/// не задевает: это другая причина, и выдавать её за шум нельзя.
/// </remarks>
public static class MeasurementNoiseFloor
{
    public static IReadOnlyList<NoisyMeasurementCell> Evaluate(
        IReadOnlyList<V10> phaseReferences,
        in V10 airStdDev,
        int channelCount,
        int airSamples,
        int averaging,
        int thresholdPercent,
        IReadOnlyList<int> averagingOptions)
    {
        ArgumentNullException.ThrowIfNull(phaseReferences);
        ArgumentNullException.ThrowIfNull(averagingOptions);

        var result = new List<NoisyMeasurementCell>();
        if (airSamples <= 0 || averaging <= 0 || thresholdPercent <= 0)
        {
            return result;
        }

        var tolerance = thresholdPercent / 100.0;
        var count = Math.Min(channelCount, V10.ChannelCount);

        // Эталон R0 нулевой: это точка воздуха, сравнивать там нечего.
        for (var resistance = 1; resistance < phaseReferences.Count; resistance++)
        {
            var references = phaseReferences[resistance];
            for (var channel = 0; channel < count; channel++)
            {
                var sigma = (double)airStdDev.GetValue(channel);
                var reference = Math.Abs((double)references.GetValue(channel));
                if (!(sigma > 0.0) || !double.IsFinite(sigma) || !(reference > 0.0) || !double.IsFinite(reference))
                {
                    continue;
                }

                var noise = NoiseOfMean(sigma, averaging, airSamples);
                var allowed = tolerance * reference;
                if (noise <= allowed)
                {
                    continue;
                }

                var required = 0;
                foreach (var option in averagingOptions.Where(option => option > 0).OrderBy(option => option))
                {
                    if (NoiseOfMean(sigma, option, airSamples) <= allowed)
                    {
                        required = option;
                        break;
                    }
                }

                result.Add(new NoisyMeasurementCell(
                    resistance,
                    channel,
                    reference * PhysicalConstants.Milligrad,
                    noise * PhysicalConstants.Milligrad,
                    noise / reference,
                    required));
            }
        }

        return result;
    }

    private static double NoiseOfMean(double sigma, int averaging, int airSamples) =>
        sigma * Math.Sqrt(1.0 / averaging + 1.0 / airSamples);
}
