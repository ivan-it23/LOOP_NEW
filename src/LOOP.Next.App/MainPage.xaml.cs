using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml;
using LoopNext.Core.Logging;
using LOOP_Next_App.ViewModels;
using Microsoft.UI;
using Microsoft.UI.Xaml.Media;
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
                XamlRoot = xamlRoot,
                DefaultButton = ContentDialogButton.None
            };
            dialog.Content = BuildNotificationContent(notification, dialog);

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

    private static UIElement BuildNotificationContent(UserNotification notification, ContentDialog owner)
    {
        var dialog = new Grid
        {
            MinWidth = 420,
            MaxWidth = 560,
            RowSpacing = 18
        };
        dialog.RowDefinitions.Add(new RowDefinition { Height = GridLength.Auto });
        dialog.RowDefinitions.Add(new RowDefinition { Height = GridLength.Auto });
        dialog.RowDefinitions.Add(new RowDefinition { Height = GridLength.Auto });

        var header = new Grid
        {
            ColumnSpacing = 16,
            VerticalAlignment = VerticalAlignment.Center
        };
        header.ColumnDefinitions.Add(new ColumnDefinition { Width = GridLength.Auto });
        header.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(1, GridUnitType.Star) });

        var iconBackground = new Border
        {
            Width = 44,
            Height = 44,
            CornerRadius = new CornerRadius(22),
            Background = new SolidColorBrush(Colors.LightBlue),
            HorizontalAlignment = HorizontalAlignment.Center,
            VerticalAlignment = VerticalAlignment.Center
        };
        iconBackground.Child = new FontIcon
        {
            Glyph = "\uE946",
            FontSize = 22,
            Foreground = new SolidColorBrush(Colors.DarkBlue)
        };

        var title = new TextBlock
        {
            Text = notification.Title,
            FontSize = 22,
            FontWeight = Microsoft.UI.Text.FontWeights.SemiBold,
            TextWrapping = TextWrapping.Wrap,
            VerticalAlignment = VerticalAlignment.Center
        };

        header.Children.Add(iconBackground);
        header.Children.Add(title);
        Grid.SetColumn(title, 1);

        var message = new TextBlock
        {
            Text = notification.Message,
            FontSize = 15,
            LineHeight = 22,
            TextWrapping = TextWrapping.WrapWholeWords,
            Margin = new Thickness(60, 0, 0, 0)
        };

        var actions = new Grid
        {
            Padding = new Thickness(0, 8, 0, 0)
        };
        var okButton = new Button
        {
            Content = "OK",
            Width = 180,
            Height = 38,
            HorizontalAlignment = HorizontalAlignment.Center
        };
        if (Application.Current.Resources.TryGetValue("AccentButtonStyle", out var accentButtonStyle) &&
            accentButtonStyle is Style style)
        {
            okButton.Style = style;
        }

        okButton.Click += (_, _) => owner.Hide();
        actions.Children.Add(okButton);

        dialog.Children.Add(header);
        dialog.Children.Add(message);
        dialog.Children.Add(actions);
        Grid.SetRow(message, 1);
        Grid.SetRow(actions, 2);

        return dialog;
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

    private async void MetrologyFile_Click(object sender, RoutedEventArgs e)
    {
        if (!ViewModel.CanReadMetrologyFromFile)
        {
            return;
        }

        try
        {
            var picker = new FileOpenPicker
            {
                SuggestedStartLocation = PickerLocationId.DocumentsLibrary
            };
            WinRT.Interop.InitializeWithWindow.Initialize(picker, App.WindowHandle);
            picker.FileTypeFilter.Add("*");

            var file = await picker.PickSingleFileAsync();
            if (file is null)
            {
                ViewModel.NotifyMetrologyFileSelectionCanceled();
                return;
            }

            await ViewModel.ApplyMetrologyFromFileAsync(file.Path);
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Ошибка выбора файла метрологии");
            ViewModel.NotifyMetrologyFileDialogError(ex.Message);
        }
    }

    private async void EraseRingCells_Click(object sender, RoutedEventArgs e)
    {
        if (!ViewModel.CanEraseRingCells)
        {
            return;
        }

        var xamlRoot = XamlRoot ?? Content?.XamlRoot;
        if (xamlRoot is null)
        {
            return;
        }

        try
        {
            var dialog = new ContentDialog
            {
                XamlRoot = xamlRoot,
                Title = "Стереть все ячейки кольца?",
                Content =
                    "Команда ZERASE заполнит нулями все Z-ячейки кольца. " +
                    "Записанные сопротивления и реактивные сопротивления будут потеряны. " +
                    "Сигнатура хранится отдельно и этой командой не стирается.",
                PrimaryButtonText = "Стереть",
                CloseButtonText = "Отмена",
                DefaultButton = ContentDialogButton.Close
            };

            if (await dialog.ShowAsync() == ContentDialogResult.Primary)
            {
                await ViewModel.EraseRingCellsAsync();
            }
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Ошибка выполнения команды ZERASE");
            ViewModel.NotifyRingEraseDialogError(ex.Message);
        }
    }

    private async void Report_Click(object sender, RoutedEventArgs e)
    {
        if (!ViewModel.TryBuildReportRtf(out var rtf))
        {
            return;
        }

        try
        {
            var picker = new FileSavePicker
            {
                SuggestedFileName = ViewModel.CreateSuggestedReportFileName(),
                SuggestedStartLocation = PickerLocationId.DocumentsLibrary,
                DefaultFileExtension = ".rtf"
            };
            WinRT.Interop.InitializeWithWindow.Initialize(picker, App.WindowHandle);
            picker.FileTypeChoices.Add("RTF-рапорт", new List<string> { ".rtf" });

            var file = await picker.PickSaveFileAsync();
            if (file is null)
            {
                ViewModel.NotifyReportSaveCanceled();
                return;
            }

            await ViewModel.SaveReportRtfAsync(file.Path, rtf);
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Ошибка выбора файла рапорта");
            ViewModel.NotifyReportSaveDialogError(ex.Message);
        }
    }
}
