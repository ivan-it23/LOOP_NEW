using CommunityToolkit.Mvvm.ComponentModel;
using LoopNext.Core.Models;
using Microsoft.UI;
using Microsoft.UI.Xaml.Media;

namespace LOOP_Next_App.ViewModels;

public partial class ResistanceTableRowViewModel : ObservableObject
{
    private static readonly SolidColorBrush DefaultForegroundBrush = new(Colors.Black);
    private static readonly SolidColorBrush InToleranceBrush = new(Colors.LimeGreen);
    private static readonly SolidColorBrush WarningBrush = new(Colors.Gold);
    private static readonly SolidColorBrush OutOfToleranceBrush = new(Colors.Red);
    private static readonly SolidColorBrush CriticalBrush = new(Colors.DarkRed);

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

    [ObservableProperty]
    public partial Brush R1Foreground { get; set; } = DefaultForegroundBrush;

    [ObservableProperty]
    public partial Brush R2Foreground { get; set; } = DefaultForegroundBrush;

    [ObservableProperty]
    public partial Brush R3Foreground { get; set; } = DefaultForegroundBrush;

    [ObservableProperty]
    public partial Brush R4Foreground { get; set; } = DefaultForegroundBrush;

    [ObservableProperty]
    public partial Brush R5Foreground { get; set; } = DefaultForegroundBrush;

    [ObservableProperty]
    public partial Brush R6Foreground { get; set; } = DefaultForegroundBrush;

    [ObservableProperty]
    public partial Brush R7Foreground { get; set; } = DefaultForegroundBrush;

    [ObservableProperty]
    public partial Brush R8Foreground { get; set; } = DefaultForegroundBrush;

    public MeasurementQuality R1Quality { get; private set; } = MeasurementQuality.InTolerance;
    public MeasurementQuality R2Quality { get; private set; } = MeasurementQuality.InTolerance;
    public MeasurementQuality R3Quality { get; private set; } = MeasurementQuality.InTolerance;
    public MeasurementQuality R4Quality { get; private set; } = MeasurementQuality.InTolerance;
    public MeasurementQuality R5Quality { get; private set; } = MeasurementQuality.InTolerance;
    public MeasurementQuality R6Quality { get; private set; } = MeasurementQuality.InTolerance;
    public MeasurementQuality R7Quality { get; private set; } = MeasurementQuality.InTolerance;
    public MeasurementQuality R8Quality { get; private set; } = MeasurementQuality.InTolerance;

    public void SetResistanceCell(int resistanceIndex, MeasurementCell cell)
    {
        var text = cell.HasValue ? cell.Value.ToString("F1") : string.Empty;
        var brush = cell.HasValue ? BrushForQuality(cell.Quality) : DefaultForegroundBrush;

        switch (resistanceIndex)
        {
            case 1:
                R1Text = text;
                R1Foreground = brush;
                R1Quality = cell.Quality;
                break;
            case 2:
                R2Text = text;
                R2Foreground = brush;
                R2Quality = cell.Quality;
                break;
            case 3:
                R3Text = text;
                R3Foreground = brush;
                R3Quality = cell.Quality;
                break;
            case 4:
                R4Text = text;
                R4Foreground = brush;
                R4Quality = cell.Quality;
                break;
            case 5:
                R5Text = text;
                R5Foreground = brush;
                R5Quality = cell.Quality;
                break;
            case 6:
                R6Text = text;
                R6Foreground = brush;
                R6Quality = cell.Quality;
                break;
            case 7:
                R7Text = text;
                R7Foreground = brush;
                R7Quality = cell.Quality;
                break;
            case 8:
                R8Text = text;
                R8Foreground = brush;
                R8Quality = cell.Quality;
                break;
        }
    }

    public string GetResistanceText(int resistanceIndex) =>
        resistanceIndex switch
        {
            1 => R1Text,
            2 => R2Text,
            3 => R3Text,
            4 => R4Text,
            5 => R5Text,
            6 => R6Text,
            7 => R7Text,
            8 => R8Text,
            _ => string.Empty
        };

    public Brush GetResistanceForeground(int resistanceIndex) =>
        resistanceIndex switch
        {
            1 => R1Foreground,
            2 => R2Foreground,
            3 => R3Foreground,
            4 => R4Foreground,
            5 => R5Foreground,
            6 => R6Foreground,
            7 => R7Foreground,
            8 => R8Foreground,
            _ => DefaultForegroundBrush
        };

    public MeasurementQuality GetResistanceQuality(int resistanceIndex) =>
        resistanceIndex switch
        {
            1 => R1Quality,
            2 => R2Quality,
            3 => R3Quality,
            4 => R4Quality,
            5 => R5Quality,
            6 => R6Quality,
            7 => R7Quality,
            8 => R8Quality,
            _ => MeasurementQuality.InTolerance
        };

    private static SolidColorBrush BrushForQuality(MeasurementQuality quality) =>
        quality switch
        {
            MeasurementQuality.InTolerance => InToleranceBrush,
            MeasurementQuality.Warning => WarningBrush,
            MeasurementQuality.OutOfTolerance => OutOfToleranceBrush,
            MeasurementQuality.Critical => CriticalBrush,
            _ => DefaultForegroundBrush
        };
}
