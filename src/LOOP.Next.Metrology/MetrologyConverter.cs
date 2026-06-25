using LoopNext.Core.Models;

namespace LoopNext.Metrology;

// Преобразование метрологии прибора в параметры зондов.
// Перенос metro_convert (function.cpp:81). Порядок элементов сохранён: внешний цикл по
// частоте, внутренний по передатчику, итоговый размер списка = 2 * n_tx при валидной сигнатуре.
public static class MetrologyConverter
{
    public const int FrequencyCount = 2;

    // Делитель геометрии: L1/L2 хранятся в миллиметрах *1000 (function.cpp:91-92).
    private const double GeometryScale = 1000.0;

    // Множитель частоты: F хранится в килогерцах (function.cpp:93).
    private const double FrequencyScale = 1000.0;

    public static IReadOnlyList<SondeParameter> Convert(in GpToolMetrology metrology)
    {
        var result = new List<SondeParameter>();
        var identity = ToolIdentity.Decode(metrology.Signature);

        // Параметры заполняются только при валидной сигнатуре, как в metro_convert (function.cpp:87).
        if (!identity.IsValid)
        {
            return result;
        }

        for (var freq = 0; freq < FrequencyCount; freq++)
        {
            for (var tx = 0; tx < identity.TransmitterCount; tx++)
            {
                result.Add(new SondeParameter
                {
                    L1 = (float)(metrology.GetL1(tx) / GeometryScale),
                    L2 = (float)(metrology.GetL2(tx) / GeometryScale),
                    Frequency = (float)(metrology.GetFrequency(freq) * FrequencyScale)
                });
            }
        }

        return result;
    }
}
