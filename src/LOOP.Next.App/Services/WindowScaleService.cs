namespace LOOP_Next_App.Services;

/// <summary>
/// Правила масштаба содержимого главного окна.
///
/// Приложение верстается в эффективных пикселях, поэтому на мониторе с большим
/// числом эффективных пикселей та же разметка занимает меньшую долю экрана и
/// элементы выглядят мелкими. Масштаб содержимого приводит размер элементов к
/// эталонной конфигурации разработки независимо от монитора пользователя.
///
/// Класс не зависит от WinUI.
/// </summary>
public static class WindowScaleService
{
    /// <summary>
    /// Эталонная рабочая область разработки, измеренная на месте: ноутбук
    /// 1920x1080 при системном масштабе Windows 125%, рабочая область без панели
    /// задач - 1920x1030 физических пикселей, то есть 1536x824 эффективных.
    /// Именно такой размер элементов считается правильным.
    ///
    /// Для монитора 1920x1080 при системном масштабе 100% отношения по обеим
    /// осям дают ровно 1.25, поэтому автоматический масштаб там равен 125%.
    /// </summary>
    public const double ReferenceContentWidth = 1536.0;

    public const double ReferenceContentHeight = 824.0;

    public const int MinimumAutomaticScalePercent = 100;

    public const int MaximumAutomaticScalePercent = 200;

    public const int MinimumScalePercent = 80;

    public const int MaximumScalePercent = 220;

    public const int MinimumScalePosition = -2;

    public const int DefaultScalePosition = 0;

    public const int MaximumScalePosition = 2;

    private const int ScaleStepPercent = 5;

    private const int UserAdjustmentStepPercent = 10;

    /// <summary>
    /// Пять позиций, предлагаемых пользователю: две меньше эталона,
    /// эталонная позиция и две больше эталона.
    /// </summary>
    public static IReadOnlyList<int> AvailableScalePositions { get; } =
        new[] { -2, -1, 0, 1, 2 };

    /// <summary>
    /// Масштаб по умолчанию для текущего монитора. Считается по доле рабочей
    /// области относительно эталонной: монитор, дающий больше эффективных
    /// пикселей, получает пропорционально более крупные элементы. Меньше 100%
    /// масштаб не опускается, чтобы на маленьких экранах разметка не сжималась.
    /// </summary>
    public static int GetAutomaticScalePercent(
        double effectiveWorkAreaWidth,
        double effectiveWorkAreaHeight)
    {
        if (!double.IsFinite(effectiveWorkAreaWidth) ||
            !double.IsFinite(effectiveWorkAreaHeight) ||
            effectiveWorkAreaWidth <= 0 ||
            effectiveWorkAreaHeight <= 0)
        {
            return MinimumAutomaticScalePercent;
        }

        // Ограничение по высоте не даёт содержимому вылезти за экран на
        // мониторах с широким, но низким соотношением сторон.
        var ratio = Math.Min(
            effectiveWorkAreaWidth / ReferenceContentWidth,
            effectiveWorkAreaHeight / ReferenceContentHeight);

        var roundedPercent = (int)Math.Round(
            ratio * 100.0 / ScaleStepPercent,
            MidpointRounding.AwayFromZero) * ScaleStepPercent;

        return Math.Clamp(
            roundedPercent,
            MinimumAutomaticScalePercent,
            MaximumAutomaticScalePercent);
    }

    /// <summary>
    /// Приводит сохранённую позицию к одному из пяти допустимых уровней.
    /// </summary>
    public static int NormalizeScalePosition(int scalePosition) =>
        Math.Clamp(scalePosition, MinimumScalePosition, MaximumScalePosition);

    /// <summary>
    /// Возвращает фактический процент для выбранной позиции относительно
    /// автоматического масштаба текущего монитора.
    /// </summary>
    public static int GetScalePercent(int automaticScalePercent, int scalePosition)
    {
        var normalizedAutomatic = Math.Clamp(
            automaticScalePercent,
            MinimumAutomaticScalePercent,
            MaximumAutomaticScalePercent);
        var normalizedPosition = NormalizeScalePosition(scalePosition);

        return NormalizeScalePercent(
            normalizedAutomatic + normalizedPosition * UserAdjustmentStepPercent);
    }

    /// <summary>
    /// Переводит сохранённый процент из предыдущей реализации в ближайшую
    /// из пяти относительных позиций.
    /// </summary>
    public static int GetClosestScalePosition(int automaticScalePercent, int scalePercent) =>
        AvailableScalePositions
            .OrderBy(position => Math.Abs(GetScalePercent(automaticScalePercent, position) - scalePercent))
            .ThenBy(position => Math.Abs(position))
            .First();

    /// <summary>
    /// Приводит сохранённое или вычисленное значение к допустимому диапазону.
    /// </summary>
    public static int NormalizeScalePercent(int scalePercent) =>
        Math.Clamp(scalePercent, MinimumScalePercent, MaximumScalePercent);

    /// <summary>
    /// Пересчитывает процент в множитель преобразования.
    /// </summary>
    public static double GetScaleFactor(int scalePercent) =>
        NormalizeScalePercent(scalePercent) / 100.0;

    /// <summary>
    /// Подпись одной из пяти позиций в диалоге выбора масштаба.
    /// </summary>
    public static string GetScaleOptionText(int scalePosition, int automaticScalePercent)
    {
        var normalizedPosition = NormalizeScalePosition(scalePosition);
        var scalePercent = GetScalePercent(automaticScalePercent, normalizedPosition);

        return normalizedPosition switch
        {
            -2 => $"{scalePercent}% — значительно меньше",
            -1 => $"{scalePercent}% — немного меньше",
            0 => $"{scalePercent}% — по умолчанию (рекомендуется)",
            1 => $"{scalePercent}% — немного больше",
            _ => $"{scalePercent}% — значительно больше"
        };
    }
}
