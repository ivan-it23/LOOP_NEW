using CommunityToolkit.Mvvm.ComponentModel;

namespace LOOP_Next_App.ViewModels;

// Отображение одного измерительного канала прибора: метка передатчика/частоты
// и значения фазы (сырое, локальный воздух, скорректированное) в милли-градусах.
public partial class ChannelReadingViewModel : ObservableObject
{
    public ChannelReadingViewModel(int index, string label)
    {
        Index = index;
        Label = label;
    }

    public int Index { get; }

    public string Label { get; }

    [ObservableProperty]
    public partial string RawText { get; set; } = "—";

    [ObservableProperty]
    public partial string AirText { get; set; } = "—";

    [ObservableProperty]
    public partial string CorrectedText { get; set; } = "—";
}
