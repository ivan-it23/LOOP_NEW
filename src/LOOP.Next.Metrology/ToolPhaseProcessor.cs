using LoopNext.Core.Models;

namespace LoopNext.Metrology;

// Преобразование кадра прибора в ориентированные фазы.
// Перенос fill_phase_from_tool_data (function.cpp:53). Глобальная зависимость от
// metrology_struct.Rx_Position заменена явным параметром rxPosition, поведение сохранено.
public static class ToolPhaseProcessor
{
    public const int FrequencyCount = 2;

    // Результат заполнения фаз: значения каналов и диагностика ориентации.
    public readonly record struct PhaseResult(V10 Phases, PhaseOrientationDebug Debug);

    // Заполняет V10 ориентированными фазами из GP_TOOL_DATA с учётом Rx_Position.
    // Индексация канала idx = freq * n_tx + Tx сохранена из исходного кода.
    public static PhaseResult FillPhaseFromToolData(in GpToolData data, uint rxPosition)
    {
        var phases = default(V10);
        phases.Clear();

        var identity = ToolIdentity.Decode(data.Signature);
        var normalizedRx = PhaseMath.NormalizedRxPosition(rxPosition);

        var debug = new PhaseOrientationDebug
        {
            Valid = true,
            Signature = data.Signature,
            TransmitterCount = identity.TransmitterCount,
            RxPosition = normalizedRx
        };

        for (uint freq = 0; freq < FrequencyCount; freq++)
        {
            for (uint tx = 0; tx < identity.TransmitterCount; tx++)
            {
                var idx = (int)(freq * identity.TransmitterCount + tx);
                var rawDelta = data.GetDeltaPhase((int)freq, (int)tx);
                var sign = PhaseMath.RxPhaseOrientationSign(tx, normalizedRx);
                var finalPhase = PhaseMath.ApplyRxPhaseOrientation(rawDelta, 0.0f, tx, normalizedRx);

                phases.SetValue(idx, finalPhase);

                debug.RawDelta[freq, tx] = rawDelta;
                debug.RawNormalized[freq, tx] = PhaseMath.NormalizePhase(rawDelta);
                debug.OrientationSign[freq, tx] = sign;
                debug.FinalPhase[freq, tx] = finalPhase;
            }
        }

        return new PhaseResult(phases, debug);
    }
}
