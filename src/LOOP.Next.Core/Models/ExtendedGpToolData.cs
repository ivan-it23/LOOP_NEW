using System.Runtime.InteropServices;

namespace LoopNext.Core.Models;

/// <summary>
/// Полный 320-байтный кадр измерений новой прошивки прибора.
/// </summary>
/// <remarks>
/// Первые 240 байт обязаны в точности совпадать с <see cref="GpToolData"/>.
/// Хвост содержит два массива по десять <see cref="float"/> и существует
/// только у новой прошивки. Отдельный тип не позволяет случайно изменить
/// двоичный контракт старой 240-байтной ветки.
/// </remarks>
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public unsafe struct ExtendedGpToolData
{
    public GpToolData Common;
    public fixed float RhoAttSmt[GpToolData.FlatFrequencyTransmitterCount];
    public fixed float AttDbSmt[GpToolData.FlatFrequencyTransmitterCount];

    public readonly float GetRhoAttSmt(int frequencyIndex, int transmitterIndex) =>
        RhoAttSmt[GpToolData.GetFrequencyTransmitterIndex(frequencyIndex, transmitterIndex)];

    public void SetRhoAttSmt(int frequencyIndex, int transmitterIndex, float value) =>
        RhoAttSmt[GpToolData.GetFrequencyTransmitterIndex(frequencyIndex, transmitterIndex)] = value;

    public readonly float GetAttDbSmt(int frequencyIndex, int transmitterIndex) =>
        AttDbSmt[GpToolData.GetFrequencyTransmitterIndex(frequencyIndex, transmitterIndex)];

    public void SetAttDbSmt(int frequencyIndex, int transmitterIndex, float value) =>
        AttDbSmt[GpToolData.GetFrequencyTransmitterIndex(frequencyIndex, transmitterIndex)] = value;
}
