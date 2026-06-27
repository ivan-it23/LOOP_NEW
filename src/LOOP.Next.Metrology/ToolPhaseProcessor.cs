using LoopNext.Core.Logging;
using LoopNext.Core.Models;

namespace LoopNext.Metrology;

// Преобразует кадр прибора в рабочие фазы активного измерительного потока.
// Поведение соответствует fill_phase_from_tool_data: DELTA_PH, ориентация Tx/Rx_Position, без Air_zz и симметризации.
public static class ToolPhaseProcessor
{
    public const int FrequencyCount = 2;

    // RawPhases оставлен для совместимости вызывающего кода; в активном пути он совпадает с Phases.
    public readonly record struct PhaseResult(V10 Phases, V10 RawPhases, PhaseOrientationDebug Debug);

    // Заполняет V10 фазами из GP_TOOL_DATA по живой логике LOOP_NEW.
    public static PhaseResult FillPhaseFromToolData(in GpToolData data, in GpToolMetrology metrology)
    {
        var phases = FillRawPhaseFromToolData(in data, metrology.RxPosition);
        var rawPhases = phases;

        var identity = ToolIdentity.Decode(data.Signature);
        var normalizedRx = PhaseMath.NormalizedRxPosition(metrology.RxPosition);

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
                var finalPhase = phases.GetValue(idx);

                debug.RawDelta[freq, tx] = rawDelta;
                debug.RawNormalized[freq, tx] = PhaseMath.NormalizePhase(rawDelta);
                debug.OrientationSign[freq, tx] = sign;
                debug.FinalPhase[freq, tx] = finalPhase;
            }
        }

        var phaseValues = string.Join(", ", Enumerable.Range(0, (int)(FrequencyCount * identity.TransmitterCount))
            .Select(i => $"[{i}]={(phases.GetValue(i) * PhysicalConstants.Milligrad):F1}"));
        AppLogger.Write($"PHASE_ORIENTATION: signature=0x{data.Signature:X8}, Tx={identity.TransmitterCount}, Rx={normalizedRx}, final_mG=[{phaseValues}]");
        for (uint freq = 0; freq < FrequencyCount; freq++)
        {
            for (uint tx = 0; tx < identity.TransmitterCount; tx++)
            {
                AppLogger.Write($"  F{freq} Tx{tx}: raw={(debug.RawDelta[freq, tx] * PhysicalConstants.Milligrad):F1}, sign={debug.OrientationSign[freq, tx]}, final={(debug.FinalPhase[freq, tx] * PhysicalConstants.Milligrad):F1}");
            }
        }

        return new PhaseResult(phases, rawPhases, debug);
    }

    // Формирует ориентированные фазы без симметризации и без Air_zz.
    private static V10 FillRawPhaseFromToolData(in GpToolData data, uint rxPosition)
    {
        var phases = default(V10);
        phases.Clear();

        var identity = ToolIdentity.Decode(data.Signature);
        var normalizedRx = PhaseMath.NormalizedRxPosition(rxPosition);

        for (uint freq = 0; freq < FrequencyCount; freq++)
        {
            for (uint tx = 0; tx < identity.TransmitterCount; tx++)
            {
                var idx = (int)(freq * identity.TransmitterCount + tx);
                var rawDelta = data.GetDeltaPhase((int)freq, (int)tx);
                var finalPhase = PhaseMath.ApplyRxPhaseOrientation(rawDelta, 0.0f, tx, normalizedRx);

                phases.SetValue(idx, finalPhase);
            }
        }

        return phases;
    }
}
