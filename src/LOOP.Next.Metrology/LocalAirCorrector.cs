using LoopNext.Core.Logging;
using LoopNext.Core.Models;

namespace LoopNext.Metrology;

// Собирает и применяет локальную воздушную поправку активного измерительного потока.
// Поведение соответствует sonde_read_timer: AIR хранит среднее значение ph_data и вычитается напрямую из ph_data.
public sealed class LocalAirCorrector
{
    // Количество кадров стабилизации перед накоплением baseline.
    public const int SettleFrames = 5;

    // Диапазон ручного ввода количества кадров локального воздуха.
    public const int DefaultSampleCount = 20;
    public const int MinSampleCount = 1;
    public const int MaxSampleCount = 200;

    private V10 _air;
    private readonly float[] _sum = new float[V10.ChannelCount];
    private readonly float[] _sumSq = new float[V10.ChannelCount];
    private readonly float[] _min = new float[V10.ChannelCount];
    private readonly float[] _max = new float[V10.ChannelCount];

    private bool _collecting;
    private bool _isSet;
    private bool _resetRequested;
    private int _sampleCount = DefaultSampleCount;
    private int _sampleCounter;
    private int _settleCounter;

    public LocalAirCorrector()
    {
        _air.Clear();
        ResetAccumulator();
    }

    public V10 Air => _air;

    public bool IsSet => _isSet;

    public bool Collecting => _collecting;

    public bool ResetRequested => _resetRequested;

    public int CollectedSampleCount => _sampleCounter;

    public static int ClampSampleCount(int requested)
    {
        if (requested < MinSampleCount)
        {
            return DefaultSampleCount;
        }

        return requested > MaxSampleCount ? MaxSampleCount : requested;
    }

    // Запускает сбор baseline после установки кольца в R0/C0.
    public void Begin(int sampleCount)
    {
        _sampleCount = ClampSampleCount(sampleCount);
        AppLogger.Write($"LOCAL_AIR: start, samples={_sampleCount}, settle={SettleFrames}");
        _air.Clear();
        ResetAccumulator();
        _collecting = true;
        _resetRequested = false;
    }

    public void RequestReset()
    {
        AppLogger.Write("LOCAL_AIR: reset requested.");
        _resetRequested = true;
    }

    // Обрабатывает кадр в порядке LOOP_NEW: сбор -> сброс -> вычитание AIR.
    public V10 ProcessFrame(in V10 phase, int channelCount)
    {
        CollectBaseline(in phase, channelCount);
        ApplyResetIfRequested(channelCount);
        return ApplyCorrection(in phase, channelCount);
    }

    private void CollectBaseline(in V10 phase, int channelCount)
    {
        if (!_collecting)
        {
            return;
        }

        if (_settleCounter < SettleFrames)
        {
            _settleCounter++;
            AppLogger.Write($"LOCAL_AIR: settling frame {_settleCounter}/{SettleFrames}");
        }
        else if (_sampleCounter < _sampleCount)
        {
            var count = Math.Min(channelCount, V10.ChannelCount);
            for (var i = 0; i < count; i++)
            {
                var sample = phase.GetValue(i);
                _sum[i] += sample;
                _sumSq[i] += sample * sample;
                if (sample < _min[i])
                {
                    _min[i] = sample;
                }

                if (sample > _max[i])
                {
                    _max[i] = sample;
                }
            }

            _sampleCounter++;
            AppLogger.Write($"LOCAL_AIR: collecting sample {_sampleCounter}/{_sampleCount}");
        }

        if (_sampleCounter == _sampleCount)
        {
            var count = Math.Min(channelCount, V10.ChannelCount);
            for (var i = 0; i < count; i++)
            {
                var mean = _sum[i] / _sampleCount;
                _air.SetValue(i, mean);

                var variance = _sumSq[i] / _sampleCount - mean * mean;
                if (variance < 0.0f)
                {
                    variance = 0.0f;
                }

                var stdDev = MathF.Sqrt(variance);
                AppLogger.Write(
                    $"LOCAL_AIR: channel={i}, mean_mG={mean * PhysicalConstants.Milligrad:F1}, std_mG={stdDev * PhysicalConstants.Milligrad:F1}, " +
                    $"min_mG={_min[i] * PhysicalConstants.Milligrad:F1}, max_mG={_max[i] * PhysicalConstants.Milligrad:F1}");
            }

            _collecting = false;
            _isSet = true;
            _sampleCounter = 0;
            _settleCounter = 0;

            var airValues = string.Join(", ", Enumerable.Range(0, count).Select(i => $"[{i}]={(_air.GetValue(i) * PhysicalConstants.Milligrad):F1}"));
            AppLogger.Write($"LOCAL_AIR: accepted_mG=[{airValues}]");
        }
    }

    private void ApplyResetIfRequested(int channelCount)
    {
        if (!_resetRequested)
        {
            return;
        }

        var count = Math.Min(channelCount, V10.ChannelCount);
        for (var i = 0; i < count; i++)
        {
            _air.SetValue(i, 0f);
        }

        _resetRequested = false;
        _isSet = false;
        _collecting = false;
        ResetAccumulator();
        AppLogger.Write("LOCAL_AIR: reset to zero.");
    }

    private V10 ApplyCorrection(in V10 phase, int channelCount)
    {
        var corrected = phase;
        var count = Math.Min(channelCount, V10.ChannelCount);
        for (var i = 0; i < count; i++)
        {
            corrected.SetValue(i, corrected.GetValue(i) - _air.GetValue(i));
        }

        return corrected;
    }

    private void ResetAccumulator()
    {
        _sampleCounter = 0;
        _settleCounter = 0;
        for (var i = 0; i < _sum.Length; i++)
        {
            _sum[i] = 0f;
            _sumSq[i] = 0f;
            _min[i] = float.MaxValue;
            _max[i] = float.MinValue;
        }
    }
}
