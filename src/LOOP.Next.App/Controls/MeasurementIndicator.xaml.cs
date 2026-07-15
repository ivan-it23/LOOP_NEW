using System;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Automation;
using Microsoft.UI.Xaml.Controls;

namespace LOOP_Next_App.Controls;

// Measurement deviation indicator: color scale with current and averaged-value markers.
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
        SetZone(ZoneWarning1, 1 * zoneWidth, zoneWidth, height);
        SetZone(ZoneGreen1, 2 * zoneWidth, zoneWidth, height);
        SetZone(ZoneGreen2, 3 * zoneWidth, zoneWidth, height);
        SetZone(ZoneWarning2, 4 * zoneWidth, zoneWidth, height);
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

        ZeroLine.Height = height;
        Canvas.SetLeft(ZeroLine, center - (ZeroLine.Width / 2.0));

        PositionMarker(CurrentMarker, Value, safeThreshold, zoneWidth, center, width);
        Canvas.SetTop(CurrentMarker, Math.Max((height - CurrentMarker.Height) / 2.0, 0.0));

        AverageMarker.Height = height;
        PositionMarker(AverageMarker, AverageValue, safeThreshold, zoneWidth, center, width);
        Canvas.SetTop(AverageMarker, 0.0);

        var overflowTop = Math.Max((height - 10.0) / 2.0, 0.0);
        Canvas.SetLeft(OverflowLeft, 2.0);
        Canvas.SetTop(OverflowLeft, overflowTop);
        Canvas.SetLeft(OverflowRight, Math.Max(width - 10.0, 0.0));
        Canvas.SetTop(OverflowRight, overflowTop);
        OverflowLeft.Visibility = AverageValue < -3.0 * safeThreshold ? Visibility.Visible : Visibility.Collapsed;
        OverflowRight.Visibility = AverageValue > 3.0 * safeThreshold ? Visibility.Visible : Visibility.Collapsed;

        UpdateAccessibleDescription(safeThreshold);
    }

    private static void PositionMarker(
        FrameworkElement marker,
        double value,
        double threshold,
        double zoneWidth,
        double center,
        double totalWidth)
    {
        var finiteValue = double.IsFinite(value) ? value : 0.0;
        var offset = zoneWidth * (finiteValue / threshold);
        var markerCenter = center + Math.Clamp(offset, -3.0 * zoneWidth, 3.0 * zoneWidth);
        var markerLeft = Math.Clamp(markerCenter - (marker.Width / 2.0), 0.0, Math.Max(totalWidth - marker.Width, 0.0));
        Canvas.SetLeft(marker, markerLeft);
    }

    private void UpdateAccessibleDescription(double threshold)
    {
        var currentText = FormatPercent(Value);
        var averageText = FormatPercent(AverageValue);
        var status = GetStatus(AverageValue, threshold);
        var description = $"Среднее: {averageText}\nТекущее: {currentText}\nСостояние: {status}";

        IndicatorToolTip.Content = description;
        AutomationProperties.SetName(this, $"Шкала разности фаз. Среднее {averageText}. Текущее {currentText}. {status}.");
        AutomationProperties.SetHelpText(this, $"Допуск плюс-минус {threshold:0.#} процента.");
    }

    private static string FormatPercent(double value)
    {
        var finiteValue = double.IsFinite(value) ? value : 0.0;
        return $"{finiteValue:+0.0;-0.0;0.0}%";
    }

    private static string GetStatus(double value, double threshold)
    {
        var absoluteValue = Math.Abs(double.IsFinite(value) ? value : 0.0);
        if (absoluteValue <= threshold)
        {
            return "в допуске";
        }

        if (absoluteValue <= 2.0 * threshold)
        {
            return "зона предупреждения";
        }

        if (absoluteValue <= 3.0 * threshold)
        {
            return "вне допуска";
        }

        return "критическое отклонение за пределами шкалы";
    }
}
