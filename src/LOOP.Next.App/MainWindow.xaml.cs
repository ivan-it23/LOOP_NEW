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

    private int _windowScalePercent = WindowScaleService.DefaultScalePercent;
    private double _rasterizationScale = WindowScaleService.ReferenceRasterizationScale;
    private double _contentScaleFactor = 1.0;
    private double _toolTipScaleFactor = double.NaN;
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
    /// Загруженная страница приложения. Нужна для показа диалогов из App, где
    /// доступно только окно.
    /// </summary>
    public MainPage? Page => _mainPage;

    /// <summary>
    /// Выбранный пользователем относительный масштаб содержимого.
    /// </summary>
    public int WindowScalePercent => _windowScalePercent;

    /// <summary>
    /// Текущий системный коэффициент DPI окна.
    /// </summary>
    public double RasterizationScale => _rasterizationScale;

    /// <summary>
    /// Множитель текущего масштаба. Нужен диалогам: они размещаются в popup-корне
    /// XamlRoot, то есть вне масштабируемого кадра, и увеличиваются отдельно.
    /// Значение кешируется, потому что зависит от фактического размера рабочей
    /// области и пересчитывается в <see cref="ApplyContentScaleLayout" />.
    /// </summary>
    public double ContentScaleFactor => _contentScaleFactor;

    /// <summary>
    /// Применяет и сохраняет один из пяти относительных процентов масштаба.
    /// </summary>
    public void ApplyWindowScalePercent(int scalePercent)
    {
        _windowScalePercent = WindowScaleService.NormalizeScalePercent(scalePercent);

        SaveScalePercent(_windowScalePercent);
        ApplyContentScaleLayout();

        AppLogger.WriteDiagnostic(
            "APP_SESSION",
            "WINDOW_SCALE_APPLIED",
            $"percent={_windowScalePercent}|rasterization_scale={_rasterizationScale:F3}|host_effective={ContentScaleHost.ActualWidth:F0}x{ContentScaleHost.ActualHeight:F0}|auto_fit={WindowScaleService.GetAutoFitFactor(ContentScaleHost.ActualWidth, ContentScaleHost.ActualHeight):F3}|render_scale={_contentScaleFactor:F3}");
    }

    private void LoadSavedScaleSetting()
    {
        var settings = UserSettingsStore.Load();
        _windowScalePercent = WindowScaleService.ResolveSavedScalePercent(
            settings.WindowScaleSettingsVersion,
            settings.WindowScalePercent);

        SaveScalePercent(_windowScalePercent);
    }

    private static void SaveScalePercent(int scalePercent)
    {
        UserSettingsStore.Update(settings =>
        {
            settings.WindowScalePosition = null;
            settings.WindowScalePercent = WindowScaleService.NormalizeScalePercent(scalePercent);
            settings.WindowScaleSettingsVersion = WindowScaleService.CurrentSettingsSchemaVersion;
        });
    }

    private void OnContentScaleHostLoaded(object sender, RoutedEventArgs e)
    {
        SubscribeToXamlRootChanges();
        UpdateContentScale();
    }

    private void SubscribeToXamlRootChanges()
    {
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
    /// Пересчитывает масштаб при запуске и при смене монитора/DPI.
    /// </summary>
    private void UpdateContentScale()
    {
        _rasterizationScale = CalculateRasterizationScale();
        ApplyContentScaleLayout();
    }

    /// <summary>
    /// Системный масштаб больше не участвует в расчёте: размеры рабочей области
    /// приходят в эффективных пикселях и уже учитывают DPI. Значение читается
    /// только для журнала, чтобы по логу пользователя было видно, при каком
    /// системном масштабе Windows получен конкретный вид.
    /// </summary>
    private double CalculateRasterizationScale()
    {
        var rasterizationScale = ContentScaleHost.XamlRoot?.RasterizationScale ??
            WindowScaleService.ReferenceRasterizationScale;
        if (!double.IsFinite(rasterizationScale) || rasterizationScale <= 0)
        {
            rasterizationScale = WindowScaleService.ReferenceRasterizationScale;
        }

        var workArea = DisplayArea.GetFromWindowId(AppWindow.Id, DisplayAreaFallback.Nearest).WorkArea;

        AppLogger.WriteDiagnostic(
            "APP_SESSION",
            "WINDOW_SCALE_AUTOMATIC",
            $"rasterization_scale={rasterizationScale:F3}|reference_rasterization_scale={WindowScaleService.ReferenceRasterizationScale:F3}|reference_content={WindowScaleService.ReferenceContentWidth:F0}x{WindowScaleService.ReferenceContentHeight:F0}|work_area_physical={workArea.Width}x{workArea.Height}|host_effective={ContentScaleHost.ActualWidth:F0}x{ContentScaleHost.ActualHeight:F0}|selected_percent={_windowScalePercent}|auto_fit={WindowScaleService.GetAutoFitFactor(ContentScaleHost.ActualWidth, ContentScaleHost.ActualHeight):F3}");

        return rasterizationScale;
    }

    /// <summary>
    /// Вписывает эталонный кадр в текущую рабочую область и применяет
    /// преобразование. Кадру задаётся размер рабочей области, делённый на
    /// масштаб: содержимое верстается в приведённой области и затем
    /// масштабируется. Без пересчёта размера RenderTransform обрезал бы
    /// содержимое, потому что сам по себе разметку он не меняет.
    /// </summary>
    private void ApplyContentScaleLayout()
    {
        var hostWidth = ContentScaleHost.ActualWidth;
        var hostHeight = ContentScaleHost.ActualHeight;
        if (hostWidth <= 0 || hostHeight <= 0)
        {
            return;
        }

        var factor = WindowScaleService.GetEffectiveScaleFactor(_windowScalePercent, hostWidth, hostHeight);
        _contentScaleFactor = factor;

        // Подсказки живут в popup-корне XamlRoot и масштабируются отдельно.
        // Обход дерева страницы выполняется только при фактической смене
        // коэффициента: этот метод вызывается на каждое изменение размера окна.
        if (Math.Abs(factor - _toolTipScaleFactor) > 0.001)
        {
            _toolTipScaleFactor = factor;
            _mainPage?.ApplyContentScaleToToolTips();
        }

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
