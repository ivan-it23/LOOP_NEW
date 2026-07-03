using System.Runtime.InteropServices;
using LOOP_Next_App.ViewModels;
using Microsoft.UI.Windowing;
using Microsoft.UI.Xaml;
using Windows.Graphics;

namespace LOOP_Next_App;

/// <summary>
/// Главное окно содержит пользовательский заголовок, Frame основной страницы и ограничения размера окна.
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

        RootFrame.Navigated += OnRootFrameNavigated;
        RootFrame.Navigate(typeof(MainPage));
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
