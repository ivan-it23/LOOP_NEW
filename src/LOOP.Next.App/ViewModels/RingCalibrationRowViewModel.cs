using CommunityToolkit.Mvvm.ComponentModel;

namespace LOOP_Next_App.ViewModels;

public partial class RingCalibrationRowViewModel : ObservableObject
{
    public RingCalibrationRowViewModel(int index, string label)
    {
        Index = index;
        Label = label;
    }

    public int Index { get; }

    public string Label { get; }

    [ObservableProperty]
    public partial string L1Text { get; set; } = string.Empty;

    [ObservableProperty]
    public partial string L2Text { get; set; } = string.Empty;

    [ObservableProperty]
    public partial string FrequencyText { get; set; } = string.Empty;

    [ObservableProperty]
    public partial string CapacitanceText { get; set; } = string.Empty;

    [ObservableProperty]
    public partial string ReactanceCalculatedText { get; set; } = string.Empty;

    [ObservableProperty]
    public partial string ResistanceText { get; set; } = string.Empty;

    [ObservableProperty]
    public partial string ReactanceMeasuredText { get; set; } = string.Empty;
}
