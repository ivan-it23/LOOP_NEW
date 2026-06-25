namespace LoopNext.Core.Models;

public sealed class PhaseOrientationDebug
{
    public bool Valid { get; set; }
    public uint Signature { get; set; }
    public uint TransmitterCount { get; set; }
    public uint RxPosition { get; set; }
    public float[,] RawDelta { get; } = new float[2, 5];
    public float[,] RawNormalized { get; } = new float[2, 5];
    public int[,] OrientationSign { get; } = new int[2, 5];
    public float[,] FinalPhase { get; } = new float[2, 5];
}
