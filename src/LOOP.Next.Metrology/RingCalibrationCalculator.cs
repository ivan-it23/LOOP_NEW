using System.Numerics;
using LoopNext.Core.Models;

namespace LoopNext.Metrology;

public static class RingCalibrationCalculator
{
    public const double DefaultLoopInductance = 4.6e-6;
    public const double DefaultLoopRadius = 0.5;

    private const double CapacitanceMin = 500.0e-12;
    private const double CapacitanceMax = 70000.0e-12;
    private const double CapacitanceEpsilon = 1.0e-12;
    private const double WireResistance = 0.3;

    public static RingCalibrationValue Calculate(in SondeParameter parameter) =>
        Calculate(parameter, DefaultLoopRadius, DefaultLoopInductance);

    public static RingCalibrationValue Calculate(in SondeParameter parameter, double radius, double inductance)
    {
        // Без геометрии передатчика Loop() считает 0/0, сравнение NaN < NaN
        // ложно, и тернарный поиск сползает на нижнюю границу диапазона. Он
        // завершается, но отдаёт 500 пФ и отрицательную реактивность — числа,
        // неотличимые от расчётных по виду и негодные к записи в кольцо.
        if (!parameter.HasUsableGeometry)
        {
            return new RingCalibrationValue(float.NaN, float.NaN);
        }

        var capacitance = CMaxFi(parameter.Frequency, radius, inductance, parameter.L1, parameter.L2);
        var reactance = Z(1.0, inductance, capacitance, parameter.Frequency).Imaginary;
        return new RingCalibrationValue((float)capacitance, (float)reactance);
    }

    // Перенос C_max_fi (function.cpp:801): поиск емкости, максимизирующей фазу сигнала кольца.
    public static double CMaxFi(double frequency, double radius, double inductance, double l1, double l2)
    {
        // Та же защита для прямого вызова в обход Calculate.
        if (!(l1 > 0.0) || !(l2 > l1) || !(frequency > 0.0))
        {
            return double.NaN;
        }

        var left = CapacitanceMin;
        var right = CapacitanceMax;

        while (right - left > CapacitanceEpsilon)
        {
            var m1 = left + (right - left) / 3.0;
            var m2 = right - (right - left) / 3.0;

            if (Loop(1.0, m1, inductance, radius, frequency, l1, l2).Phase
                < Loop(1.0, m2, inductance, radius, frequency, l1, l2).Phase)
            {
                left = m1;
            }
            else
            {
                right = m2;
            }
        }

        return (left + right) / 2.0;
    }

    // Перенос Z (function.cpp:752), используется для RE_calc.
    public static Complex Z(double resistance, double inductance, double capacitance, double frequency)
    {
        var omega = 2.0 * PhysicalConstants.Pi * frequency;
        return new Complex(
            resistance + WireResistance,
            omega * inductance - 1.0 / (omega * capacitance));
    }

    // Перенос Loop (function.cpp:769), нужен только для C_max_fi.
    public static Complex Loop(
        double resistance,
        double capacitance,
        double inductance,
        double radius,
        double frequency,
        double l1,
        double l2)
    {
        var omega = 2.0 * PhysicalConstants.Pi * frequency;
        var k0 = PhysicalConstants.Pi * omega * 0.0000004 * PhysicalConstants.Pi;
        var zFull = new Complex(resistance, omega * inductance - 1.0 / (omega * capacitance));
        var q = Math.Pow(
            (Math.Pow((l2 + l1) / 2.0, 2.0) + radius * radius)
            * (Math.Pow((l2 - l1) / 2.0, 2.0) + radius * radius),
            1.5);

        var numerator = (2.0 * Complex.ImaginaryOne * zFull * q
                         + k0 * Math.Pow(radius, 4.0) * Math.Pow(l1, 3.0))
                        * Math.Pow(l2, 3.0);
        var denominator = (2.0 * Complex.ImaginaryOne * zFull * q
                           + k0 * Math.Pow(radius, 4.0) * Math.Pow(l2, 3.0))
                          * Math.Pow(l1, 3.0);

        return numerator / denominator;
    }
}

public readonly record struct RingCalibrationValue(float CapacitanceFarads, float Reactance);
