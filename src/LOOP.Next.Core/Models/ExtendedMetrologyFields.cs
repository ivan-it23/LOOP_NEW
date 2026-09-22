namespace LoopNext.Core.Models;

/// <summary>
/// Поля метрологии, которые появились в новой прошивке.
/// </summary>
/// <remarks>
/// Общая часть 0..83 одинакова в обоих поколениях, и её описывает
/// <see cref="GpToolMetrology"/>. Дальше раскладки расходятся при неизменном
/// размере структуры 240 байт: старая прошивка держит со смещения 84
/// нерасшифрованную служебную область, новая забрала у неё сначала 40 байт под
/// воздушные поправки затухания, затем ещё 8 под отметку времени записи.
///
/// Поля читаются прямо из принятых байт и только для показа и журнала:
/// расчёты этого приложения ведутся по разности фаз и лежат в общей части.
/// Заводить их в <see cref="GpToolMetrology"/> нельзя, не разведя структуру по
/// поколениям, а этого сейчас не требуется.
///
/// **Запись метрологии из этого приложения не реализована, и до моделирования
/// раскладки по поколениям её добавлять нельзя**: запись по старой раскладке
/// затрёт и поправки затухания, и отметку времени, а у новой прошивки запись
/// идёт другим кодом команды
/// (<see cref="Protocol.ToolProtocolConstants.CmdSetMetrologyExtended"/>).
/// </remarks>
public static class ExtendedMetrologyFields
{
    /// <summary>Смещение, на котором кончается общая для обоих поколений часть.</summary>
    public const int CommonSectionSize = 84;

    /// <summary>Воздушные поправки затухания, дБ: две частоты по пять передатчиков.</summary>
    public const int AirAttenuationDbOffset = CommonSectionSize;

    /// <summary>Отметка времени записи метрологии: секунды Unix-эпохи, UTC.</summary>
    public const int WriteTimestampOffset = 124;

    /// <summary>Размер структуры метрологии; одинаков в обоих поколениях.</summary>
    public const int MetrologySize = 240;

    public static bool TryReadAirAttenuationDb(
        ReadOnlySpan<byte> metrology,
        int frequencyIndex,
        int transmitterIndex,
        out float value)
    {
        value = 0.0f;
        if (metrology.Length < MetrologySize
            || (uint)frequencyIndex >= GpToolMetrology.FrequencyCount
            || (uint)transmitterIndex >= GpToolMetrology.MaxTransmitterCount)
        {
            return false;
        }

        var index = (frequencyIndex * GpToolMetrology.MaxTransmitterCount) + transmitterIndex;
        value = BitConverter.ToSingle(
            metrology.Slice(AirAttenuationDbOffset + (index * sizeof(float)), sizeof(float)));
        return true;
    }

    /// <summary>
    /// Отметку ставит записывающая программа по часам компьютера, а не прибор:
    /// собственных часов у прибора нет, он это поле только хранит и отдаёт.
    /// Отметки нет, если в поле лежат нули (метрология записана до появления
    /// поля) либо восемь байт <c>0xFF</c> (энергонезависимая память не
    /// размечена). Оба случая обязаны выводиться пустым значением, а не датой
    /// 1970 года и не отрицательным числом.
    /// </summary>
    public static bool TryReadWriteTimestamp(ReadOnlySpan<byte> metrology, out DateTimeOffset timestamp)
    {
        timestamp = default;
        if (metrology.Length < MetrologySize)
        {
            return false;
        }

        var seconds = BitConverter.ToInt64(metrology.Slice(WriteTimestampOffset, sizeof(long)));
        if (seconds == 0 || seconds == -1)
        {
            return false;
        }

        if (seconds < 0 || seconds > 253_402_300_799)
        {
            return false;
        }

        timestamp = DateTimeOffset.FromUnixTimeSeconds(seconds);
        return true;
    }
}
