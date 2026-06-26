using LoopNext.Core.Logging;
using LoopNext.Core.Models;

namespace LoopNext.Metrology;

// Преобразование кадра прибора в симметризованные фазы.
// Перенос get_phase_smt (function.cpp:684). Фазы нормализуются к Air_zz,
// ориентируются по Tx/Rx_Position и умножаются на матрицу симметризации.
public static class ToolPhaseProcessor
{
    public const int FrequencyCount = 2;

    // Результат заполнения фаз: симметризованные фазы, сырые ориентированные фазы
    // и диагностика ориентации. Сырые фазы нужны для локальной воздушной поправки,
    // которая в эталонном проекте накапливается до симметризации (get_air_smt).
    public readonly record struct PhaseResult(V10 Phases, V10 RawPhases, PhaseOrientationDebug Debug);

    // Заполняет V10 фазами из GP_TOOL_DATA.
    // Симметризованные фазы — перенос get_phase_smt (function.cpp:684).
    // Сырые ориентированные фазы — перенос fill_phase_from_tool_data (function.cpp:53).
    public static PhaseResult FillPhaseFromToolData(in GpToolData data, in GpToolMetrology metrology)
    {
        var phases = PhaseSymmetrizer.GetPhaseSmt(in data, in metrology);
        var rawPhases = FillRawPhaseFromToolData(in data, metrology.RxPosition);

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
        var rawValues = string.Join(", ", Enumerable.Range(0, (int)(FrequencyCount * identity.TransmitterCount))
            .Select(i => $"[{i}]={(rawPhases.GetValue(i) * PhysicalConstants.Milligrad):F1}"));
        AppLogger.Write($"PHASE_ORIENTATION: сигнатура=0x{data.Signature:X8}, Tx={identity.TransmitterCount}, Rx={normalizedRx}, sym (мГ)=[{phaseValues}], raw (мГ)=[{rawValues}]");
        for (uint freq = 0; freq < FrequencyCount; freq++)
        {
            for (uint tx = 0; tx < identity.TransmitterCount; tx++)
            {
                AppLogger.Write($"  F{freq} Tx{tx}: raw={(debug.RawDelta[freq, tx] * PhysicalConstants.Milligrad):F1}, sign={debug.OrientationSign[freq, tx]}, smt={(debug.FinalPhase[freq, tx] * PhysicalConstants.Milligrad):F1}");
            }
        }

        return new PhaseResult(phases, rawPhases, debug);
    }

    // Сырые ориентированные фазы без симметризации.
    // Перенос fill_phase_from_tool_data (function.cpp:53).
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
