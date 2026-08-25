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

    // Постоянная часть конвейера пишется один раз за сеанс связи — см. ниже.
    private static bool _loggedPipelineConfiguration;

    /// <summary>Сбрасывает однократную диагностику при новом сеансе связи.</summary>
    public static void ResetSessionDiagnostics()
    {
        _loggedPipelineConfiguration = false;
    }

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

        // Одна запись на кадр вместо строки на канал: покадровая расшифровка
        // повторяла те же векторы по одному значению и на длинном проходе
        // заслоняла собой обмен с прибором. Знаки ориентации выводятся здесь же
        // компактным вектором — они и были единственным, чего не было в сводке.
        //
        // Состав конвейера от кадра к кадру не меняется, поэтому пишется один
        // раз за сессию.
        if (!_loggedPipelineConfiguration)
        {
            _loggedPipelineConfiguration = true;
            AppLogger.Write(
                $"PHASE_PIPELINE: signature=0x{data.Signature:X8}, Tx={identity.TransmitterCount}, " +
                $"Rx_Position={normalizedRx}, source=DELTA_PH, phase_smt_used=false, air_zz_used=false");
        }

        AppLogger.Write(
            $"PHASE_PIPELINE_VECTORS (mG): raw_normalized=[{FormatPhaseVector(rawPhases, identity.TransmitterCount)}] " +
            $"oriented=[{FormatPhaseVector(phases, identity.TransmitterCount)}] " +
            $"orientation_signs=[{FormatOrientationSigns(debug, identity.TransmitterCount)}]");

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

    private static string FormatOrientationSigns(PhaseOrientationDebug debug, uint transmitterCount)
    {
        var parts = new List<string>((int)(FrequencyCount * transmitterCount));
        for (uint freq = 0; freq < FrequencyCount; freq++)
        {
            for (uint tx = 0; tx < transmitterCount; tx++)
            {
                parts.Add($"F{freq}T{tx + 1}={debug.OrientationSign[freq, tx]:+0;-0}");
            }
        }

        return string.Join(", ", parts);
    }

    private static string FormatPhaseVector(V10 phases, uint transmitterCount)
    {
        return string.Join(", ", Enumerable.Range(0, (int)(FrequencyCount * transmitterCount))
            .Select(i => $"[{i}]={(phases.GetValue(i) * PhysicalConstants.Milligrad):F1}"));
    }
}
