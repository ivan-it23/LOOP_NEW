using LoopNext.Core.Logging;
using LoopNext.Core.Models;

namespace LoopNext.Metrology;

// Сбор и применение локальной воздушной поправки.
// Точный перенос поведения button_local_air_Click и блока локального воздуха в
// sonde_read_timer (WorkForm.h:6033-6065, 6166-6242), включая ResetLocalAirAccumulator
// и GetLocalAirSampleCountFromUi (WorkForm.h:5431-5455).
//
// Класс хранит только расчётное состояние поправки; запуск кольца (Loop_work) и опрос
// прибора остаются ответственностью вышележащих слоёв.
public sealed class LocalAirCorrector
{
    // Кадры стабилизации перед накоплением (LOCAL_AIR_SETTLE_FRAMES, WorkForm.h:5418).
    public const int SettleFrames = 5;

    // Значения выбора количества кадров и диапазон ручного ввода (WorkForm.h:5439-5440).
    public const int DefaultSampleCount = 20;
    public const int MinSampleCount = 1;
    public const int MaxSampleCount = 200;

    private V10 _air;
    private readonly float[] _sum = new float[V10.ChannelCount];

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

    // Текущая воздушная поправка (baseline) по каналам.
    public V10 Air => _air;

    public bool IsSet => _isSet;

    public bool Collecting => _collecting;

    // Нормализация запрошенного количества кадров: fallback 20 при значении < 1, потолок 200.
    public static int ClampSampleCount(int requested)
    {
        if (requested < MinSampleCount)
        {
            return DefaultSampleCount;
        }

        return requested > MaxSampleCount ? MaxSampleCount : requested;
    }

    // Запуск сбора baseline: обнуление поправки и аккумулятора (соответствует ветке
    // button_local_air_Click при local_air_is_set == false после успешного Loop_work).
    public void Begin(int sampleCount)
    {
        _sampleCount = ClampSampleCount(sampleCount);
        AppLogger.Write($"LOCAL_AIR: начало сбора, samples={_sampleCount}, settle={SettleFrames}");
        _air.Clear();
        ResetAccumulator();
        _collecting = true;
        _resetRequested = false;
    }

    // Запрос сброса существующей поправки (ветка local_air_is_set == true).
    public void RequestReset()
    {
        AppLogger.Write("LOCAL_AIR: запрошен сброс текущей поправки.");
        _resetRequested = true;
    }

    // Обработка одного кадра: сбор baseline, обработка сброса и применение поправки.
    // Порядок блоков сохранён из sonde_read_timer: сбор -> сброс -> вычитание AIR.
    public V10 ProcessFrame(in V10 phaseData, int channelCount)
    {
        CollectBaseline(in phaseData, channelCount);
        ApplyResetIfRequested(channelCount);
        return ApplyCorrection(in phaseData, channelCount);
    }

    private void CollectBaseline(in V10 phaseData, int channelCount)
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
                _sum[i] += phaseData.GetValue(i);
            }

            _sampleCounter++;
            AppLogger.Write($"LOCAL_AIR: collecting sample {_sampleCounter}/{_sampleCount}");
        }

        if (_sampleCounter == _sampleCount)
        {
            var count = Math.Min(channelCount, V10.ChannelCount);
            var airValues = string.Join(", ", Enumerable.Range(0, count).Select(i => $"[{i}]={(_sum[i] / _sampleCount * PhysicalConstants.Milligrad):F1}"));
            for (var i = 0; i < count; i++)
            {
                _air.SetValue(i, _sum[i] / _sampleCount);
            }

            _collecting = false;
            _isSet = true;
            _sampleCounter = 0;
            _settleCounter = 0;
            AppLogger.Write($"LOCAL_AIR: поправка установлена (мГ): {airValues}");
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
        AppLogger.Write("LOCAL_AIR: поправка сброшена.");
    }

    private V10 ApplyCorrection(in V10 phaseData, int channelCount)
    {
        var corrected = phaseData;
        var count = Math.Min(channelCount, V10.ChannelCount);
        for (var i = 0; i < count; i++)
        {
            corrected.SetValue(i, corrected.GetValue(i) - _air.GetValue(i));
        }

        return corrected;
    }

    // Обнуление счётчиков и накопителя (ResetLocalAirAccumulator, WorkForm.h:5446).
    private void ResetAccumulator()
    {
        _sampleCounter = 0;
        _settleCounter = 0;
        for (var i = 0; i < _sum.Length; i++)
        {
            _sum[i] = 0f;
        }
    }
}
