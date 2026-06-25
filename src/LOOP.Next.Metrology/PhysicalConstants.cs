namespace LoopNext.Metrology;

// Физические и геометрические константы, перенесённые из variable.h старого проекта.
// Значение Pi сохранено как точная float-аппроксимация исходного кода (variable.h:55),
// чтобы нормализация фаз воспроизводила прежнее поведение бит-в-бит на границах диапазона.
public static class PhysicalConstants
{
    public const double Pi = 3.1415927410125732;

    public const double DegreesPerRadian = 180.0 / Pi;

    // Коэффициент перевода фазы из радиан в милли-градусы (mG, variable.h).
    public const double Milligrad = 1000.0 * DegreesPerRadian;

    // Коэффициент piomegamu0 для расчёта фазы кольца (function.cpp:108).
    // В эталоне это double-литерал, результат приводится к float после умножения.
    public const double LoopPhaseFrequencyFactor = 0.00002480502;

    // Коэффициент omega*mu0 для расчёта фазы среды (function.cpp:117).
    // В эталоне это float-литерал 0.0000078957f, поэтому весь расчёт ведётся во float.
    public const float MediumPhaseFrequencyFactor = 0.0000078957f;
}
