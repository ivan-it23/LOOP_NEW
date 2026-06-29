using LoopNext.Core.Logging;
using LoopNext.Core.Models;

namespace LoopNext.Metrology;

// Преобразует кадр прибора в рабочие фазы активного измерительного потока.
// DELTA_PH from the tool is already normalized by firmware, but Tx sign orientation
// is applied by the firmware only when it builds phase_smt. The active app path uses
// DELTA_PH, so it applies the Rx_Position sign here before local-air correction.
public static class ToolPhaseProcessor
{
    public const int FrequencyCount = 2;

    // RawPhases хранит нормализованный DELTA_PH до ориентации знака; Phases хранит рабочий ориентированный поток.
    public readonly record struct PhaseResult(V10 Phases, V10 RawPhases, PhaseOrientationDebug Debug);

    // Заполняет V10 фазами DELTA_PH с ориентацией знака Tx/Rx_Position.
    public static PhaseResult FillPhaseFromToolData(in GpToolData data, in GpToolMetrology metrology)
    {
        var identity = ToolIdentity.Decode(data.Signature);
        var normalizedRx = PhaseMath.NormalizedRxPosition(metrology.RxPosition);
        var phases = FillRawPhaseFromToolData(in data, normalizedRx);
        var rawPhases = FillNormalizedDeltaPhaseFromToolData(in data);

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
                var orientationSign = PhaseMath.RxPhaseOrientationSign(tx, normalizedRx);
                var finalPhase = phases.GetValue(idx);

                debug.RawDelta[freq, tx] = rawDelta;
                debug.RawNormalized[freq, tx] = PhaseMath.NormalizePhase(rawDelta);
                debug.OrientationSign[freq, tx] = orientationSign;
                debug.FinalPhase[freq, tx] = finalPhase;
            }
        }

        var rawValues = FormatPhaseVector(rawPhases, identity.TransmitterCount);
        var orientedValues = FormatPhaseVector(phases, identity.TransmitterCount);
        AppLogger.Write(
            $"PHASE_PIPELINE: signature=0x{data.Signature:X8}, Tx={identity.TransmitterCount}, Rx={normalizedRx}, " +
            $"source=DELTA_PH, raw_normalized_mG=[{rawValues}], oriented_mG=[{orientedValues}], phase_smt_used=false, air_zz_used=false");
        for (uint freq = 0; freq < FrequencyCount; freq++)
        {
            for (uint tx = 0; tx < identity.TransmitterCount; tx++)
            {
                var orientationSign = debug.OrientationSign[freq, tx];
                AppLogger.Write(
                    $"PHASE_PIPELINE_CH: F{freq} Tx{tx + 1}, raw_delta_mG={(debug.RawDelta[freq, tx] * PhysicalConstants.Milligrad):F1}, " +
                    $"normalized_mG={(debug.RawNormalized[freq, tx] * PhysicalConstants.Milligrad):F1}, orientation_sign={orientationSign}, " +
                    $"oriented_mG={(debug.FinalPhase[freq, tx] * PhysicalConstants.Milligrad):F1}");
            }
        }

        return new PhaseResult(phases, rawPhases, debug);
    }

    // Формирует фазы без симметризации и без Air_zz; знак Tx/Rx_Position нужен для DELTA_PH.
    private static V10 FillRawPhaseFromToolData(in GpToolData data, uint rxPosition)
    {
        var phases = default(V10);
        phases.Clear();

        var identity = ToolIdentity.Decode(data.Signature);
        for (uint freq = 0; freq < FrequencyCount; freq++)
        {
            for (uint tx = 0; tx < identity.TransmitterCount; tx++)
            {
                var idx = (int)(freq * identity.TransmitterCount + tx);
                var rawDelta = data.GetDeltaPhase((int)freq, (int)tx);
                var finalPhase = PhaseMath.ApplyRxPhaseOrientation(rawDelta, 0.0f, tx, rxPosition);

                phases.SetValue(idx, finalPhase);
            }
        }

        return phases;
    }

    private static V10 FillNormalizedDeltaPhaseFromToolData(in GpToolData data)
    {
        var phases = default(V10);
        phases.Clear();

        var identity = ToolIdentity.Decode(data.Signature);
        for (uint freq = 0; freq < FrequencyCount; freq++)
        {
            for (uint tx = 0; tx < identity.TransmitterCount; tx++)
            {
                var idx = (int)(freq * identity.TransmitterCount + tx);
                var rawDelta = data.GetDeltaPhase((int)freq, (int)tx);
                phases.SetValue(idx, PhaseMath.NormalizePhase(rawDelta));
            }
        }

        return phases;
    }

    private static string FormatPhaseVector(V10 phases, uint transmitterCount)
    {
        return string.Join(", ", Enumerable.Range(0, (int)(FrequencyCount * transmitterCount))
            .Select(i => $"[{i}]={(phases.GetValue(i) * PhysicalConstants.Milligrad):F1}"));
    }
}
