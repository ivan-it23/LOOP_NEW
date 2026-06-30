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
        MaximizeOnLaunch();

        RootFrame.Navigate(typeof(MainPage));
    }

    private void MaximizeOnLaunch()
    {
        if (AppWindow.Presenter is Microsoft.UI.Windowing.OverlappedPresenter presenter)
        {
            presenter.Maximize();
        }
    }
}
