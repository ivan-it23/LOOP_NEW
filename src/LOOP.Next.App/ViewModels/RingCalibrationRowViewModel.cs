using CommunityToolkit.Mvvm.ComponentModel;

namespace LOOP_Next_App.ViewModels;

/// <summary>
/// Откуда взялось значение в поле «Реакт. изм.».
/// </summary>
/// <remarks>
/// Поле называется измеренным, но заполняется тремя разными путями: чтением из
/// кольца при подключении, расчётом по метрологии прибора и вводом с
/// клавиатуры. Пока происхождение не различалось, «Рассчитать» защищала от
/// перезаписи любое непустое значение, то есть в первую очередь то, что уже
/// лежит в кольце. Получался замкнутый круг: приложение возвращало кольцу его
/// же содержимое, и единственным выходом была ручная очистка поля.
///
/// Защищать нужно ровно одно - введённое человеком: это и есть измерение из
/// сертификата, которое расчёт подменять не вправе.
/// </remarks>
public enum RingReactanceOrigin
{
    /// <summary>Значения нет.</summary>
    None,

    /// <summary>Прочитано из памяти кольца.</summary>
    Ring,

    /// <summary>Получено расчётом по метрологии подключённого прибора.</summary>
    Calculated,

    /// <summary>Введено пользователем вручную.</summary>
    User
}

public partial class RingCalibrationRowViewModel : ObservableObject
{
    private bool _isProgrammaticReactanceUpdate;

    public RingCalibrationRowViewModel(int index, string label)
    {
        Index = index;
        Label = label;
    }

    public int Index { get; }

    public string Label { get; }

    [ObservableProperty]
    public partial string L1Text { get; set; } = string.Empty;

    [ObservableProperty]
    public partial string L2Text { get; set; } = string.Empty;

    [ObservableProperty]
    public partial string FrequencyText { get; set; } = string.Empty;

    [ObservableProperty]
    public partial string CapacitanceText { get; set; } = string.Empty;

    [ObservableProperty]
    public partial string ReactanceCalculatedText { get; set; } = string.Empty;

    [ObservableProperty]
    public partial string ResistanceText { get; set; } = string.Empty;

    [ObservableProperty]
    public partial string ReactanceMeasuredText { get; set; } = string.Empty;

    [ObservableProperty]
    public partial bool IsRingValueReadOnly { get; set; }

    /// <summary>
    /// Происхождение текущего значения «Реакт. изм.».
    /// </summary>
    public RingReactanceOrigin ReactanceMeasuredOrigin { get; private set; } = RingReactanceOrigin.None;

    /// <summary>
    /// Значение, введённое человеком и потому защищённое от пересчёта.
    /// </summary>
    public bool HasUserEnteredReactance => ReactanceMeasuredOrigin == RingReactanceOrigin.User;

    /// <summary>
    /// Задаёт «Реакт. изм.» от имени приложения с указанием источника.
    /// </summary>
    /// <remarks>
    /// Присваивание через свойство неотличимо от правки в поле ввода, поэтому
    /// программные пути обязаны идти сюда: иначе прочитанное из кольца значение
    /// тут же объявит себя пользовательским и снова станет неприкосновенным.
    /// </remarks>
    public void SetReactanceMeasured(string text, RingReactanceOrigin origin)
    {
        _isProgrammaticReactanceUpdate = true;
        try
        {
            ReactanceMeasuredText = text;
        }
        finally
        {
            _isProgrammaticReactanceUpdate = false;
        }

        ReactanceMeasuredOrigin = string.IsNullOrWhiteSpace(text)
            ? RingReactanceOrigin.None
            : origin;
    }

    partial void OnReactanceMeasuredTextChanged(string value)
    {
        if (_isProgrammaticReactanceUpdate)
        {
            return;
        }

        // Правка пришла из поля ввода: значение принадлежит пользователю.
        ReactanceMeasuredOrigin = string.IsNullOrWhiteSpace(value)
            ? RingReactanceOrigin.None
            : RingReactanceOrigin.User;
    }
}
