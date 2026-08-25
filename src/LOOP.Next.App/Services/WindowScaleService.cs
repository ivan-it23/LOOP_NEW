namespace LOOP_Next_App.Services;

/// <summary>
/// Единые правила масштаба содержимого главного окна.
///
/// Значения намеренно абсолютные и не зависят от размера монитора или
/// системного DPI: на каждом компьютере пользователь видит один и тот же
/// фиксированный набор вариантов.
/// </summary>
public static class WindowScaleService
{
    public const int MinimumScalePercent = 80;

    public const int DefaultScalePercent = 100;

    public const int MaximumScalePercent = 120;

    private const int ScaleStepPercent = 10;

    private const int MinimumLegacyScalePosition = -2;

    private const int MaximumLegacyScalePosition = 2;

    /// <summary>
    /// Единственные значения, которые разрешено показывать и применять.
    /// </summary>
    public static IReadOnlyList<int> AvailableScalePercents { get; } =
        new[] { 80, 90, 100, 110, 120 };

    /// <summary>
    /// Приводит произвольное или сохранённое значение к ближайшему допустимому.
    /// При равном расстоянии выбирается вариант, ближайший к 100%.
    /// </summary>
    public static int NormalizeScalePercent(int scalePercent) =>
        AvailableScalePercents
            .OrderBy(percent => Math.Abs(percent - scalePercent))
            .ThenBy(percent => Math.Abs(percent - DefaultScalePercent))
            .First();

    /// <summary>
    /// Переносит относительную позицию из версии 1.1.1 в абсолютный процент.
    /// </summary>
    public static int ConvertLegacyScalePosition(int scalePosition)
    {
        var normalizedPosition = Math.Clamp(
            scalePosition,
            MinimumLegacyScalePosition,
            MaximumLegacyScalePosition);

        return DefaultScalePercent + normalizedPosition * ScaleStepPercent;
    }

    public static int GetScaleOptionIndex(int scalePercent) =>
        (NormalizeScalePercent(scalePercent) - MinimumScalePercent) / ScaleStepPercent;

    public static double GetScaleFactor(int scalePercent) =>
        NormalizeScalePercent(scalePercent) / 100.0;

    public static string GetScaleOptionText(int scalePercent) =>
        $"{NormalizeScalePercent(scalePercent)}%";
}
