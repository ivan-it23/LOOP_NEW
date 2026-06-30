using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml;
using LoopNext.Core.Logging;
using LOOP_Next_App.ViewModels;
using Windows.Storage.Pickers;

namespace LOOP_Next_App;

/// <summary>
/// Основная страница переключает рабочие области вкладок и привязывает MainPageViewModel.
/// </summary>
public sealed partial class MainPage : Page
{
    public MainPageViewModel ViewModel { get; } = new();

    // Гарантирует, что одновременно отображается не более одного модального уведомления.
    private bool _isShowingNotification;

    public MainPage()
    {
        InitializeComponent();
        UpdateSelectedTabContent();
        ViewModel.NotificationRequested += OnNotificationRequested;
    }

    private async void OnNotificationRequested(UserNotification notification)
    {
        if (_isShowingNotification)
        {
            return;
        }

        var xamlRoot = XamlRoot ?? Content?.XamlRoot;
        if (xamlRoot is null)
        {
            return;
        }

        _isShowingNotification = true;
        try
        {
            var dialog = new ContentDialog
            {
                Title = notification.Title,
                Content = notification.Message,
                CloseButtonText = "ОК",
                XamlRoot = xamlRoot
            };
            await dialog.ShowAsync();
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Ошибка показа диалога уведомления");
        }
        finally
        {
            _isShowingNotification = false;
        }
    }

    private void RootTabs_SelectionChanged(object sender, SelectionChangedEventArgs e)
    {
        UpdateSelectedTabContent();
    }

    private void UpdateSelectedTabContent()
    {
        if (MainConnectionPanel is null ||
            MainResistanceTable is null ||
            MainIdentificationPanel is null ||
            RingVerificationContent is null ||
            SettingsContent is null)
        {
            return;
        }

        var selectedIndex = RootTabs?.SelectedIndex ?? 0;

        SetMainContentVisibility(selectedIndex == 0);
        RingVerificationContent.Visibility = selectedIndex == 1 ? Visibility.Visible : Visibility.Collapsed;
        SettingsContent.Visibility = selectedIndex == 2 ? Visibility.Visible : Visibility.Collapsed;
    }

    private void SetMainContentVisibility(bool isVisible)
    {
        var visibility = isVisible ? Visibility.Visible : Visibility.Collapsed;
        MainConnectionPanel.Visibility = visibility;
        MainResistanceTable.Visibility = visibility;
        MainIdentificationPanel.Visibility = visibility;
    }

    private async void ReadMetrologyFromFile_Click(object sender, RoutedEventArgs e)
    {
        var picker = new FileOpenPicker();
        WinRT.Interop.InitializeWithWindow.Initialize(picker, App.WindowHandle);
        picker.FileTypeFilter.Add(".bin");

        var file = await picker.PickSingleFileAsync();
        if (file is null)
        {
            ViewModel.StatusText = "Чтение файла метрологии отменено";
            return;
        }

        await ViewModel.ReadMetrologyFromFileAsync(file.Path);
    }

    private async void WriteMetrologyToFile_Click(object sender, RoutedEventArgs e)
    {
        var picker = new FileSavePicker
        {
            SuggestedFileName = "Metrology"
        };
        WinRT.Interop.InitializeWithWindow.Initialize(picker, App.WindowHandle);
        picker.FileTypeChoices.Add("Файл метрологии", new List<string> { ".bin" });

        var file = await picker.PickSaveFileAsync();
        if (file is null)
        {
            ViewModel.StatusText = "Запись файла метрологии отменена";
            return;
        }

        await ViewModel.WriteMetrologyToFileAsync(file.Path);
    }
}
