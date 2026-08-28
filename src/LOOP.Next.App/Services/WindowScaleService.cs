namespace LOOP_Next_App.Services;

/// <summary>
/// Правила масштаба содержимого главного окна.
///
/// Содержимое верстается в эталонном кадре фиксированного размера, а затем
/// вписывается в фактическую рабочую область окна. Пользовательские 80–100%
/// применяются поверх вписывания и означают «мельче относительно вида, который
/// заполняет окно», а не абсолютный коэффициент.
///
/// Отдельная компенсация системного DPI не нужна: XAML измеряет элементы в
/// эффективных пикселях, поэтому одно и то же физическое окно 1920x1080 даёт
/// хосту 1536 эффективных пикселей при Windows 125% и 1920 при Windows 100%.
/// Вписывание в этот размер само даёт коэффициенты 1.0 и 1.25 соответственно,
/// то есть одинаковый вид, и вдобавок обслуживает экраны других размеров.
/// </summary>
public static class WindowScaleService
{
    /// <summary>
    /// Ширина эталонного кадра содержимого в эффективных пикселях.
    ///
    /// Эталоном принят вид на компьютере разработки: экран 1920x1080 при
    /// системном масштабе Windows 125%, окно развёрнуто. Рабочая область
    /// 1920 физических пикселей даёт 1920 / 1.25 = 1536 эффективных.
    /// Таблица измерений верстается звёздочными колонками, поэтому именно эта
    /// ширина задаёт эталонные пропорции колонок и подписей.
    /// </summary>
    public const double ReferenceContentWidth = 1536.0;

    /// <summary>
    /// Высота эталонного кадра содержимого в эффективных пикселях: столько
    /// занимает рабочая часть страницы от вкладок до кнопки `Рапорт` на
    /// эталонном компьютере. Значение намеренно меньше полной высоты рабочей
    /// области, потому что содержимое страницы выровнено по верху и не
    /// растягивается: свободный остаток внизу эталонного вида в кадр не входит.
    /// </summary>
    public const double ReferenceContentHeight = 712.0;

    /// <summary>
    /// Системный масштаб Windows на эталонном компьютере. Хранится как
    /// пояснение к происхождению эталонного кадра и как безопасное значение
    /// диагностики до готовности XamlRoot; в расчёт масштаба не входит.
    /// </summary>
    public const double ReferenceRasterizationScale = 1.25;

    /// <summary>
    /// Границы автоматического вписывания. Нужны только как защита от
    /// вырожденных размеров окна; на реальных экранах не срабатывают.
    /// </summary>
    public const double MinimumAutoFitFactor = 0.5;

    public const double MaximumAutoFitFactor = 3.0;

    public const int MinimumScalePercent = 80;

    public const int DefaultScalePercent = 100;

    public const int MaximumScalePercent = 100;

    /// <summary>
    /// Первая версия настроек, в которой процент означает относительный выбор
    /// пользователя, а не абсолютный RenderTransform из версии 1.1.3.
    /// </summary>
    public const int CurrentSettingsSchemaVersion = 1;

    private const int ScaleStepPercent = 10;

    /// <summary>
    /// Единственные значения, которые разрешено показывать и применять.
    ///
    /// Верхняя граница - 100%, и это следствие вписывания в окно, а не
    /// произвольное ограничение. При 100% эталонный кадр уже занимает всю
    /// ширину рабочей области. Выбор больше 100% сужает область вёрстки, потому
    /// что кадру достаётся ширина окна, делённая на масштаб: 110% оставляют
    /// таблице 1396 эффективных пикселей вместо 1536, а 120% - 1280. Таблица
    /// измерений ниже своей минимальной ширины не сжимается, и правые
    /// сопротивления уходили за границу окна; горизонтальная прокрутка у неё
    /// намеренно отключена, поэтому вернуть их пользователь не мог. Значения
    /// ниже 100% безопасны: кадр становится шире, а таблица растягивается.
    /// </summary>
    public static IReadOnlyList<int> AvailableScalePercents { get; } =
        new[] { 80, 90, 100 };

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
    /// Читает настройку только из текущей схемы. Более старые версии приложения
    /// сохраняли под теми же значениями другую физическую величину, поэтому при
    /// первой миграции безопасно возвращаем эталонные 100%.
    /// </summary>
    public static int ResolveSavedScalePercent(int? settingsSchemaVersion, int? savedScalePercent) =>
        settingsSchemaVersion == CurrentSettingsSchemaVersion && savedScalePercent is int percent
            ? NormalizeScalePercent(percent)
            : DefaultScalePercent;

    /// <summary>
    /// Коэффициент вписывания эталонного кадра в рабочую область окна.
    /// Берётся меньшее из отношений по ширине и высоте, поэтому содержимое
    /// заполняет окно хотя бы по одной стороне и никогда не обрезается по
    /// другой. Размеры принимаются в эффективных пикселях.
    /// </summary>
    public static double GetAutoFitFactor(double hostWidth, double hostHeight)
    {
        if (!double.IsFinite(hostWidth) || !double.IsFinite(hostHeight) ||
            hostWidth <= 0 || hostHeight <= 0)
        {
            return 1.0;
        }

        var fit = Math.Min(
            hostWidth / ReferenceContentWidth,
            hostHeight / ReferenceContentHeight);

        return Math.Clamp(fit, MinimumAutoFitFactor, MaximumAutoFitFactor);
    }

    public static int GetScaleOptionIndex(int scalePercent) =>
        (NormalizeScalePercent(scalePercent) - MinimumScalePercent) / ScaleStepPercent;

    public static double GetScaleFactor(int scalePercent) =>
        NormalizeScalePercent(scalePercent) / 100.0;

    /// <summary>
    /// Итоговый коэффициент RenderTransform: вписывание в окно, умноженное на
    /// относительный выбор пользователя. На эталонном компьютере при 100%
    /// возвращает 1.0, то есть эталонный вид воспроизводится в точности.
    /// </summary>
    public static double GetEffectiveScaleFactor(int scalePercent, double hostWidth, double hostHeight) =>
        GetScaleFactor(scalePercent) * GetAutoFitFactor(hostWidth, hostHeight);

    public static string GetScaleOptionText(int scalePercent) =>
        $"{NormalizeScalePercent(scalePercent)}%";

    /// <summary>
    /// Базовый размер шрифта всплывающей подсказки WinUI в эффективных пикселях.
    /// </summary>
    public const double ToolTipBaseFontSize = 12.0;

    /// <summary>
    /// Размер шрифта подсказки при текущем масштабе содержимого.
    ///
    /// Подсказки живут в popup-корне `XamlRoot`, поэтому масштаб главного кадра
    /// на них не действует и их приходится приводить отдельно. Приводим размером
    /// шрифта, а не `RenderTransform`: `Popup` располагает и обрезает подсказку по
    /// её неотмасштабированной разметке, поэтому трансформация увеличивала текст,
    /// не увеличивая коробку, и длинные подсказки обрезались по правому краю.
    /// От размера шрифта коробка измеряется сама, поэтому позиция и границы
    /// остаются верными.
    /// </summary>
    public static double GetToolTipFontSize(double contentScaleFactor)
    {
        if (!double.IsFinite(contentScaleFactor) || contentScaleFactor <= 0)
        {
            return ToolTipBaseFontSize;
        }

        return ToolTipBaseFontSize * contentScaleFactor;
    }
}
