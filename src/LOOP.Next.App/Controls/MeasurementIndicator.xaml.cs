using System;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;

namespace LOOP_Next_App.Controls;

// Measurement deviation indicator: color scale and one cursor for the averaged value.
// Positioning logic is inherited from do_graf (WorkForm.h).
public sealed partial class MeasurementIndicator : UserControl
{
    public static readonly DependencyProperty ValueProperty =
        DependencyProperty.Register(nameof(Value), typeof(double), typeof(MeasurementIndicator),
            new PropertyMetadata(0.0, OnValueChanged));

    public static readonly DependencyProperty AverageValueProperty =
        DependencyProperty.Register(nameof(AverageValue), typeof(double), typeof(MeasurementIndicator),
            new PropertyMetadata(0.0, OnAverageValueChanged));

    public static readonly DependencyProperty ThresholdProperty =
        DependencyProperty.Register(nameof(Threshold), typeof(double), typeof(MeasurementIndicator),
            new PropertyMetadata(6.0, OnThresholdChanged));

    public MeasurementIndicator()
    {
        InitializeComponent();
    }

    public double Value
    {
        get => (double)GetValue(ValueProperty);
        set => SetValue(ValueProperty, value);
    }

    public double AverageValue
    {
        get => (double)GetValue(AverageValueProperty);
        set => SetValue(AverageValueProperty, value);
    }

    public double Threshold
    {
        get => (double)GetValue(ThresholdProperty);
        set => SetValue(ThresholdProperty, value);
    }

    private static void OnValueChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
    {
        ((MeasurementIndicator)d).UpdateCursors();
    }

    private static void OnAverageValueChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
    {
        ((MeasurementIndicator)d).UpdateCursors();
    }

    private static void OnThresholdChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
    {
        ((MeasurementIndicator)d).UpdateCursors();
    }

    private void OnCanvasSizeChanged(object sender, SizeChangedEventArgs e)
    {
        UpdateZones();
        UpdateCursors();
    }

    private void UpdateZones()
    {
        var width = IndicatorCanvas.ActualWidth;
        var height = IndicatorCanvas.ActualHeight;
        var zoneWidth = width / 6.0;

        SetZone(ZoneRed1, 0 * zoneWidth, zoneWidth, height);
        SetZone(ZoneYellow1, 1 * zoneWidth, zoneWidth, height);
        SetZone(ZoneGreen1, 2 * zoneWidth, zoneWidth, height);
        SetZone(ZoneGreen2, 3 * zoneWidth, zoneWidth, height);
        SetZone(ZoneYellow2, 4 * zoneWidth, zoneWidth, height);
        SetZone(ZoneRed2, 5 * zoneWidth, zoneWidth, height);
    }

    private static void SetZone(FrameworkElement zone, double left, double width, double height)
    {
        zone.Width = Math.Max(width, 0.0);
        zone.Height = Math.Max(height, 0.0);
        Canvas.SetLeft(zone, left);
    }

    private void UpdateCursors()
    {
        var width = IndicatorCanvas.ActualWidth;
        if (width <= 0.0)
        {
            return;
        }

        var height = IndicatorCanvas.ActualHeight;
        var zoneWidth = width / 6.0;
        var center = 3 * zoneWidth;
        var safeThreshold = Math.Max(Threshold, 0.001);

        var averageOffset = zoneWidth * (AverageValue / safeThreshold);
        averageOffset = Math.Clamp(averageOffset, -3 * zoneWidth, 3 * zoneWidth - AverageCursor.Width);
        Canvas.SetLeft(AverageCursor, center + averageOffset);
        AverageCursor.Height = height;
    }
}
