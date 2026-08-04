using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml;
using LoopNext.Core.Logging;
using LOOP_Next_App.Services;
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

    // Защищает диалог масштаба от повторного открытия быстрым двойным нажатием.
    private bool _isShowingWindowScaleDialog;

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
            ApplyContentScaleToDialog(dialog);

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

    /// <summary>
    /// Применяет к диалогу текущий масштаб содержимого. Диалоги WinUI живут в
    /// popup-корне XamlRoot, поэтому масштаб главного кадра на них не действует,
    /// и без этого при увеличенном интерфейсе они выглядели бы мелкими.
    /// </summary>
    private static void ApplyContentScaleToDialog(ContentDialog dialog)
    {
        if (App.Window is not MainWindow window)
        {
            return;
        }

        var factor = window.ContentScaleFactor;
        if (Math.Abs(factor - 1.0) < 0.001)
        {
            return;
        }

        dialog.RenderTransformOrigin = new Windows.Foundation.Point(0.5, 0.5);
        dialog.RenderTransform = new ScaleTransform { ScaleX = factor, ScaleY = factor };
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
            SettingsContent is null ||
            DocumentationContent is null)
        {
            return;
        }

        var selectedIndex = RootTabs?.SelectedIndex ?? 0;

        SetMainContentVisibility(selectedIndex == 0);
        RingVerificationContent.Visibility = selectedIndex == 1 ? Visibility.Visible : Visibility.Collapsed;
        SettingsContent.Visibility = selectedIndex == 2 ? Visibility.Visible : Visibility.Collapsed;
        DocumentationContent.Visibility = selectedIndex == 3 ? Visibility.Visible : Visibility.Collapsed;
    }

    private void SetMainContentVisibility(bool isVisible)
    {
        var visibility = isVisible ? Visibility.Visible : Visibility.Collapsed;
        MainConnectionPanel.Visibility = visibility;
        MainResistanceTable.Visibility = visibility;
        MainIdentificationPanel.Visibility = visibility;
    }

    /// <summary>
    /// Открывает выбор масштаба содержимого. Значение применяется сразу, чтобы
    /// результат был виден за диалогом; отмена возвращает исходную настройку.
    /// </summary>
    private async void WindowScale_Click(object sender, RoutedEventArgs e)
    {
        if (_isShowingWindowScaleDialog || App.Window is not MainWindow window)
        {
            return;
        }

        var xamlRoot = XamlRoot ?? Content?.XamlRoot;
        if (xamlRoot is null)
        {
            return;
        }

        var originalPosition = window.WindowScalePosition;
        _isShowingWindowScaleDialog = true;
        try
        {
            var dialog = new ContentDialog
            {
                XamlRoot = xamlRoot,
                Title = "Масштаб окна",
                PrimaryButtonText = "Готово",
                SecondaryButtonText = "Отмена",
                DefaultButton = ContentDialogButton.Primary
            };
            dialog.Content = BuildWindowScaleContent(window);
            ApplyContentScaleToDialog(dialog);

            if (await dialog.ShowAsync() != ContentDialogResult.Primary)
            {
                window.ApplyWindowScalePosition(originalPosition);
            }
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Ошибка выбора масштаба окна");
            window.ApplyWindowScalePosition(originalPosition);
        }
        finally
        {
            _isShowingWindowScaleDialog = false;
        }
    }

    private static UIElement BuildWindowScaleContent(MainWindow window)
    {
        var automaticScalePercent = window.AutomaticScalePercent;
        var currentPosition = window.WindowScalePosition;

        var panel = new StackPanel { Width = 380, Spacing = 14 };
        panel.Children.Add(new TextBlock
        {
            Text =
                "Средний уровень — рекомендуемый масштаб для текущего монитора. " +
                "Выберите один из двух меньших или двух больших уровней. " +
                "Изменение применяется сразу и сохраняется между запусками.",
            FontSize = 13,
            LineHeight = 19,
            TextWrapping = TextWrapping.WrapWholeWords,
            Foreground = Application.Current.Resources["TextFillColorSecondaryBrush"] as Brush
        });

        var options = new RadioButtons { MaxColumns = 1 };
        foreach (var position in WindowScaleService.AvailableScalePositions)
        {
            options.Items.Add(WindowScaleService.GetScaleOptionText(position, automaticScalePercent));
        }

        options.SelectedIndex = currentPosition - WindowScaleService.MinimumScalePosition;

        options.SelectionChanged += (_, _) =>
        {
            var index = options.SelectedIndex;
            if (index < 0)
            {
                return;
            }

            var position = index + WindowScaleService.MinimumScalePosition;
            window.ApplyWindowScalePosition(position);
        };

        panel.Children.Add(options);
        return panel;
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
            ApplyContentScaleToDialog(dialog);

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
        if (!ViewModel.TryBuildReportExcel(out var workbookBytes))
        {
            return;
        }

        try
        {
            var picker = new FileSavePicker
            {
                SuggestedFileName = ViewModel.CreateSuggestedReportFileName(),
                SuggestedStartLocation = PickerLocationId.DocumentsLibrary,
                DefaultFileExtension = ".xlsx"
            };
            WinRT.Interop.InitializeWithWindow.Initialize(picker, App.WindowHandle);
            picker.FileTypeChoices.Add("Excel-рапорт", new List<string> { ".xlsx" });

            var file = await picker.PickSaveFileAsync();
            if (file is null)
            {
                ViewModel.NotifyReportSaveCanceled();
                return;
            }

            await ViewModel.SaveReportExcelAsync(file.Path, workbookBytes);
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Ошибка выбора файла рапорта");
            ViewModel.NotifyReportSaveDialogError(ex.Message);
        }
    }
}
