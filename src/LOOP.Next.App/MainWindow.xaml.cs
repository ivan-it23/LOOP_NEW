using Microsoft.UI.Xaml;

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

        RootFrame.Navigate(typeof(MainPage));
    }
}
