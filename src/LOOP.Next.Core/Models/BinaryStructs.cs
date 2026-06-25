using System.Runtime.InteropServices;

namespace LoopNext.Core.Models;

[StructLayout(LayoutKind.Sequential, Pack = 1)]
public unsafe struct V10
{
    public const int ChannelCount = 10;

    public fixed float Value[ChannelCount];

    public readonly float GetValue(int channelIndex)
    {
        ValidateIndex(channelIndex, ChannelCount, nameof(channelIndex));
        return Value[channelIndex];
    }

    public void SetValue(int channelIndex, float value)
    {
        ValidateIndex(channelIndex, ChannelCount, nameof(channelIndex));
        Value[channelIndex] = value;
    }

    public void Clear()
    {
        for (var i = 0; i < ChannelCount; i++)
        {
            Value[i] = 0.0f;
        }
    }

    private static void ValidateIndex(int index, int length, string parameterName)
    {
        if ((uint)index >= (uint)length)
        {
            throw new ArgumentOutOfRangeException(parameterName, index, $"Index must be in range 0..{length - 1}.");
        }
    }
}

[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct IncSet
{
    public uint MaWindowSize;
    public uint MldWindowSize;
    public uint N;
    public uint K;
    public uint AutoDelta;
    public uint InterruptByAnglePath;
    public float WMin;
    public float WMax;
    public float KPredict;
    public float ApsDelta;
    public float HistoryAngle;
    public float KDelta;
}

[StructLayout(LayoutKind.Sequential, Pack = 1)]
public unsafe struct GpToolData
{
    public const int FrequencyCount = 2;
    public const int MaxTransmitterCount = 5;
    public const int FlatFrequencyTransmitterCount = FrequencyCount * MaxTransmitterCount;

    public uint Signature;
    public uint Condition;
    public uint Frame;
    public float Temperature;
    public fixed float RhoSmt[FlatFrequencyTransmitterCount];
    public fixed float PhaseSmt[FlatFrequencyTransmitterCount];
    public fixed float AmRx1[FlatFrequencyTransmitterCount];
    public fixed float ZeroAmRx1[FrequencyCount];
    public fixed float AmRx2[FlatFrequencyTransmitterCount];
    public fixed float ZeroAmRx2[FrequencyCount];
    public fixed float DeltaPhase[FlatFrequencyTransmitterCount];
    public fixed float ZeroDeltaPhase[FrequencyCount];

    public readonly float GetRhoSmt(int frequencyIndex, int transmitterIndex) =>
        RhoSmt[GetFrequencyTransmitterIndex(frequencyIndex, transmitterIndex)];

    public void SetRhoSmt(int frequencyIndex, int transmitterIndex, float value) =>
        RhoSmt[GetFrequencyTransmitterIndex(frequencyIndex, transmitterIndex)] = value;

    public readonly float GetPhaseSmt(int frequencyIndex, int transmitterIndex) =>
        PhaseSmt[GetFrequencyTransmitterIndex(frequencyIndex, transmitterIndex)];

    public void SetPhaseSmt(int frequencyIndex, int transmitterIndex, float value) =>
        PhaseSmt[GetFrequencyTransmitterIndex(frequencyIndex, transmitterIndex)] = value;

    public readonly float GetAmRx1(int frequencyIndex, int transmitterIndex) =>
        AmRx1[GetFrequencyTransmitterIndex(frequencyIndex, transmitterIndex)];

    public void SetAmRx1(int frequencyIndex, int transmitterIndex, float value) =>
        AmRx1[GetFrequencyTransmitterIndex(frequencyIndex, transmitterIndex)] = value;

    public readonly float GetZeroAmRx1(int frequencyIndex)
    {
        ValidateIndex(frequencyIndex, FrequencyCount, nameof(frequencyIndex));
        return ZeroAmRx1[frequencyIndex];
    }

    public void SetZeroAmRx1(int frequencyIndex, float value)
    {
        ValidateIndex(frequencyIndex, FrequencyCount, nameof(frequencyIndex));
        ZeroAmRx1[frequencyIndex] = value;
    }

    public readonly float GetAmRx2(int frequencyIndex, int transmitterIndex) =>
        AmRx2[GetFrequencyTransmitterIndex(frequencyIndex, transmitterIndex)];

    public void SetAmRx2(int frequencyIndex, int transmitterIndex, float value) =>
        AmRx2[GetFrequencyTransmitterIndex(frequencyIndex, transmitterIndex)] = value;

    public readonly float GetZeroAmRx2(int frequencyIndex)
    {
        ValidateIndex(frequencyIndex, FrequencyCount, nameof(frequencyIndex));
        return ZeroAmRx2[frequencyIndex];
    }

    public void SetZeroAmRx2(int frequencyIndex, float value)
    {
        ValidateIndex(frequencyIndex, FrequencyCount, nameof(frequencyIndex));
        ZeroAmRx2[frequencyIndex] = value;
    }

    public readonly float GetDeltaPhase(int frequencyIndex, int transmitterIndex) =>
        DeltaPhase[GetFrequencyTransmitterIndex(frequencyIndex, transmitterIndex)];

    public void SetDeltaPhase(int frequencyIndex, int transmitterIndex, float value) =>
        DeltaPhase[GetFrequencyTransmitterIndex(frequencyIndex, transmitterIndex)] = value;

    public readonly float GetZeroDeltaPhase(int frequencyIndex)
    {
        ValidateIndex(frequencyIndex, FrequencyCount, nameof(frequencyIndex));
        return ZeroDeltaPhase[frequencyIndex];
    }

    public void SetZeroDeltaPhase(int frequencyIndex, float value)
    {
        ValidateIndex(frequencyIndex, FrequencyCount, nameof(frequencyIndex));
        ZeroDeltaPhase[frequencyIndex] = value;
    }

    public static int GetFrequencyTransmitterIndex(int frequencyIndex, int transmitterIndex)
    {
        ValidateIndex(frequencyIndex, FrequencyCount, nameof(frequencyIndex));
        ValidateIndex(transmitterIndex, MaxTransmitterCount, nameof(transmitterIndex));
        return frequencyIndex * MaxTransmitterCount + transmitterIndex;
    }

    private static void ValidateIndex(int index, int length, string parameterName)
    {
        if ((uint)index >= (uint)length)
        {
            throw new ArgumentOutOfRangeException(parameterName, index, $"Index must be in range 0..{length - 1}.");
        }
    }
}

[StructLayout(LayoutKind.Sequential, Pack = 1)]
public unsafe struct GpToolMetrology
{
    public const int FrequencyCount = 2;
    public const int MaxTransmitterCount = 5;
    public const int FlatFrequencyTransmitterCount = FrequencyCount * MaxTransmitterCount;
    public const int ServiceValueCount = 78;

    public uint Signature;
    public uint Serial;
    public fixed ushort L1[MaxTransmitterCount];
    public fixed ushort L2[MaxTransmitterCount];
    public fixed ushort Frequency[FrequencyCount];
    public fixed short AirZz[FlatFrequencyTransmitterCount];
    public fixed short AirZzAmplitude[FlatFrequencyTransmitterCount];
    public uint SondeDiameterMm;
    public uint WorkType;
    public uint RxPosition;
    public fixed ushort Service[ServiceValueCount];

    public readonly ushort GetL1(int transmitterIndex)
    {
        ValidateIndex(transmitterIndex, MaxTransmitterCount, nameof(transmitterIndex));
        return L1[transmitterIndex];
    }

    public void SetL1(int transmitterIndex, ushort value)
    {
        ValidateIndex(transmitterIndex, MaxTransmitterCount, nameof(transmitterIndex));
        L1[transmitterIndex] = value;
    }

    public readonly ushort GetL2(int transmitterIndex)
    {
        ValidateIndex(transmitterIndex, MaxTransmitterCount, nameof(transmitterIndex));
        return L2[transmitterIndex];
    }

    public void SetL2(int transmitterIndex, ushort value)
    {
        ValidateIndex(transmitterIndex, MaxTransmitterCount, nameof(transmitterIndex));
        L2[transmitterIndex] = value;
    }

    public readonly ushort GetFrequency(int frequencyIndex)
    {
        ValidateIndex(frequencyIndex, FrequencyCount, nameof(frequencyIndex));
        return Frequency[frequencyIndex];
    }

    public void SetFrequency(int frequencyIndex, ushort value)
    {
        ValidateIndex(frequencyIndex, FrequencyCount, nameof(frequencyIndex));
        Frequency[frequencyIndex] = value;
    }

    public readonly short GetAirZz(int frequencyIndex, int transmitterIndex) =>
        AirZz[GetFrequencyTransmitterIndex(frequencyIndex, transmitterIndex)];

    public void SetAirZz(int frequencyIndex, int transmitterIndex, short value) =>
        AirZz[GetFrequencyTransmitterIndex(frequencyIndex, transmitterIndex)] = value;

    public readonly short GetAirZzAmplitude(int frequencyIndex, int transmitterIndex) =>
        AirZzAmplitude[GetFrequencyTransmitterIndex(frequencyIndex, transmitterIndex)];

    public void SetAirZzAmplitude(int frequencyIndex, int transmitterIndex, short value) =>
        AirZzAmplitude[GetFrequencyTransmitterIndex(frequencyIndex, transmitterIndex)] = value;

    public readonly ushort GetServiceValue(int serviceIndex)
    {
        ValidateIndex(serviceIndex, ServiceValueCount, nameof(serviceIndex));
        return Service[serviceIndex];
    }

    public void SetServiceValue(int serviceIndex, ushort value)
    {
        ValidateIndex(serviceIndex, ServiceValueCount, nameof(serviceIndex));
        Service[serviceIndex] = value;
    }

    public static int GetFrequencyTransmitterIndex(int frequencyIndex, int transmitterIndex)
    {
        ValidateIndex(frequencyIndex, FrequencyCount, nameof(frequencyIndex));
        ValidateIndex(transmitterIndex, MaxTransmitterCount, nameof(transmitterIndex));
        return frequencyIndex * MaxTransmitterCount + transmitterIndex;
    }

    private static void ValidateIndex(int index, int length, string parameterName)
    {
        if ((uint)index >= (uint)length)
        {
            throw new ArgumentOutOfRangeException(parameterName, index, $"Index must be in range 0..{length - 1}.");
        }
    }
}
