namespace LoopNext.Core.Models;

public enum LoopMeasurementComponent
{
    Z1,
    Z2,
    X1,
    Y1,
    X2,
    Y2
}

public struct SondeParameter
{
    public float L1;
    public float L2;
    public float Frequency;

    // Единственное место, где решается, пригоден ли канал к расчёту.
    // Незаполненная в метрологии пара L1/L2 приходит нулями: прямая задача
    // вырождается в 0/0 и отдаёт NaN, а обратная по такому NaN не сходится
    // никогда. Поэтому вырожденную геометрию отсекаем до вычислений, а не после.
    public readonly bool HasUsableGeometry =>
        float.IsFinite(L1) && L1 > 0f &&
        float.IsFinite(L2) && L2 > L1 &&
        float.IsFinite(Frequency) && Frequency > 0f;
}

public struct LoopParameter
{
    public float Resistance;
    public float AdditionalWireResistance;
    public float Reactance;
    public float Radius;
}

public readonly record struct MeasurementSettings(
    int AveragingCount,
    int ThresholdPercent,
    bool ExpressMode,
    int PollIntervalMs,
    int LocalAirSampleCount);
