using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Data;
using Microsoft.UI.Xaml.Input;
using Microsoft.UI.Xaml.Media;
using Microsoft.UI.Xaml.Navigation;

namespace LOOP_Next_App;

/// <summary>
/// Задаёт запуск WinUI-приложения и общие ссылки на главное окно.
/// </summary>
public partial class App : Application
{
    /// <summary>
    /// Хранит главное окно для диалогов, выбора файлов и WinRT-взаимодействия.
    /// </summary>
    public static Window Window { get; private set; } = null!;

    /// <summary>
    /// Маршалирует обновления из фоновых потоков в UI-поток.
    /// </summary>
    public static Microsoft.UI.Dispatching.DispatcherQueue DispatcherQueue { get; private set; } = null!;

    /// <summary>
    /// Возвращает HWND главного окна для API, требующих InitializeWithWindow.
    /// </summary>
    public static nint WindowHandle =>
        WinRT.Interop.WindowNative.GetWindowHandle(Window);

    public App()
    {
        InitializeComponent();
    }

    /// <summary>
    /// Создаёт и активирует главное окно при запуске приложения.
    /// </summary>
    protected override void OnLaunched(Microsoft.UI.Xaml.LaunchActivatedEventArgs args)
    {
        Window = new MainWindow();
        DispatcherQueue = Microsoft.UI.Dispatching.DispatcherQueue.GetForCurrentThread();
        Window.Activate();
    }
}
