using LoopNext.Core.Models;

namespace LoopNext.Metrology;

// Расчёты фаз и кажущегося УЭС по сигналам прибора и параметрам кольца.
// Точный перенос dfi_from_loop, dfi_ro, ro_dfi (function.cpp:101-160).
// Вся комплексная арифметика выполняется во float через ComplexF, как в эталонном коде;
// порядок промежуточных приведений к float сохранён для совпадения результатов.
public static class LoopResistivity
{
    // Параметры метода золотого сечения для ro_dfi (function.cpp:124-131).
    private const float EpsilonArg = 0.0000005f;
    private const float EpsilonInf = 0.000000005f;
    private const float RoMin = 0.01f;
    private const float RoMax = 7000.0f;
    private const double GoldenRatio = 0.382;

    // Возврат при INF-подобной остановке поиска (function.cpp:145).
    private const float RoInfStop = 7200.0f;

    private const float FrequencyThreshold = 1_000_000.0f;

    // Фаза, наводимая кольцом. Перенос dfi_from_loop (function.cpp:101).
    public static float DfiFromLoop(in SondeParameter param, in LoopParameter loop)
    {
        // Полное сопротивление кольца зависит от частотного диапазона.
        // При F == FrequencyThreshold обе ветви пропускаются, как в исходном коде.
        var zFull = new ComplexF(0f, 0f);
        if (param.Frequency < FrequencyThreshold)
        {
            zFull = new ComplexF(loop.Resistance + 0.3f, loop.Reactance);
        }

        if (param.Frequency > FrequencyThreshold)
        {
            zFull = new ComplexF(loop.Resistance + 0.5f, loop.Reactance);
        }

        // piomegamu0 вычисляется через double-литерал и приводится к float (function.cpp:108).
        var piOmegaMu0 = (float)(PhysicalConstants.LoopPhaseFrequencyFactor * param.Frequency);

        var halfSum = (param.L2 + param.L1) / 2f;
        var halfDiff = (param.L2 - param.L1) / 2f;
        var q = (float)Math.Pow(
            (Math.Pow(halfSum, 2) + Math.Pow(loop.Radius, 2))
            * (Math.Pow(halfDiff, 2) + Math.Pow(loop.Radius, 2)),
            1.5);

        // Скалярные слагаемые приводятся к float после умножения в double, как в исходном выражении.
        var l1Term = (float)((double)piOmegaMu0 * Math.Pow(loop.Radius, 4) * Math.Pow(param.L1, 3));
        var l2Term = (float)((double)piOmegaMu0 * Math.Pow(loop.Radius, 4) * Math.Pow(param.L2, 3));
        var l1Cubed = (float)Math.Pow(param.L1, 3);
        var l2Cubed = (float)Math.Pow(param.L2, 3);

        var twoIzQ = new ComplexF(0f, 2f) * zFull * q;
        var numerator = (twoIzQ + l1Term) * l2Cubed;
        var denominator = (twoIzQ + l2Term) * l1Cubed;

        return (numerator / denominator).Arg();
    }

    // Фаза, наводимая средой с удельным сопротивлением ro. Перенос dfi_ro (function.cpp:116).
    public static float DfiRo(in SondeParameter param, float ro)
    {
        var omegaMu0Sigma = PhysicalConstants.MediumPhaseFrequencyFactor * param.Frequency / ro;
        var ik = ComplexF.ImaginaryOne * ComplexF.Sqrt(new ComplexF(0f, omegaMu0Sigma));
        var sgn = ComplexF.Exp(ik * (param.L2 - param.L1))
                  * ((1f - ik * param.L2) / (1f - ik * param.L1));
        return sgn.Arg();
    }

    // Кажущееся УЭС по фазе методом золотого сечения. Перенос ro_dfi (function.cpp:123).
    public static float RoDfi(in SondeParameter param, float phaseFromLoop)
    {
        var ro0 = RoMin;
        var roMax = RoMax;
        var stopArg = false;
        var stopInf = false;
        var result = 0.0f;

        do
        {
            var x1 = (float)(ro0 + GoldenRatio * (roMax - ro0));
            var x2 = (float)(roMax - GoldenRatio * (roMax - ro0));
            var a = phaseFromLoop - DfiRo(param, x1);
            var b = phaseFromLoop - DfiRo(param, x2);

            if (MathF.Abs(a) > MathF.Abs(b))
            {
                ro0 = x1;
            }
            else
            {
                roMax = x2;
            }

            if (MathF.Abs(a) < EpsilonArg || MathF.Abs(b) < EpsilonArg)
            {
                stopArg = true;
            }
            else if (MathF.Abs(a - b) < EpsilonInf)
            {
                stopInf = true;
            }
        }
        while (!stopArg && !stopInf);

        if (stopArg)
        {
            result = (ro0 + roMax) / 2f;
        }

        if (stopInf)
        {
            result = RoInfStop;
        }

        return result;
    }

    // Кажущееся УЭС по всем каналам прибора. Перенос ro_dfi(vector, V10) (function.cpp:153).
    // Заполняются только каналы, для которых определён параметр зонда; остальные остаются нулевыми.
    public static V10 RoDfi(IReadOnlyList<SondeParameter> sondeParams, in V10 phaseFromLoop)
    {
        var roData = default(V10);
        roData.Clear();

        var count = Math.Min(sondeParams.Count, V10.ChannelCount);
        for (var i = 0; i < count; i++)
        {
            var param = sondeParams[i];
            roData.SetValue(i, RoDfi(param, phaseFromLoop.GetValue(i)));
        }

        return roData;
    }

    // Расчет фаз по сопротивлениям кольца. Перенос calc_PH_from_loop (function.cpp:163).
    // Индекс 0 сохраняется как пустой "воздух", R1..R8 лежат в индексах 1..8, как в старом PH_computed.
    public static IReadOnlyList<V10> CalcPhaseFromLoop(
        IReadOnlyList<SondeParameter> sondeParams,
        IReadOnlyList<LoopParameter> loopParams)
    {
        var computed = new List<V10>(MeasurementTable.ResistanceCount + 1);

        var air = default(V10);
        air.Clear();
        computed.Add(air);

        for (var resistanceIndex = 0; resistanceIndex < MeasurementTable.ResistanceCount; resistanceIndex++)
        {
            var values = default(V10);
            values.Clear();

            if (resistanceIndex >= loopParams.Count)
            {
                computed.Add(values);
                continue;
            }

            var count = Math.Min(sondeParams.Count, V10.ChannelCount);
            for (var i = 0; i < count; i++)
            {
                if (i >= loopParams.Count)
                {
                    continue;
                }

                var loop = loopParams[resistanceIndex];
                loop.Reactance = loopParams[i].Reactance;
                values.SetValue(i, DfiFromLoop(sondeParams[i], loop));
            }

            computed.Add(values);
        }

        return computed;
    }

    // Расчет УЭС по фазам кольца. Перенос calc_Ro_from_loop (function.cpp:182).
    public static IReadOnlyList<V10> CalcRoFromLoop(
        IReadOnlyList<SondeParameter> sondeParams,
        IReadOnlyList<V10> phaseComputed)
    {
        var computed = new List<V10>(phaseComputed.Count);
        foreach (var phase in phaseComputed)
        {
            computed.Add(RoDfi(sondeParams, phase));
        }

        return computed;
    }
}
