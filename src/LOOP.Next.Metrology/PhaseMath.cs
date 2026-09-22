namespace LoopNext.Metrology;

// Чистые функции нормализации и ориентации фаз.
// Перенос из function.cpp: normalize_phase, normalized_rx_position,
// rx_phase_orientation_sign, apply_rx_phase_orientation (function.cpp:29-51).
// Поведение сохранено без изменений согласно правилам миграции.
public static class PhaseMath
{
    // Приведение фазы к диапазону (-Pi, Pi]. Воспроизводит normalize_phase (function.cpp:29).
    public static float NormalizePhase(float phase)
    {
        while (phase > PhysicalConstants.Pi)
        {
            phase -= (float)(2 * PhysicalConstants.Pi);
        }

        while (phase <= -PhysicalConstants.Pi)
        {
            phase += (float)(2 * PhysicalConstants.Pi);
        }

        return phase;
    }

    // Нормализация положения приёмников: значимо только Rx_Position == 1 (function.cpp:35).
    public static uint NormalizedRxPosition(uint rxPosition) => rxPosition == 1 ? 1u : 0u;

    /// <summary>
    /// Знак ориентации фазы для передатчика Tx. Индексация с нуля: 0 = T1.
    /// </summary>
    /// <remarks>
    /// Знак чередуется только по чётности передатчика: <c>+ − + − +</c>.
    /// Это поведение всех трёх имеющихся исходников прошивки — старого
    /// Автономного (<c>main_AUTO_5Tx.txt</c>), старого LWD
    /// (<c>main_LWD_1DDS</c>) и нового LWD
    /// (<c>LWD_1_DDS_ATT_UES (2).c</c>): в каждом из них
    /// <c>simmetry</c> берёт <c>Dir = (Tx % 2 == 0) ? +1 : -1</c> и
    /// <c>Rx_Position</c> в фазовой математике не участвует.
    ///
    /// <c>Rx_Position</c> обслуживается самим прибором и до появления
    /// <c>DELTA_PH</c>: в главном цикле прошивка при <c>Rx_Position == 1</c>
    /// меняет местами каналы АЦП (<c>CH1 = ADC_CH2, CH2 = ADC_CH1</c>), после
    /// чего считает <c>DELTA_PH = d_ph(PH_2, PH_1)</c>. Приложение получает
    /// уже переставленную величину, и второй переворот на хосте был бы двойным
    /// учётом одного и того же.
    /// </remarks>
    public static int TxPhaseOrientationSign(uint transmitterIndex) =>
        (transmitterIndex & 1) == 0 ? 1 : -1;

    /// <summary>
    /// Прежний знак, зависевший от <c>Rx_Position</c>. Оставлен только для
    /// диагностики: по нему в журнал выводится, что дал бы прежний расчёт,
    /// когда <c>Rx_Position == 1</c> и два варианта расходятся. В рабочем
    /// потоке измерений не используется.
    /// </summary>
    public static int LegacyRxPhaseOrientationSign(uint transmitterIndex, uint rxPosition)
    {
        var rx = NormalizedRxPosition(rxPosition);
        var oddTxNumber = ((transmitterIndex + 1) & 1) != 0; // T1, T3, T5
        var invert = !oddTxNumber; // Rx_Position 0: T2/T4 инвертируются
        if (rx == 1)
        {
            invert = !invert; // Rx_Position 1: схема смещалась на один передатчик
        }

        return invert ? -1 : 1;
    }

    /// <summary>
    /// Применение воздушной поправки и ориентации к сырой фазе.
    /// </summary>
    public static float ApplyTxPhaseOrientation(float rawPhase, float airPhase, uint transmitterIndex)
    {
        var phase = NormalizePhase(rawPhase) - airPhase;
        if (TxPhaseOrientationSign(transmitterIndex) < 0)
        {
            phase = -phase;
        }

        return NormalizePhase(phase);
    }
}
