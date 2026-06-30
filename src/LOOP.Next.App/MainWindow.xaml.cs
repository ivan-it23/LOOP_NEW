using Microsoft.UI.Xaml;
using Windows.Graphics;

namespace LOOP_Next_App;

/// <summary>
/// Главное окно содержит пользовательский заголовок и Frame для основной страницы приложения.
/// </summary>
public sealed partial class MainWindow : Window
{
    public MainWindow()
    {
        InitializeComponent();

        ExtendsContentIntoTitleBar = true;
        SetTitleBar(AppTitleBar);

        AppWindow.SetIcon("Assets/AppIcon.ico");
        OpenAcrossCurrentScreenWidth();

        RootFrame.Navigate(typeof(MainPage));
    }

    private void OpenAcrossCurrentScreenWidth()
    {
        var windowHandle = WinRT.Interop.WindowNative.GetWindowHandle(this);
        var windowId = Microsoft.UI.Win32Interop.GetWindowIdFromWindow(windowHandle);
        var displayArea = Microsoft.UI.Windowing.DisplayArea.GetFromWindowId(
            windowId,
            Microsoft.UI.Windowing.DisplayAreaFallback.Nearest);
        var workArea = displayArea.WorkArea;
        var height = AppWindow.Size.Height > 0
            ? Math.Min(AppWindow.Size.Height, workArea.Height)
            : Math.Min(900, workArea.Height);

        AppWindow.MoveAndResize(new RectInt32(workArea.X, workArea.Y, workArea.Width, height));
    }
}
