using System.Runtime.InteropServices;
using LoopNext.Core.Logging;
using LOOP_Next_App.Services;
using LOOP_Next_App.ViewModels;
using Microsoft.UI.Windowing;
using Microsoft.UI.Xaml;
using Windows.Graphics;

namespace LOOP_Next_App;

/// <summary>
/// Главное окно содержит пользовательский заголовок, Frame основной страницы,
/// ограничения размера окна и масштаб содержимого.
/// </summary>
public sealed partial class MainWindow : Window
{
    private const double MinimumWindowWidthPercent = 0.80;
    private const double MinimumWindowFiveTransmittersHeightPercent = 0.91;
    private const double MinimumWindowCompactHeightPercent = 0.82;
    private const int DefaultTransmitterCount = 5;
    private const uint WmGetMinMaxInfo = 0x0024;

    private readonly SubclassProc _windowSubclassProc;
    private MainPage? _mainPage;
    private SizeInt32 _minimumWindowSize;
    private bool _isApplyingMinimumSize;

    private int _windowScalePosition = WindowScaleService.DefaultScalePosition;
    private int? _legacyWindowScalePercent;
    private int _automaticScalePercent = WindowScaleService.MinimumAutomaticScalePercent;
    private bool _isXamlRootSubscribed;

    public MainWindow()
    {
        InitializeComponent();

        _windowSubclassProc = WindowSubclassProc;

        ExtendsContentIntoTitleBar = true;
        SetTitleBar(AppTitleBar);

        AppWindow.SetIcon("Assets/AppIcon.ico");
        AppWindow.Changed += OnAppWindowChanged;
        InstallWindowSubclass();
        ApplyMinimumWindowSize(DefaultTransmitterCount);
        MaximizeOnLaunch();

        LoadSavedScaleSetting();
        ContentScaleHost.Loaded += OnContentScaleHostLoaded;

        RootFrame.Navigated += OnRootFrameNavigated;
        RootFrame.Navigate(typeof(MainPage));
    }

    /// <summary>
    /// Сохранённая позиция пользователя: от -2 до +2, где 0 — рекомендуемый
    /// масштаб для текущего монитора.
    /// </summary>
    public int WindowScalePosition => _windowScalePosition;

    /// <summary>
    /// Масштаб, подобранный для текущего монитора.
    /// </summary>
    public int AutomaticScalePercent => _automaticScalePercent;

    /// <summary>
    /// Фактически применённый масштаб содержимого.
    /// </summary>
    public int EffectiveScalePercent =>
        WindowScaleService.GetScalePercent(_automaticScalePercent, _windowScalePosition);

    /// <summary>
    /// Множитель текущего масштаба. Нужен диалогам: они размещаются в popup-корне
    /// XamlRoot, то есть вне масштабируемого кадра, и увеличиваются отдельно.
    /// </summary>
    public double ContentScaleFactor =>
        WindowScaleService.GetScaleFactor(EffectiveScalePercent);

    /// <summary>
    /// Применяет и сохраняет одну из пяти позиций масштаба.
    /// </summary>
    public void ApplyWindowScalePosition(int scalePosition)
    {
        _windowScalePosition = WindowScaleService.NormalizeScalePosition(scalePosition);
        _legacyWindowScalePercent = null;

        SaveScalePosition(_windowScalePosition);
        UpdateContentScale();

        AppLogger.WriteDiagnostic(
            "APP_SESSION",
            "WINDOW_SCALE_APPLIED",
            $"position={_windowScalePosition}|automatic={_automaticScalePercent}|effective={EffectiveScalePercent}");
    }

    private void LoadSavedScaleSetting()
    {
        var settings = UserSettingsStore.Load();
        if (settings.WindowScalePosition is int savedPosition)
        {
            _windowScalePosition = WindowScaleService.NormalizeScalePosition(savedPosition);
            return;
        }

        _legacyWindowScalePercent = settings.WindowScalePercent;
    }

    private static void SaveScalePosition(int scalePosition)
    {
        var settings = UserSettingsStore.Load();
        settings.WindowScalePosition = WindowScaleService.NormalizeScalePosition(scalePosition);
        settings.WindowScalePercent = null;
        UserSettingsStore.Save(settings);
    }

