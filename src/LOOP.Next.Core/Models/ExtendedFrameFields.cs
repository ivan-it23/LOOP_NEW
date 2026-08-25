namespace LoopNext.Core.Models;

/// <summary>
/// Поля кадра измерений, которые появились в новой прошивке.
/// </summary>
/// <remarks>
/// Приложение считает только по разности фаз, поэтому амплитудные величины не
/// заведены в <see cref="GpToolData"/> и читаются прямо из принятых байт — их
/// единственное назначение здесь — журнал обмена.
///
/// Раскладка: новая прошивка дописала два массива в конец кадра, не сдвинув
/// общую часть. Смещения отсчитываются от начала структуры измерений, то есть
/// уже после служебных байт нагрузки.
/// </remarks>
public static class ExtendedFrameFields
{
    /// <summary>Амплитудные УЭС, Ом·м.</summary>
    public const int RhoAttSmtOffset = 240;

    /// <summary>Симметризованные затухания, дБ.</summary>
    public const int AttDbSmtOffset = 280;

    /// <summary>Размер каждого из массивов: две частоты по пять передатчиков.</summary>
    public const int ChannelArraySize = GpToolData.FlatFrequencyTransmitterCount * sizeof(float);

    /// <summary>Размер кадра, в котором эти поля присутствуют.</summary>
    public const int RequiredFrameSize = AttDbSmtOffset + ChannelArraySize;

    public static bool TryReadRhoAttSmt(ReadOnlySpan<byte> frame, int frequencyIndex, int transmitterIndex, out float value)
    {
        return TryRead(frame, RhoAttSmtOffset, frequencyIndex, transmitterIndex, out value);
    }

    public static bool TryReadAttDbSmt(ReadOnlySpan<byte> frame, int frequencyIndex, int transmitterIndex, out float value)
    {
        return TryRead(frame, AttDbSmtOffset, frequencyIndex, transmitterIndex, out value);
    }

    private static bool TryRead(
        ReadOnlySpan<byte> frame,
        int arrayOffset,
        int frequencyIndex,
        int transmitterIndex,
        out float value)
    {
        value = 0.0f;
        if (frame.Length < RequiredFrameSize
            || (uint)frequencyIndex >= GpToolData.FrequencyCount
            || (uint)transmitterIndex >= GpToolData.MaxTransmitterCount)
        {
            return false;
        }

        var index = (frequencyIndex * GpToolData.MaxTransmitterCount) + transmitterIndex;
        value = BitConverter.ToSingle(frame.Slice(arrayOffset + (index * sizeof(float)), sizeof(float)));
        return true;
    }
}
