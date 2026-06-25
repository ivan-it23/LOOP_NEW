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
