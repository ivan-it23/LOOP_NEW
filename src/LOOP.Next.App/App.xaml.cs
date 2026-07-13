using LoopNext.Core.Logging;
using LOOP_Next_App.Services;
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

        AppLogger.WriteDiagnostic(
            "APP_SESSION",
            "START",
            $"pid={Environment.ProcessId}|version={ApplicationInfo.CurrentVersion}|base_directory={AppContext.BaseDirectory}|log_path={AppLogger.LogFilePath}");

        // Перехват необработанных исключений UI-потока: фиксирует причину сбоя в журнал
        // и не даёт процессу молча завершиться, оставляя окно приложения открытым.
        UnhandledException += OnUnhandledException;
        TaskScheduler.UnobservedTaskException += OnUnobservedTaskException;
    }

    private static void OnUnhandledException(object sender, Microsoft.UI.Xaml.UnhandledExceptionEventArgs e)
    {
        AppLogger.Write(e.Exception, "НЕОБРАБОТАННОЕ ИСКЛЮЧЕНИЕ UI-потока");
        e.Handled = true;
    }

    private static void OnUnobservedTaskException(object? sender, UnobservedTaskExceptionEventArgs e)
    {
        AppLogger.Write(e.Exception, "НЕОБРАБОТАННОЕ ИСКЛЮЧЕНИЕ фоновой задачи");
        e.SetObserved();
    }

    /// <summary>
    /// Создаёт и активирует главное окно при запуске приложения.
    /// </summary>
    protected override void OnLaunched(Microsoft.UI.Xaml.LaunchActivatedEventArgs args)
    {
        AppLogger.WriteDiagnostic("APP_SESSION", "WINDOW_CREATE");
        Window = new MainWindow();
        DispatcherQueue = Microsoft.UI.Dispatching.DispatcherQueue.GetForCurrentThread();
        Window.Activate();
        AppLogger.WriteDiagnostic(
            "APP_SESSION",
            "WINDOW_ACTIVATED",
            $"hwnd={WindowHandle}|title=Кольца");
        _ = AutoUpdateService.RunUpdateCheckAsync(WindowHandle, silent: true);
    }
}