    private void OnContentScaleHostLoaded(object sender, RoutedEventArgs e)
    {
        SubscribeToXamlRootChanges();
        UpdateContentScale();
    }

    private void SubscribeToXamlRootChanges()
    {
        // Смена монитора меняет RasterizationScale, поэтому автоматический масштаб
        // нужно пересчитывать при перемещении окна между экранами.
        if (_isXamlRootSubscribed || ContentScaleHost.XamlRoot is null)
        {
            return;
        }

        ContentScaleHost.XamlRoot.Changed += OnXamlRootChanged;
        _isXamlRootSubscribed = true;
    }

    private void OnXamlRootChanged(Microsoft.UI.Xaml.XamlRoot sender, Microsoft.UI.Xaml.XamlRootChangedEventArgs args) =>
        UpdateContentScale();

    private void OnContentScaleHostSizeChanged(object sender, SizeChangedEventArgs e) =>
        ApplyContentScaleLayout();

    /// <summary>
    /// Пересчитывает автоматический масштаб по текущему монитору и применяет результат.
    /// </summary>
    private void UpdateContentScale()
    {
        _automaticScalePercent = CalculateAutomaticScalePercent();

        // Первая реализация сохраняла абсолютный процент. После вычисления
        // эталона текущего монитора переносим его на ближайшую из пяти позиций.
        if (_legacyWindowScalePercent is int legacyScalePercent)
        {
            _windowScalePosition = WindowScaleService.GetClosestScalePosition(
                _automaticScalePercent,
                legacyScalePercent);
            _legacyWindowScalePercent = null;
            SaveScalePosition(_windowScalePosition);
        }

        ApplyContentScaleLayout();
    }

    private int CalculateAutomaticScalePercent()
    {
        var rasterizationScale = ContentScaleHost.XamlRoot?.RasterizationScale ?? 1.0;
        if (!double.IsFinite(rasterizationScale) || rasterizationScale <= 0)
        {
            rasterizationScale = 1.0;
        }

        // WorkArea выражена в физических пикселях; для верстки нужны эффективные.
        var workArea = DisplayArea.GetFromWindowId(AppWindow.Id, DisplayAreaFallback.Nearest).WorkArea;
        var effectiveWidth = workArea.Width / rasterizationScale;
        var effectiveHeight = workArea.Height / rasterizationScale;
        var automaticScalePercent = WindowScaleService.GetAutomaticScalePercent(effectiveWidth, effectiveHeight);

        // Диагностика нужна при разборе жалоб на размер элементов на чужих мониторах.
        AppLogger.WriteDiagnostic(
            "APP_SESSION",
            "WINDOW_SCALE_AUTOMATIC",
            $"rasterization_scale={rasterizationScale:F3}|work_area_physical={workArea.Width}x{workArea.Height}|work_area_effective={effectiveWidth:F1}x{effectiveHeight:F1}|reference={WindowScaleService.ReferenceContentWidth:F0}x{WindowScaleService.ReferenceContentHeight:F0}|automatic_percent={automaticScalePercent}");

        return automaticScalePercent;
    }

    /// <summary>
    /// Задаёт кадру размер рабочей области, делённый на масштаб, и применяет
    /// преобразование. Без пересчёта размера RenderTransform обрезал бы содержимое.
    /// </summary>
    private void ApplyContentScaleLayout()
    {
        var hostWidth = ContentScaleHost.ActualWidth;
        var hostHeight = ContentScaleHost.ActualHeight;
        if (hostWidth <= 0 || hostHeight <= 0)
        {
            return;
        }

        var factor = WindowScaleService.GetScaleFactor(EffectiveScalePercent);

        ContentScaleTransform.ScaleX = factor;
        ContentScaleTransform.ScaleY = factor;
        RootFrame.Width = hostWidth / factor;
        RootFrame.Height = hostHeight / factor;
    }

    private void MaximizeOnLaunch()
    {
        if (AppWindow.Presenter is OverlappedPresenter presenter)
        {
            presenter.Maximize();
        }
    }

    private void OnRootFrameNavigated(object sender, Microsoft.UI.Xaml.Navigation.NavigationEventArgs e)
    {
        if (_mainPage is not null)
        {
            _mainPage.ViewModel.PropertyChanged -= OnMainPageViewModelPropertyChanged;
        }

        _mainPage = RootFrame.Content as MainPage;
        if (_mainPage is null)
        {
            return;
        }

        _mainPage.ViewModel.PropertyChanged += OnMainPageViewModelPropertyChanged;
        ApplyMinimumWindowSize(_mainPage.ViewModel.TransmitterCount);
    }

