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

    // Знак ориентации фазы для передатчика Tx с учётом положения приёмников.
    // Воспроизводит rx_phase_orientation_sign (function.cpp:39). Tx индексируется с нуля: 0 = T1.
    public static int RxPhaseOrientationSign(uint transmitterIndex, uint rxPosition)
    {
        var rx = NormalizedRxPosition(rxPosition);
        var oddTxNumber = ((transmitterIndex + 1) & 1) != 0; // T1, T3, T5
        var invert = !oddTxNumber; // Rx_Position 0: T2/T4 инвертируются
        if (rx == 1)
        {
            invert = !invert; // Rx_Position 1: схема смещена на один передатчик
        }

        return invert ? -1 : 1;
    }

    // Применение воздушной поправки и ориентации к сырой фазе.
    // Воспроизводит apply_rx_phase_orientation (function.cpp:47).
    public static float ApplyRxPhaseOrientation(float rawPhase, float airPhase, uint transmitterIndex, uint rxPosition)
    {
        var phase = NormalizePhase(rawPhase) - airPhase;
        if (RxPhaseOrientationSign(transmitterIndex, rxPosition) < 0)
        {
            phase = -phase;
        }

        return NormalizePhase(phase);
    }
}
