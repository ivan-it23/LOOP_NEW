using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml;
using LOOP_Next_App.ViewModels;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace LOOP_Next_App;

/// <summary>
/// The main content page displayed inside the application window.
/// </summary>
public sealed partial class MainPage : Page
{
    public MainPageViewModel ViewModel { get; } = new();

    public MainPage()
    {
        InitializeComponent();
        UpdateSelectedTabContent();
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
}