    private void OnMainPageViewModelPropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
    {
        if (e.PropertyName == nameof(MainPageViewModel.TransmitterCount) && sender is MainPageViewModel viewModel)
        {
            ApplyMinimumWindowSize(viewModel.TransmitterCount);
        }
    }

    private void OnAppWindowChanged(AppWindow sender, AppWindowChangedEventArgs args)
    {
        if (args.DidSizeChange)
        {
            EnsureWindowIsNotBelowMinimum();
        }
    }

    private void InstallWindowSubclass()
    {
        var hWnd = WinRT.Interop.WindowNative.GetWindowHandle(this);
        SetWindowSubclass(hWnd, _windowSubclassProc, UIntPtr.Zero, UIntPtr.Zero);
    }

    private void ApplyMinimumWindowSize(int transmitterCount)
    {
        var displayArea = DisplayArea.GetFromWindowId(AppWindow.Id, DisplayAreaFallback.Nearest);
        var workArea = displayArea.WorkArea;
        var heightPercent = transmitterCount is 3 or 4
            ? MinimumWindowCompactHeightPercent
            : MinimumWindowFiveTransmittersHeightPercent;

        _minimumWindowSize = new SizeInt32(
            (int)Math.Ceiling(workArea.Width * MinimumWindowWidthPercent),
            (int)Math.Ceiling(workArea.Height * heightPercent));

        EnsureWindowIsNotBelowMinimum();
    }

    private void EnsureWindowIsNotBelowMinimum()
    {
        if (_isApplyingMinimumSize ||
            _minimumWindowSize.Width <= 0 ||
            _minimumWindowSize.Height <= 0 ||
            AppWindow.Presenter is OverlappedPresenter { State: OverlappedPresenterState.Maximized })
        {
            return;
        }

        var currentSize = AppWindow.Size;
        var width = Math.Max(currentSize.Width, _minimumWindowSize.Width);
        var height = Math.Max(currentSize.Height, _minimumWindowSize.Height);

        if (width == currentSize.Width && height == currentSize.Height)
        {
            return;
        }

        _isApplyingMinimumSize = true;
        try
        {
            AppWindow.Resize(new SizeInt32(width, height));
        }
        finally
        {
            _isApplyingMinimumSize = false;
        }
    }

    private IntPtr WindowSubclassProc(
        IntPtr hWnd,
        uint message,
        UIntPtr wParam,
        IntPtr lParam,
        UIntPtr subclassId,
        UIntPtr refData)
    {
        if (message == WmGetMinMaxInfo)
        {
            var minMaxInfo = Marshal.PtrToStructure<MinMaxInfo>(lParam);
            minMaxInfo.MinTrackSize.X = _minimumWindowSize.Width;
            minMaxInfo.MinTrackSize.Y = _minimumWindowSize.Height;
            Marshal.StructureToPtr(minMaxInfo, lParam, fDeleteOld: false);
            return IntPtr.Zero;
        }

        return DefSubclassProc(hWnd, message, wParam, lParam);
    }

    private delegate IntPtr SubclassProc(
        IntPtr hWnd,
        uint message,
        UIntPtr wParam,
        IntPtr lParam,
        UIntPtr subclassId,
        UIntPtr refData);

    [DllImport("comctl32.dll", SetLastError = true)]
    private static extern bool SetWindowSubclass(
        IntPtr hWnd,
        SubclassProc subclassProc,
        UIntPtr subclassId,
        UIntPtr refData);

    [DllImport("comctl32.dll", SetLastError = true)]
    private static extern IntPtr DefSubclassProc(
        IntPtr hWnd,
        uint message,
        UIntPtr wParam,
        IntPtr lParam);

    [StructLayout(LayoutKind.Sequential)]
    private struct Point
    {
        public int X;
        public int Y;
    }

    [StructLayout(LayoutKind.Sequential)]
    private struct MinMaxInfo
    {
        public Point Reserved;
        public Point MaxSize;
        public Point MaxPosition;
        public Point MinTrackSize;
        public Point MaxTrackSize;
    }
}
