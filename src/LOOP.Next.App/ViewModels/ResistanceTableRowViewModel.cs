using CommunityToolkit.Mvvm.ComponentModel;

namespace LOOP_Next_App.ViewModels;

public partial class ResistanceTableRowViewModel : ObservableObject
{
    public ResistanceTableRowViewModel(int index, string sondeLabel, string airText)
    {
        Index = index;
        SondeLabel = sondeLabel;
        AirText = airText;
    }

    public int Index { get; }

    public string SondeLabel { get; }

    [ObservableProperty]
    public partial string AirText { get; set; }

    [ObservableProperty]
    public partial string UesText { get; set; } = string.Empty;

    [ObservableProperty]
    public partial string R1Text { get; set; } = string.Empty;

    [ObservableProperty]
    public partial string R2Text { get; set; } = string.Empty;

    [ObservableProperty]
    public partial string R3Text { get; set; } = string.Empty;

    [ObservableProperty]
    public partial string R4Text { get; set; } = string.Empty;

    [ObservableProperty]
    public partial string R5Text { get; set; } = string.Empty;

    [ObservableProperty]
    public partial string R6Text { get; set; } = string.Empty;

    [ObservableProperty]
    public partial string R7Text { get; set; } = string.Empty;

    [ObservableProperty]
    public partial string R8Text { get; set; } = string.Empty;
}
