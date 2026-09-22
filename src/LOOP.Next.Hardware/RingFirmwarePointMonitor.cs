namespace LoopNext.Hardware;

/// <summary>
/// Classifies whether a commanded ring point remains physically observable in
/// the tool phase stream. The ring protocol only acknowledges commands and has
/// no RESREAD/CAPREAD state query, so this monitor deliberately uses the real
/// corrected phase as the independent evidence source.
/// </summary>
public sealed class RingFirmwarePointMonitor
{
    private const double AbsoluteResponseFloorMg = 100.0;
    private const double ReferenceResponseFraction = 0.10;
    private const double DroppedRetentionLimit = 0.10;
    private const double HeldRetentionLimit = 0.50;

    /// <summary>
    /// Запас над порогом обнаружения, ниже которого классификация невозможна.
    /// </summary>
    /// <remarks>
    /// Порог отклика не опускается ниже <see cref="AbsoluteResponseFloorMg"/>.
    /// Значит, для точки, где сама эталонная фаза меньше этого порога, даже
    /// безупречное измерение не превысит его, и «отклика нет» будет неотличимо
    /// от «отклик мал по построению». Так выглядят большие сопротивления: при
    /// R8 эталон по ближним каналам падает до десятков мГ. Такие точки обязаны
    /// оставаться неопределёнными, иначе исправный прогон будет прерван.
    /// </remarks>
    private const double ConclusiveReferenceMargin = 1.5;

    /// <summary>
    /// Номер кадра, который не считается уликой отклика.
    /// </summary>
    /// <remarks>
    /// Прибор строит кадры непрерывно, а опрос идёт раз в секунду, поэтому
    /// первый кадр после квитанции <c>RES/CAP</c> мог быть снят ещё до того, как
    /// команда дошла до кольца, и несёт состояние предыдущей точки.
    ///
    /// Подтверждено прогоном 22.09: на точке R1/C8 первый кадр показал по всем
    /// высокочастотным каналам ровно то же, что предыдущая точка C7, а кадры
    /// 2..10 - шум. Приняв первый кадр за пик, монитор объявил бы, что кольцо
    /// удержало меру и отпустило, хотя оно её вовсе не включало.
    ///
    /// Отклик, начинающийся со второго кадра, уликой уже является: он снят
    /// заведомо после команды.
    /// </remarks>
    private const int StaleFrameIndex = 1;

    private readonly List<RingFirmwarePhaseObservation> _observations = new();

    public void Observe(
        int frameIndex,
        double elapsedMilliseconds,
        double measuredPhaseMg,
        double referencePhaseMg,
        bool isAveragingFrame)
    {
        if (frameIndex <= 0 ||
            !double.IsFinite(elapsedMilliseconds) ||
            !double.IsFinite(measuredPhaseMg) ||
            !double.IsFinite(referencePhaseMg))
        {
            return;
        }

        _observations.Add(new RingFirmwarePhaseObservation(
            frameIndex,
            Math.Max(0.0, elapsedMilliseconds),
            measuredPhaseMg,
            referencePhaseMg,
            isAveragingFrame));
    }

    public RingFirmwarePointSummary BuildSummary()
    {
        if (_observations.Count == 0)
        {
            return RingFirmwarePointSummary.Empty;
        }

        // Первый кадр в уликах не участвует: он мог быть снят до команды.
        var evidence = _observations
            .Where(observation => observation.FrameIndex > StaleFrameIndex)
            .ToArray();
        if (evidence.Length == 0)
        {
            evidence = _observations.ToArray();
        }

        var peak = evidence
            .OrderByDescending(observation => Math.Abs(observation.MeasuredPhaseMg))
            .First();
        var last = _observations[^1];
        var averaging = _observations.Where(observation => observation.IsAveragingFrame).ToArray();
        var responseThresholdMg = Math.Max(
            AbsoluteResponseFloorMg,
            Math.Abs(peak.ReferencePhaseMg) * ReferenceResponseFraction);
        var peakAbsMg = Math.Abs(peak.MeasuredPhaseMg);
        var lateMeanAbsMg = averaging.Length == 0
            ? double.NaN
            : averaging.Average(observation => Math.Abs(observation.MeasuredPhaseMg));
        var retentionRatio = peakAbsMg <= double.Epsilon || !double.IsFinite(lateMeanAbsMg)
            ? double.NaN
            : lateMeanAbsMg / peakAbsMg;
        var firstResponse = evidence.FirstOrDefault(
            observation => Math.Abs(observation.MeasuredPhaseMg) >= responseThresholdMg);

        var state = Classify(
            _observations.Count,
            averaging.Length,
            peakAbsMg,
            responseThresholdMg,
            retentionRatio,
            peak.ReferencePhaseMg);

        return new RingFirmwarePointSummary(
            state,
            _observations.Count,
            averaging.Length,
            responseThresholdMg,
            firstResponse.FrameIndex,
            firstResponse.ElapsedMilliseconds,
            peak.FrameIndex,
            peak.ElapsedMilliseconds,
            peak.MeasuredPhaseMg,
            lateMeanAbsMg,
            last.MeasuredPhaseMg,
            retentionRatio,
            peak.ReferencePhaseMg);
    }

    private static RingFirmwareRetentionState Classify(
        int observationCount,
        int averagingCount,
        double peakAbsMg,
        double responseThresholdMg,
        double retentionRatio,
        double referencePhaseMg)
    {
        if (observationCount < 2 || averagingCount == 0 || !double.IsFinite(retentionRatio))
        {
            return RingFirmwareRetentionState.Inconclusive;
        }

        // Ожидаемый отклик сам по себе ниже порога обнаружения: отличить
        // молчащее кольцо от малой по построению фазы нечем.
        if (Math.Abs(referencePhaseMg) < AbsoluteResponseFloorMg * ConclusiveReferenceMargin)
        {
            return RingFirmwareRetentionState.Inconclusive;
        }

        if (peakAbsMg < responseThresholdMg)
        {
            return RingFirmwareRetentionState.NoResponse;
        }

        if (retentionRatio <= DroppedRetentionLimit)
        {
            return RingFirmwareRetentionState.Dropped;
        }

        if (retentionRatio >= HeldRetentionLimit)
        {
            return RingFirmwareRetentionState.Held;
        }

        return RingFirmwareRetentionState.Inconclusive;
    }
}

public enum RingFirmwareRetentionState
{
    Inconclusive,
    NoResponse,
    Held,
    Dropped
}

public readonly record struct RingFirmwarePhaseObservation(
    int FrameIndex,
    double ElapsedMilliseconds,
    double MeasuredPhaseMg,
    double ReferencePhaseMg,
    bool IsAveragingFrame);

public readonly record struct RingFirmwarePointSummary(
    RingFirmwareRetentionState State,
    int ObservationCount,
    int AveragingObservationCount,
    double ResponseThresholdMg,
    int FirstResponseFrame,
    double FirstResponseElapsedMilliseconds,
    int PeakFrame,
    double PeakElapsedMilliseconds,
    double PeakPhaseMg,
    double LateMeanAbsPhaseMg,
    double LastPhaseMg,
    double RetentionRatio,
    double ReferencePhaseMg)
{
    public static RingFirmwarePointSummary Empty { get; } = new(
        RingFirmwareRetentionState.Inconclusive,
        0,
        0,
        double.NaN,
        0,
        0.0,
        0,
        0.0,
        double.NaN,
        double.NaN,
        double.NaN,
        double.NaN,
        double.NaN);
}
