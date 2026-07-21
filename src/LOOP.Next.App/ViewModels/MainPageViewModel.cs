using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Globalization;
using System.IO.Ports;
using System.Linq;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using LoopNext.Core.Logging;
using LoopNext.Core.Models;
using LoopNext.Core.Protocol;
using LoopNext.Hardware;
using LoopNext.Metrology;
using LOOP.Next.Reporting;
using LOOP_Next_App.Services;
using Microsoft.UI;
using Microsoft.UI.Text;
using Microsoft.UI.Xaml.Media;

namespace LOOP_Next_App.ViewModels;

// ViewModel координирует UI главной страницы, аппаратный слой и расчётный конвейер измерений.
public partial class MainPageViewModel : ObservableObject
{
    private static readonly SolidColorBrush AirColumnDefaultBrush = new(Colors.White);
    private static readonly SolidColorBrush LocalAirSetBrush = new(Colors.Red);
    private static readonly SolidColorBrush AirColumnDefaultForegroundBrush = new(Colors.Black);
    private static readonly SolidColorBrush AirColumnSetForegroundBrush = new(Colors.White);
    private static readonly string[] DefaultRingCalibrationLabels =
    {
        "T1 401",
        "T2 401",
        "T3 401",
        "T4 401",
        "T5 401",
        "T1 2001",
        "T2 2001",
        "T3 2001",
        "T4 2001",
        "T5 2001"
    };

    private const int MeasurementSkipFrames = 5;
    private const double CompactIdentificationPanelWidth = 470;
    private const double ExpandedIdentificationPanelWidth = 540;
    private const int ExpandedIdentificationTypeNameLength = 14;

    private readonly SerialPort _toolPort = new();
    private readonly SerialPort _ringPort = new();
    private RingDeviceInfo _ringDeviceInfo = RingDeviceInfo.Unknown;
    private readonly LocalAirCorrector _localAir = new();
    private readonly object _airLock = new();
    private readonly object _measurementLock = new();
    private readonly SemaphoreSlim _toolIoLock = new(1, 1);
    private readonly UserSettings _userSettings;
    private bool _isRestoringPorts;

    private CancellationTokenSource? _pollCts;
    private IReadOnlyList<SondeParameter> _sondeParameters = Array.Empty<SondeParameter>();
    private IReadOnlyList<LoopParameter> _loopParameters = Array.Empty<LoopParameter>();
    private IReadOnlyList<V10> _phaseReferences = Array.Empty<V10>();
    private IReadOnlyList<V10> _resistivityReferences = Array.Empty<V10>();
    private readonly Queue<float>[,] _resistancePercentBuffers = new Queue<float>[MeasurementTable.ResistanceCount, V10.ChannelCount];
    private CancellationTokenSource? _autoCts;
    private CancellationTokenSource? _singleResistanceCts;
    private int _activeResistanceIndex;
    private int _activeCapacitanceIndex;
    private int _activeMeasurementFrameCount;
    private string _activeMeasurementPointId = "none";
    private int _channelCount;
    private readonly object _autoLock = new();
    private TaskCompletionSource<bool>? _autoMeasurementCompleteTcs;
    private uint _rxPosition;
    private GpToolMetrology _metrology;
    private int _frameCounter;
    private string _ringWorkflowId = "none";
    private string _activeMeasurementRunId = "none";
    private long _activeMeasurementRunStartedTimestamp;
    private int _diagnosticCompletedPoints;
    private int _diagnosticFailedPoints;
    private readonly Dictionary<int, RingFirmwarePointMonitor> _activeFirmwarePointMonitors = new();
    private long _activeMeasurementPointStartedTimestamp;
    private bool _activeFirmwarePointSummaryLogged = true;

    public MainPageViewModel()
    {
        _userSettings = UserSettingsStore.Load();
        BuildDefaultResistanceTable();
        BuildDefaultRingCalibrationRows();
        RefreshPorts();
        AppLogger.WriteDiagnostic(
            "RING_WORKFLOW",
            "VIEW_MODEL_READY",
            $"workflow={_ringWorkflowId}|tool_port={SelectedToolPort ?? "none"}|ring_port={SelectedRingPort ?? "none"}|poll_interval_ms={PollIntervalMs:F0}|air_samples={SelectedFrameCount}|averaging={SelectedAveraging}|threshold_percent={ThresholdPercent}|express={ExpressMode}");
    }

    // Запрос на показ модального сообщения пользователю; обрабатывается code-behind страницы.
    public event Action<UserNotification>? NotificationRequested;

    public ObservableCollection<string> ToolPorts { get; } = new();

    public ObservableCollection<string> RingPorts { get; } = new();

    public ObservableCollection<ChannelReadingViewModel> Channels { get; } = new();

    public ObservableCollection<ResistanceTableRowViewModel> ResistanceRows { get; } = new();

    public ObservableCollection<RingCalibrationRowViewModel> RingCalibrationRows { get; } = new();

    public ObservableCollection<RingCalibrationRowViewModel> RingResistanceRows { get; } = new();

    public ObservableCollection<RingCalibrationRowViewModel> RingGeometryRows { get; } = new();

    public ObservableCollection<RingCalibrationRowViewModel> RingFrequencyRows { get; } = new();

    public ObservableCollection<int> FrameCountOptions { get; } = new() { 20, 30, 40, 50, 60 };

    public ObservableCollection<int> AveragingOptions { get; } = new() { 5, 10, 15, 20 };

    public ObservableCollection<int> ThresholdOptions { get; } = new() { 5, 6, 7, 8, 9, 10 };

    public bool CanStartSingleResistance =>
        IsConnected &&
        IsRingConnected &&
        !IsAutoMode &&
        !IsResistanceMeasurementRunning;

    public bool CanToggleAutoMode => !IsResistanceMeasurementRunning || IsAutoMode;

    public bool CanReadMetrologyFromFile => !IsConnected;

    public string MetrologyFileButtonText => IsConnected ? "(Считано из прибора)" : "Из файла";

    [ObservableProperty]
    [NotifyPropertyChangedFor(nameof(CanConnect))]
    [NotifyPropertyChangedFor(nameof(CanToggleTool))]
    public partial string? SelectedToolPort { get; set; }

    partial void OnSelectedToolPortChanged(string? value)
    {
        if (_isRestoringPorts)
        {
            return;
        }

        SaveSelectedToolPort(value);
    }

    [ObservableProperty]
    [NotifyPropertyChangedFor(nameof(CanConnectRing))]
    [NotifyPropertyChangedFor(nameof(CanToggleRing))]
    public partial string? SelectedRingPort { get; set; }

    partial void OnSelectedRingPortChanged(string? value)
    {
        if (_isRestoringPorts)
        {
            return;
        }

        SaveSelectedRingPort(value);
    }

    [ObservableProperty]
    public partial int SelectedFrameCount { get; set; } = LocalAirCorrector.DefaultSampleCount;

    // Усреднение и порог применяются при расчёте отклонений в таблице R1..R8.
    [ObservableProperty]
    public partial int SelectedAveraging { get; set; } = 5;

    [ObservableProperty]
    [NotifyPropertyChangedFor(nameof(ScaleMinus3PercentLabel))]
    [NotifyPropertyChangedFor(nameof(ScaleMinus2PercentLabel))]
    [NotifyPropertyChangedFor(nameof(ScaleMinus1PercentLabel))]
    [NotifyPropertyChangedFor(nameof(ScalePlus1PercentLabel))]
    [NotifyPropertyChangedFor(nameof(ScalePlus2PercentLabel))]
    [NotifyPropertyChangedFor(nameof(ScalePlus3PercentLabel))]
    public partial int ThresholdPercent { get; set; } = 6;

    public string ScaleMinus3PercentLabel => FormatScalePercent(-3);

    public string ScaleMinus2PercentLabel => FormatScalePercent(-2);

    public string ScaleMinus1PercentLabel => FormatScalePercent(-1);

    public string ScalePlus1PercentLabel => FormatScalePercent(1);

    public string ScalePlus2PercentLabel => FormatScalePercent(2);

    public string ScalePlus3PercentLabel => FormatScalePercent(3);

    partial void OnThresholdPercentChanged(int value)
    {
        foreach (var row in ResistanceRows)
        {
            row.IndicatorThreshold = value;
        }
    }

    private string FormatScalePercent(int multiplier)
    {
        var value = multiplier * ThresholdPercent;
        return value > 0
            ? FormattableString.Invariant($"+{value}%")
            : FormattableString.Invariant($"{value}%");
    }

    // Режимы измерений управляют автоциклом кольца и настройкой экспресс-проверки.
    [ObservableProperty]
    [NotifyPropertyChangedFor(nameof(CanStartSingleResistance))]
    [NotifyPropertyChangedFor(nameof(CanToggleAutoMode))]
    [NotifyCanExecuteChangedFor(nameof(RunResistanceCommand))]
    public partial bool IsAutoMode { get; set; }

    partial void OnIsAutoModeChanged(bool value)
    {
        if (value)
        {
            if (IsResistanceMeasurementRunning)
            {
                StatusText = "Режим АВТО: дождитесь завершения текущего R";
                IsAutoMode = false;
                return;
            }

            _ = RunAutoModeAsync();
        }
        else
        {
            _autoCts?.Cancel();
        }
    }

    [ObservableProperty]
    public partial bool ExpressMode { get; set; } = true;

    [ObservableProperty]
    public partial double PollIntervalMs { get; set; } = 1000;

    [ObservableProperty]
    [NotifyPropertyChangedFor(nameof(CanStartSingleResistance))]
    [NotifyPropertyChangedFor(nameof(CanToggleAutoMode))]
    [NotifyCanExecuteChangedFor(nameof(RunResistanceCommand))]
    public partial bool IsResistanceMeasurementRunning { get; set; }

    [ObservableProperty]
    public partial bool ReportR1Selected { get; set; }

    [ObservableProperty]
    public partial bool ReportR2Selected { get; set; }

    [ObservableProperty]
    public partial bool ReportR3Selected { get; set; }

    [ObservableProperty]
    public partial bool ReportR4Selected { get; set; }

    [ObservableProperty]
    public partial bool ReportR5Selected { get; set; }

    [ObservableProperty]
    public partial bool ReportR6Selected { get; set; }

    [ObservableProperty]
    public partial bool ReportR7Selected { get; set; }

    [ObservableProperty]
    public partial bool ReportR8Selected { get; set; }

    partial void OnPollIntervalMsChanged(double value)
    {
        if (!double.IsFinite(value) || value < 700)
        {
            PollIntervalMs = 700;
        }
    }

    [ObservableProperty]
    public partial bool IsRingEditMode { get; set; }

    partial void OnIsRingEditModeChanged(bool value) => ApplyRingEditModes();

    [ObservableProperty]
    [NotifyPropertyChangedFor(nameof(CanConnect))]
    [NotifyPropertyChangedFor(nameof(CanToggleTool))]
    [NotifyPropertyChangedFor(nameof(CanReadMetrologyFromFile))]
    [NotifyPropertyChangedFor(nameof(MetrologyFileButtonText))]
    [NotifyPropertyChangedFor(nameof(ToolToggleText))]
    [NotifyPropertyChangedFor(nameof(CanStartSingleResistance))]
    [NotifyCanExecuteChangedFor(nameof(RunResistanceCommand))]
    public partial bool IsConnected { get; set; }

    [ObservableProperty]
    [NotifyPropertyChangedFor(nameof(CanConnectRing))]
    [NotifyPropertyChangedFor(nameof(CanToggleRing))]
    [NotifyPropertyChangedFor(nameof(RingToggleText))]
    [NotifyPropertyChangedFor(nameof(CanStartSingleResistance))]
    [NotifyCanExecuteChangedFor(nameof(RunResistanceCommand))]
    public partial bool IsRingConnected { get; set; }

    [ObservableProperty]
    [NotifyPropertyChangedFor(nameof(PollingGlyph))]
    [NotifyPropertyChangedFor(nameof(PollingText))]
    public partial bool IsPolling { get; set; }

    [ObservableProperty]
    public partial string ConnectionStateText { get; set; } = "Выключено";

    partial void OnIsConnectedChanged(bool value)
    {
        ConnectionStateText = value ? "Включено" : "Выключено";
    }

    public bool CanConnect => !IsConnected && !string.IsNullOrWhiteSpace(SelectedToolPort);

    public bool CanConnectRing => !IsRingConnected && !string.IsNullOrWhiteSpace(SelectedRingPort);

    public bool CanToggleTool => IsConnected || !string.IsNullOrWhiteSpace(SelectedToolPort);

    public bool CanToggleRing => IsRingConnected || !string.IsNullOrWhiteSpace(SelectedRingPort);

    public string ToolToggleText => IsConnected ? "Выключить прибор" : "Включить прибор";

    public string RingToggleText => IsRingConnected ? "Выключить кольцо" : "Включить кольцо";

    public string PollingGlyph => IsPolling ? "\uE769" : "\uE768";

    public string PollingText => IsPolling ? "Пауза" : "Старт";

    [ObservableProperty]
    public partial string StatusText { get; set; } = "Готово к подключению";

    [ObservableProperty]
    public partial int FrameCount { get; set; }

    [ObservableProperty]
    public partial string ToolTypeName { get; set; } = "—";

    [ObservableProperty]
    public partial double IdentificationPanelWidth { get; set; } = CompactIdentificationPanelWidth;

    [ObservableProperty]
    public partial string SignatureText { get; set; } = "—";

    [ObservableProperty]
    public partial string SerialText { get; set; } = "—";

    [ObservableProperty]
    public partial int TransmitterCount { get; set; } = 5;

    [ObservableProperty]
    public partial string TransmitterCountText { get; set; } = "—";

    [ObservableProperty]
    public partial string RxPositionText { get; set; } = "—";

    [ObservableProperty]
    public partial string TemperatureText { get; set; } = "—";

    [ObservableProperty]
    public partial string ModificationText { get; set; } = "—";

    [ObservableProperty]
    public partial string NumberText { get; set; } = "—";

    [ObservableProperty]
    public partial string PaletteText { get; set; } = "—";

    [ObservableProperty]
    public partial string SignatureBitsText { get; set; } = "—";

    [ObservableProperty]
    public partial string LocalAirState { get; set; } = "Не задан";

    [ObservableProperty]
    public partial string LocalAirButtonText { get; set; } = "Локальный воздух";

    [ObservableProperty]
    public partial Windows.UI.Text.FontWeight LocalAirButtonFontWeight { get; set; } = FontWeights.Normal;

    [ObservableProperty]
    public partial Brush AirColumnBackground { get; set; } = AirColumnDefaultBrush;

    [ObservableProperty]
    public partial Brush AirColumnForeground { get; set; } = AirColumnDefaultForegroundBrush;

    [RelayCommand]
    private async Task ToggleToolAsync()
    {
        if (IsConnected)
        {
            Disconnect();
        }
        else
        {
            await ConnectAsync();
        }
    }

    [RelayCommand]
    private async Task ToggleRingAsync()
    {
        if (IsRingConnected)
        {
            await DisconnectRingAsync();
        }
        else
        {
            await ConnectRingAsync();
        }
    }

    [RelayCommand]
    private void RefreshPorts()
    {
        var ports = SerialPortEnumerator.GetAvailablePortNames();
        var toolPort = SelectAvailablePort(ports, SelectedToolPort, _userSettings.ToolPort);
        var ringPort = SelectAvailablePort(ports, SelectedRingPort, _userSettings.RingPort);

        ToolPorts.Clear();
        RingPorts.Clear();
        foreach (var port in ports)
        {
            ToolPorts.Add(port);
            RingPorts.Add(port);
        }

        _isRestoringPorts = true;
        try
        {
            SelectedToolPort = toolPort;
            SelectedRingPort = ringPort;
        }
        finally
        {
            _isRestoringPorts = false;
        }
    }

    private static string? SelectAvailablePort(IReadOnlyList<string> ports, string? currentPort, string? savedPort)
    {
        if (!string.IsNullOrWhiteSpace(currentPort) && ports.Contains(currentPort))
        {
            return currentPort;
        }

        if (!string.IsNullOrWhiteSpace(savedPort) && ports.Contains(savedPort))
        {
            return savedPort;
        }

        return ports.Count > 0 ? ports[0] : null;
    }

    private void SaveSelectedToolPort(string? portName)
    {
        if (string.IsNullOrWhiteSpace(portName) || string.Equals(_userSettings.ToolPort, portName, StringComparison.Ordinal))
        {
            return;
        }

        _userSettings.ToolPort = portName;
        UserSettingsStore.Save(_userSettings);
    }

    private void SaveSelectedRingPort(string? portName)
    {
        if (string.IsNullOrWhiteSpace(portName) || string.Equals(_userSettings.RingPort, portName, StringComparison.Ordinal))
        {
            return;
        }

        _userSettings.RingPort = portName;
        UserSettingsStore.Save(_userSettings);
    }

    [RelayCommand]
    private async Task ConnectAsync()
    {
        if (IsConnected)
        {
            return;
        }

        if (string.IsNullOrWhiteSpace(SelectedToolPort))
        {
            StatusText = "Выберите COM-порт прибора";
            return;
        }

        StatusText = "Подключение...";
        var portName = SelectedToolPort;
        SaveSelectedToolPort(portName);

        var result = await Task.Run(() =>
        {
            _toolPort.PortName = portName;
            return ToolConnection.Connect(_toolPort);
        });

        if (!result.Success)
        {
            AppLogger.WriteDiagnostic(
                "RING_WORKFLOW",
                "TOOL_CONNECT_FAILED",
                $"workflow={CurrentRingWorkflowId}|port={portName}|error={SanitizeDiagnosticValue(result.ErrorMessage ?? "unknown")}");
            StatusText = result.ErrorMessage ?? "Не удалось открыть порт прибора";
            IsConnected = false;
            return;
        }

        _sondeParameters = result.SondeParameters;
        _channelCount = _sondeParameters.Count;
        _rxPosition = result.Metrology.RxPosition;
        _metrology = result.Metrology;
        AppLogger.WriteDiagnostic(
            "RING_WORKFLOW",
            "TOOL_CONNECTED",
            $"workflow={CurrentRingWorkflowId}|port={portName}|signature=0x{_metrology.Signature:X8}|serial={_metrology.Serial}|work_type={_metrology.WorkType}|tx={_channelCount / MetrologyConverter.FrequencyCount}|channels={_channelCount}|rx_position={_rxPosition}");
        for (var channel = 0; channel < _sondeParameters.Count; channel++)
        {
            var parameter = _sondeParameters[channel];
            AppLogger.WriteDiagnostic(
                "RING_CALIBRATION",
                "METROLOGY_CHANNEL",
                $"workflow={CurrentRingWorkflowId}|source=tool|channel={channel}|frequency_Hz={parameter.Frequency:G9}|L1_m={parameter.L1:G9}|L2_m={parameter.L2:G9}");
        }

        ApplyIdentification(in _metrology);
        BuildChannels(_sondeParameters, in _metrology);
        BuildRingCalibrationRows(_sondeParameters);
        CalculateRingCalibrationCore(showUserMessage: false, populateMissingMeasuredValues: false);
        if (IsRingConnected)
        {
            await LoadRingParametersAsync();
        }

        IsConnected = true;
        _frameCounter = 0;
        FrameCount = 0;

        StatusText = result.Metrology.Signature != 0
            ? "Прибор подключён, метрология считана"
            : "Порт открыт, метрология не считана";

        StartPolling();
    }

    [RelayCommand]
    private void Disconnect()
    {
        AppLogger.WriteDiagnostic(
            "RING_WORKFLOW",
            "TOOL_DISCONNECT",
            $"workflow={CurrentRingWorkflowId}|port={_toolPort.PortName}|polling={IsPolling}|measurement_run={_activeMeasurementRunId}");
        StopPolling();
        ToolConnection.Disconnect(_toolPort);
        IsConnected = false;
        StatusText = "Прибор отключён";
    }

    [RelayCommand]
    private async Task ConnectRingAsync()
    {
        if (IsRingConnected)
        {
            return;
        }

        if (string.IsNullOrWhiteSpace(SelectedRingPort))
        {
            StatusText = "Выберите COM-порт кольца";
            return;
        }

        var portName = SelectedRingPort;
        _ringWorkflowId = AppLogger.NextOperationId("ring-workflow");
        SaveSelectedRingPort(portName);
        StatusText = "Подключение кольца...";
        AppLogger.WriteDiagnostic(
            "RING_WORKFLOW",
            "RING_CONNECT_REQUESTED",
            $"workflow={_ringWorkflowId}|ring_port={portName}|tool_connected={IsConnected}|tool_port={SelectedToolPort ?? "none"}|tool_signature=0x{_metrology.Signature:X8}|tx={_channelCount / MetrologyConverter.FrequencyCount}|channels={_channelCount}");

        var result = await Task.Run(() =>
        {
            _ringPort.PortName = portName;
            return RingConnection.Connect(_ringPort, _ringWorkflowId);
        });

        if (result.IsConnected)
        {
            _ringDeviceInfo = result.DeviceInfo;
            IsRingConnected = true;
            AppLogger.WriteDiagnostic(
                "RING_WORKFLOW",
                "RING_CONNECTED",
                $"workflow={_ringWorkflowId}|ring_port={portName}|profile={result.DeviceInfo.ProtocolVersion}|signature={result.DeviceInfo.Signature?.ToString(CultureInfo.InvariantCulture) ?? "none"}|memory_cells={result.DeviceInfo.ImpedanceCellCount}|working_cells={RingProtocolConstants.WorkingParameterCellCount}|tx={_channelCount / MetrologyConverter.FrequencyCount}|channels={_channelCount}");
            AppLogger.WriteDiagnostic(
                "RING_FIRMWARE_DIAG",
                "PROTOCOL_CAPABILITIES",
                $"workflow={_ringWorkflowId}|ring_port={portName}|profile={result.DeviceInfo.ProtocolVersion}|signature={result.DeviceInfo.Signature?.ToString(CultureInfo.InvariantCulture) ?? "none"}|selection_semantics=documented_single_RES_and_single_CAP|state_readback_supported=false|RESREAD_supported=false|CAPREAD_supported=false|keepalive_requirement=undocumented|physical_state_evidence=corrected_tool_phase");
            StatusText = result.DeviceInfo.ProtocolVersion == RingProtocolVersion.Extended
                ? $"Кольцо подключено, SGN={result.DeviceInfo.Signature}"
                : "Кольцо подключено (совместимый режим)";
            if (IsConnected)
            {
                await LoadRingParametersAsync();
            }
        }
        else
        {
            _ringDeviceInfo = RingDeviceInfo.Unknown;
            IsRingConnected = false;
            AppLogger.WriteDiagnostic(
                "RING_WORKFLOW",
                "RING_CONNECT_FAILED",
                $"workflow={_ringWorkflowId}|ring_port={portName}|status={result.Status}");
            StatusText = result.Status switch
            {
                RingConnection.RingConnectStatus.PortOpenFailed => "Не удается открыть порт кольца!",
                RingConnection.RingConnectStatus.NoResponse => "Нет ответа от кольца !",
                RingConnection.RingConnectStatus.Rejected => "Не удается установить соединение !",
                _ => $"Кольцо: {result.Status}"
            };
        }
    }

    [RelayCommand]
    private async Task DisconnectRingAsync()
    {
        var workflow = CurrentRingWorkflowId;
        AppLogger.WriteDiagnostic(
            "RING_WORKFLOW",
            "RING_DISCONNECT_REQUESTED",
            $"workflow={workflow}|ring_port={_ringPort.PortName}|profile={_ringDeviceInfo.ProtocolVersion}|measurement_run={_activeMeasurementRunId}");
        _autoCts?.Cancel();
        _singleResistanceCts?.Cancel();
        SetActiveMeasurementPoint(0, 0);
        await Task.Run(() => RingConnection.Disconnect(_ringPort, workflow));
        _ringDeviceInfo = RingDeviceInfo.Unknown;
        IsRingConnected = false;
        StatusText = "Кольцо отключено";
        AppLogger.WriteDiagnostic(
            "RING_WORKFLOW",
            "RING_DISCONNECTED",
            $"workflow={workflow}|ring_port={_ringPort.PortName}");
        _ringWorkflowId = "none";
    }

    public string CreateSuggestedReportFileName() =>
        $"LOOP_report_{DateTime.Now:yyyyMMdd_HHmmss}";

    public bool TryBuildReportRtf(out string rtf)
    {
        var reportId = AppLogger.NextOperationId("ring-report");
        var correlation = BuildDiagnosticCorrelation(reportId);
        rtf = string.Empty;
        AppLogger.WriteDiagnostic(
            "RING_REPORT",
            "BUILD_REQUESTED",
            $"correlation={correlation}|workflow={CurrentRingWorkflowId}|measurement_run={_activeMeasurementRunId}|rows={ResistanceRows.Count}|selected=[{string.Join(",", GetSelectedReportResistances())}]");

        if (ResistanceRows.Count == 0)
        {
            NotifyReportBlocked("В таблице нет строк с данными. Подключите прибор, считайте метрологию и заполните таблицу R1..R8 перед созданием рапорта.");
            return false;
        }

        var selectedResistances = GetSelectedReportResistances();
        if (selectedResistances.Count == 0)
        {
            var filledResistances = GetFilledReportResistances();
            var message = filledResistances.Count == 0
                ? "Рапорт не может быть сформирован: в таблице нет заполненных столбцов и ни один чекбокс R1..R8 не выбран. Сначала заполните хотя бы один столбец R1..R8, затем отметьте его чекбокс."
                : $"Рапорт не может быть сформирован: не выбран ни один чекбокс R1..R8. Отметьте нужные столбцы для рапорта. Сейчас в таблице есть данные для: {FormatResistanceList(filledResistances)}.";
            NotifyReportBlocked(message);
            return false;
        }

        var emptySelected = selectedResistances
            .Where(resistanceIndex => !HasAnyReportData(resistanceIndex))
            .ToArray();
        if (emptySelected.Length > 0)
        {
            NotifyReportBlocked(
                $"Рапорт не может быть сформирован: выбранные столбцы {FormatResistanceList(emptySelected)} не содержат данных. Заполните эти столбцы или снимите с них чекбоксы.");
            return false;
        }

        var report = new LoopReport(
            SerialText,
            DateTime.Now,
            _loopParameters.Take(MeasurementTable.ResistanceCount).Select(parameter => parameter.Resistance).ToArray(),
            selectedResistances,
            BuildReportChannels(selectedResistances));

        rtf = LoopReportRtfBuilder.Build(report);
        AppLogger.WriteDiagnostic(
            "RING_REPORT",
            "BUILT",
            $"correlation={correlation}|serial={SerialText}|selected=[{string.Join(",", selectedResistances)}]|channels={report.Channels.Count}|rtf_characters={rtf.Length}");
        foreach (var resistanceIndex in selectedResistances)
        {
            AppLogger.WriteDiagnostic(
                "RING_REPORT",
                "COLUMN_INCLUDED",
                $"correlation={correlation}|R={resistanceIndex}|cells=[{FormatResistanceColumnDiagnostic(resistanceIndex)}]");
        }
        StatusText = "Рапорт: выберите место сохранения";
        return true;
    }

    public async Task SaveReportRtfAsync(string path, string rtf)
    {
        if (string.IsNullOrWhiteSpace(path))
        {
            NotifyReportSaveCanceled();
            return;
        }

        try
        {
            await Task.Run(() => File.WriteAllText(path, rtf));
            AppLogger.WriteDiagnostic(
                "RING_REPORT",
                "SAVED",
                $"workflow={CurrentRingWorkflowId}|path={SanitizeDiagnosticValue(path)}|characters={rtf.Length}");
            StatusText = $"Рапорт сохранён: {path}";
            RaiseUserMessage("Рапорт сохранён", $"Файл рапорта успешно сохранён:\n{path}");
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Ошибка сохранения рапорта");
            AppLogger.WriteDiagnostic(
                "RING_REPORT",
                "SAVE_FAILED",
                ex,
                $"workflow={CurrentRingWorkflowId}|path={SanitizeDiagnosticValue(path)}|characters={rtf.Length}");
            StatusText = $"Рапорт не сохранён: {ex.Message}";
            RaiseUserMessage("Рапорт не сохранён", $"Не удалось записать файл рапорта:\n{path}\n\nПричина: {ex.Message}");
        }
    }

    public void NotifyReportSaveCanceled()
    {
        StatusText = "Рапорт: сохранение отменено";
    }

    public void NotifyReportSaveDialogError(string errorMessage)
    {
        StatusText = "Рапорт: ошибка выбора файла";
        RaiseUserMessage("Рапорт не сохранён", $"Не удалось открыть окно сохранения рапорта.\n\nПричина: {errorMessage}");
    }

    public void NotifyMetrologyFileSelectionCanceled()
    {
        StatusText = "Выбор файла метрологии отменён";
    }

    public void NotifyMetrologyFileDialogError(string errorMessage)
    {
        StatusText = "Метрология из файла не считана";
        RaiseUserMessage("Метрология не считана", $"Не удалось открыть окно выбора файла метрологии.\n\nПричина: {errorMessage}");
    }

    private bool CanRunResistance(object? parameter) =>
        CanStartSingleResistance && TryGetResistanceIndex(parameter, out _);

    [RelayCommand(CanExecute = nameof(CanRunResistance))]
    private async Task RunResistanceAsync(object? parameter)
    {
        if (!TryGetResistanceIndex(parameter, out var resistanceIndex))
        {
            StatusText = "R: неверный номер";
            return;
        }

        if (!await PrepareResistanceMeasurementAsync($"R{resistanceIndex}"))
        {
            return;
        }

        _singleResistanceCts?.Cancel();
        _singleResistanceCts = new CancellationTokenSource();
        var token = _singleResistanceCts.Token;
        IsResistanceMeasurementRunning = true;
        var diagnosticRun = BeginMeasurementDiagnosticRun("manual", resistanceIndex);
        var diagnosticStatus = "failed";

        try
        {
            AppLogger.Write($"=== РУЧНОЙ ЗАПУСК R{resistanceIndex} ===");
            var measured = await RunResistanceMeasurementAsync(resistanceIndex, $"R{resistanceIndex}", token);
            diagnosticStatus = measured ? "completed" : "failed";
            AppLogger.Write($"=== РУЧНОЙ ПРОХОД R{resistanceIndex} ЗАВЕРШЁН ===");
        }
        catch (OperationCanceledException)
        {
            diagnosticStatus = "cancelled";
            AppLogger.Write($"РУЧНОЙ ПРОХОД R{resistanceIndex}: отменён.");
            StatusText = $"R{resistanceIndex}: остановлено";
        }
        finally
        {
            lock (_autoLock)
            {
                _autoMeasurementCompleteTcs = null;
            }

            SetActiveMeasurementPoint(0, 0);
            if (_ringPort.IsOpen)
            {
                AppLogger.Write($"РУЧНОЙ ПРОХОД R{resistanceIndex}: сброс кольца в 0/0.");
                await Task.Run(
                    () => RingConnection.SetMeasurementPoint(_ringPort, 0, 0, diagnosticRun),
                    CancellationToken.None);
            }

            IsResistanceMeasurementRunning = false;
            EndMeasurementDiagnosticRun(diagnosticRun, diagnosticStatus);
            _singleResistanceCts.Dispose();
            _singleResistanceCts = null;
        }
    }

    [RelayCommand]
    private async Task ReadMetrologyFromToolAsync()
    {
        StatusText = "Чтение метрологии из зонда...";
        var result = await ExecuteToolPortOperationAsync(MetrologyPort.ReadFromTool);
        if (!result.Success)
        {
            StatusText = result.ErrorMessage ?? "ФАЙЛ МЕТРОЛОГИИ НЕ БЫЛ СЧИТАН!";
            return;
        }

        if (result.Value.Code != ToolResponseCode.Ok)
        {
            StatusText = result.Value.Code switch
            {
                ToolResponseCode.Timeout => "ФАЙЛ МЕТРОЛОГИИ НЕ БЫЛ СЧИТАН! (таймаут)",
                ToolResponseCode.CrcError => "ФАЙЛ МЕТРОЛОГИИ НЕ БЫЛ СЧИТАН! (ошибка CRC)",
                _ => $"ФАЙЛ МЕТРОЛОГИИ НЕ БЫЛ СЧИТАН! ({result.Value.Code})"
            };
            return;
        }

        ApplyMetrology(result.Value.Metrology);
        StatusText = "ФАЙЛ МЕТРОЛОГИИ СЧИТАН ИЗ ПРИБОРА!";
    }

    public async Task ApplyMetrologyFromFileAsync(string path)
    {
        if (IsConnected)
        {
            StatusText = "Метрология уже считана из прибора";
            return;
        }

        if (string.IsNullOrWhiteSpace(path))
        {
            StatusText = "Выбор файла метрологии отменён";
            return;
        }

        try
        {
            StatusText = "Чтение метрологии из файла...";
            var metrology = await Task.Run(() => MetrologyFile.Read(path));
            var parameters = MetrologyConverter.Convert(in metrology);
            if (parameters.Count == 0)
            {
                StatusText = "Файл метрологии не содержит поддерживаемую сигнатуру";
                RaiseUserMessage(
                    "Метрология не считана",
                    "Файл метрологии прочитан, но сигнатура зонда не соответствует поддерживаемым типам Автономный или LWD.");
                return;
            }

            ApplyMetrology(in metrology);
            AppLogger.Write($"METROLOGY_FILE: applied '{path}', Signature=0x{metrology.Signature:X8}, Serial={metrology.Serial}");
            StatusText = "Метрология считана из файла";
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Ошибка чтения метрологии из файла");
            StatusText = "Метрология из файла не считана";
            RaiseUserMessage(
                "Метрология не считана",
                $"Не удалось прочитать файл метрологии:\n{path}\n\nПричина: {ex.Message}");
        }
    }

    [RelayCommand]
    private void CalculateRingCalibration()
    {
        CalculateRingCalibrationCore(showUserMessage: true, populateMissingMeasuredValues: true);
    }

    private bool CalculateRingCalibrationCore(bool showUserMessage, bool populateMissingMeasuredValues)
    {
        var calculationId = AppLogger.NextOperationId("ring-capacitance-calc");
        var correlation = BuildDiagnosticCorrelation(calculationId);
        var stopwatch = Stopwatch.StartNew();
        var parameters = GetRingCalculationParameters();
        AppLogger.WriteDiagnostic(
            "RING_CALC",
            "CAPACITANCE_BEGIN",
            $"correlation={correlation}|sonde_parameters={parameters.Count}|rows={RingCalibrationRows.Count}|loop_inductance_H={RingCalibrationCalculator.DefaultLoopInductance:G17}|loop_radius_m={RingCalibrationCalculator.DefaultLoopRadius:G17}");
        if (parameters.Count == 0)
        {
            AppLogger.WriteDiagnostic(
                "RING_CALC",
                "CAPACITANCE_SKIPPED",
                $"correlation={correlation}|reason=no_sonde_metrology|elapsed_ms={stopwatch.ElapsedMilliseconds}");
            StatusText = "Нет метрологии зонда для расчёта кольца";
            if (showUserMessage)
            {
                RaiseUserMessage(
                    "Расчёт не выполнен",
                    "Нет метрологии зонда. Сначала подключите прибор или считайте метрологию из файла.");
            }

            return false;
        }

        var count = Math.Min(RingCalibrationRows.Count, parameters.Count);
        ClearInactiveRingCalculationRows(count);
        var populatedMeasuredValues = 0;
        var preservedMeasuredValues = 0;
        for (var i = 0; i < count; i++)
        {
            var parameter = parameters[i];
            var value = RingCalibrationCalculator.Calculate(parameter);
            RingCalibrationRows[i].L1Text = parameter.L1.ToString("F3");
            RingCalibrationRows[i].L2Text = parameter.L2.ToString("F3");
            RingCalibrationRows[i].FrequencyText = (parameter.Frequency / 1000f).ToString("F0");
            RingCalibrationRows[i].CapacitanceText = (value.CapacitanceFarads * 1_000_000_000_000f).ToString("F0");
            RingCalibrationRows[i].ReactanceCalculatedText = value.Reactance.ToString("F3");
            var hasMeasuredReactance =
                TryParseDouble(RingCalibrationRows[i].ReactanceMeasuredText, out var measuredReactance) &&
                measuredReactance > 0.000001;
            if (populateMissingMeasuredValues && !hasMeasuredReactance)
            {
                RingCalibrationRows[i].ReactanceMeasuredText = value.Reactance.ToString("F4");
                populatedMeasuredValues++;
            }
            else if (hasMeasuredReactance)
            {
                preservedMeasuredValues++;
            }

            AppLogger.WriteDiagnostic(
                "RING_CALC",
                "CAPACITANCE_CHANNEL",
                $"correlation={correlation}|channel={i}|frequency_Hz={parameter.Frequency:G9}|L1_m={parameter.L1:G9}|L2_m={parameter.L2:G9}|capacitance_F={value.CapacitanceFarads:G9}|capacitance_pF={value.CapacitanceFarads * 1_000_000_000_000f:F3}|reactance={value.Reactance:G9}|measured_reactance_source={(hasMeasuredReactance ? "preserved" : populateMissingMeasuredValues ? "calculated_fallback" : "unchanged")}");
        }

        if (populateMissingMeasuredValues)
        {
            RebuildLoopParametersFromRingRows();
        }

        RecalculateLoopReferences();
        AppLogger.WriteDiagnostic(
            "RING_CALC",
            "CAPACITANCE_COMPLETED",
            $"correlation={correlation}|calculated_channels={count}|populated_measured_values={populatedMeasuredValues}|preserved_measured_values={preservedMeasuredValues}|elapsed_ms={stopwatch.ElapsedMilliseconds}");
        StatusText = "Расчёт ёмкостей кольца выполнен";
        if (showUserMessage)
        {
            RaiseUserMessage(
                "Расчёт выполнен",
                $"Рассчитаны ёмкости и реактивные сопротивления для {count} каналов.\n\n" +
                $"Поле Re meas дополнено расчётными значениями: {populatedMeasuredValues}. " +
                $"Сохранено ранее измеренных значений: {preservedMeasuredValues}.");
        }

        return true;
    }

    [RelayCommand]
    private async Task ReadRingParametersAsync()
    {
        if (!IsRingConnected || !_ringPort.IsOpen)
        {
            StatusText = "Подключите кольцо";
            RaiseUserMessage(
                "Чтение не выполнено",
                "Кольцо не подключено. Подключите кольцо и повторите чтение.");
            return;
        }

        await LoadRingParametersAsync(showUserMessage: true);
    }

    private async Task LoadRingParametersAsync(bool showUserMessage = false)
    {
        if (!IsRingConnected || !_ringPort.IsOpen)
        {
            return;
        }

        StatusText = "Чтение параметров кольца...";
        var readId = AppLogger.NextOperationId("ring-parameters-read");
        var correlation = BuildDiagnosticCorrelation(readId);
        var stopwatch = Stopwatch.StartNew();
        AppLogger.WriteDiagnostic(
            "RING_CALIBRATION",
            "PARAMETERS_READ_BEGIN",
            $"correlation={correlation}|profile={_ringDeviceInfo.ProtocolVersion}|signature={_ringDeviceInfo.Signature?.ToString(CultureInfo.InvariantCulture) ?? "none"}|memory_cells={_ringDeviceInfo.ImpedanceCellCount}|working_cells={RingProtocolConstants.WorkingParameterCellCount}|channel_count={_channelCount}");
        var result = await Task.Run(() => RingConnection.ReadLoopParameters(_ringPort, _ringDeviceInfo, correlation));

        ApplyRingCells(result.Cells);

        var parameterLayout = RingParameterLayout.BuildParameterSet(result.Cells, _channelCount);
        var requiredParameterCount = parameterLayout.RequiredCellCount;
        var completeParameterCount = parameterLayout.CompleteCellCount;
        var hasRequiredParameters = parameterLayout.IsComplete;
        _loopParameters = hasRequiredParameters
            ? parameterLayout.Parameters
            : Array.Empty<LoopParameter>();
        RecalculateLoopReferences();
        ClearResistanceBuffers();
        ClearResistanceCells();
        SetActiveMeasurementPoint(0, 0);
        if (result.Success && hasRequiredParameters)
        {
            AppLogger.Write($"Параметры кольца прочитаны в ViewModel: {_loopParameters.Count} ячеек.");
            for (var i = 0; i < result.Cells.Count; i++)
            {
                var cell = result.Cells[i];
                AppLogger.Write($"Z{i}: R={cell.Resistance:F2}, Re={cell.Reactance:F4}, valid={cell.HasValue}");
                AppLogger.WriteDiagnostic(
                    "RING_CALIBRATION",
                    "PARAMETER_CELL",
                    $"correlation={correlation}|index={cell.Index}|role={(cell.Index < requiredParameterCount ? "required_for_tool" : cell.Index < RingProtocolConstants.WorkingParameterCellCount ? "inactive_working" : "extended_ignored_by_math")}|valid={cell.HasValue}|resistance={cell.Resistance:G9}|reactance={cell.Reactance:G9}|displayed_resistance={cell.Index < RingResistanceRows.Count}|displayed_reactance={cell.Index < RingCalibrationRows.Count}|used_in_math={cell.Index < requiredParameterCount && cell.HasValue}");
            }
            AppLogger.WriteDiagnostic(
                "RING_CALIBRATION",
                "PARAMETERS_READ_COMPLETED",
                $"correlation={correlation}|success={result.Success}|cells={result.Cells.Count}|calculation_parameters={_loopParameters.Count}|complete_parameters={completeParameterCount}|required_parameters={requiredParameterCount}|invalid_resistance_indexes=[{string.Join(",", parameterLayout.InvalidResistanceIndexes)}]|invalid_reactance_indexes=[{string.Join(",", parameterLayout.InvalidReactanceIndexes)}]|has_required={hasRequiredParameters}|phase_reference_sets={_phaseReferences.Count}|resistivity_reference_sets={_resistivityReferences.Count}|elapsed_ms={stopwatch.ElapsedMilliseconds}");
            StatusText = $"Параметры кольца прочитаны: {_loopParameters.Count}";
            if (showUserMessage)
            {
                RaiseUserMessage(
                    "Чтение завершено",
                    $"Из кольца считано {result.Cells.Count} ячеек.\n" +
                    $"Полных расчётных параметров: {_loopParameters.Count} из {requiredParameterCount}.");
            }
        }
        else
        {
            AppLogger.Write("Параметры кольца не получены.");
            AppLogger.WriteDiagnostic(
                "RING_CALIBRATION",
                "PARAMETERS_READ_FAILED",
                $"correlation={correlation}|result_success={result.Success}|cells={result.Cells.Count}|raw_calculation_parameters={result.Parameters.Count}|complete_parameters={completeParameterCount}|required_parameters={requiredParameterCount}|invalid_resistance_indexes=[{string.Join(",", parameterLayout.InvalidResistanceIndexes)}]|invalid_reactance_indexes=[{string.Join(",", parameterLayout.InvalidReactanceIndexes)}]|has_required={hasRequiredParameters}|elapsed_ms={stopwatch.ElapsedMilliseconds}");
            StatusText = result.Cells.Count > 0
                ? $"Параметры кольца неполные: {completeParameterCount} из {requiredParameterCount}"
                : "Параметры кольца не получены";
            if (showUserMessage)
            {
                var message = result.Cells.Count > 0
                    ? $"Ячейки кольца прочитаны, но полных расчётных параметров только {completeParameterCount} из {requiredParameterCount}.\n\n" +
                      "Проверьте R1…R8 и Re meas активных каналов. Нулевое Re meas допустимо только для неактивных каналов."
                    : "Кольцо не вернуло параметры. Проверьте соединение и повторите чтение.";
                RaiseUserMessage(
                    result.Cells.Count > 0 ? "Параметры прочитаны не полностью" : "Чтение не выполнено",
                    message);
            }
        }
    }

    [RelayCommand]
    private async Task WriteRingParametersAsync()
    {
        if (!IsRingConnected || !_ringPort.IsOpen)
        {
            StatusText = "Включите кольцо";
            RaiseUserMessage(
                "Запись не выполнена",
                "Кольцо не подключено. Подключите кольцо и повторите запись.");
            return;
        }

        IReadOnlyList<RingLoopParameterCell> cells;
        try
        {
            cells = BuildRingWriteCells();
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Ошибка подготовки параметров кольца к записи");
            StatusText = $"Ошибка параметров кольца: {ex.Message}";
            RaiseUserMessage(
                "Запись не выполнена",
                $"Нельзя подготовить данные для записи.\n\nПричина: {ex.Message}");
            return;
        }

        StatusText = "Запись параметров кольца...";
        var writeId = AppLogger.NextOperationId("ring-parameters-write");
        var correlation = BuildDiagnosticCorrelation(writeId);
        var stopwatch = Stopwatch.StartNew();
        AppLogger.WriteDiagnostic(
            "RING_CALIBRATION",
            "PARAMETERS_WRITE_BEGIN",
            $"correlation={correlation}|cells={cells.Count}|indexes=[{string.Join(",", cells.Select(cell => cell.Index))}]|preserved_range=Z10..Z31");
        foreach (var cell in cells)
        {
            AppLogger.WriteDiagnostic(
                "RING_CALIBRATION",
                "PARAMETER_WRITE_INPUT",
                $"correlation={correlation}|index={cell.Index}|resistance={cell.Resistance:G9}|reactance={cell.Reactance:G9}|has_value={cell.HasValue}");
        }

        var result = await Task.Run(() => RingConnection.WriteLoopParameters(_ringPort, cells, correlation));
        if (result.Success)
        {
            StatusText = "Проверка записанных параметров...";
            var verificationCorrelation = $"{correlation}:verify";
            var verification = await Task.Run(
                () => RingConnection.ReadLoopParameters(_ringPort, _ringDeviceInfo, verificationCorrelation));
            var verificationFailures = GetRingWriteVerificationFailures(cells, verification.Cells);
            AppLogger.WriteDiagnostic(
                "RING_CALIBRATION",
                verificationFailures.Count == 0 ? "PARAMETERS_WRITE_VERIFIED" : "PARAMETERS_WRITE_VERIFICATION_FAILED",
                $"correlation={correlation}|read_cells={verification.Cells.Count}|failed_indexes=[{string.Join(",", verificationFailures)}]|elapsed_ms={stopwatch.ElapsedMilliseconds}");
            if (verificationFailures.Count > 0)
            {
                var failedCells = string.Join(", ", verificationFailures.Select(index => $"Z{index}"));
                StatusText = $"Запись кольца не подтверждена: {failedCells}";
                RaiseUserMessage(
                    "Запись не подтверждена",
                    $"Кольцо подтвердило команды записи, но обратное чтение не совпало для: {failedCells}.\n\n" +
                    "Повторите запись. Если ошибка повторится, переподключите кольцо.");
                return;
            }

            var writtenIndexes = cells.Select(cell => cell.Index).ToHashSet();
            var verifiedCells = verification.Cells
                .Where(cell => writtenIndexes.Contains(cell.Index))
                .OrderBy(cell => cell.Index)
                .ToArray();
            ApplyRingCells(verifiedCells);
            RebuildLoopParametersFromRingRows();
            RecalculateLoopReferences();
            ClearResistanceBuffers();
            ClearResistanceCells();
            AppLogger.WriteDiagnostic(
                "RING_CALIBRATION",
                "PARAMETERS_WRITE_COMPLETED",
                $"correlation={correlation}|success=true|written={result.Cells.Count}|verified={verifiedCells.Length}|phase_reference_sets={_phaseReferences.Count}|resistivity_reference_sets={_resistivityReferences.Count}|elapsed_ms={stopwatch.ElapsedMilliseconds}");
            StatusText = "Параметры кольца записаны и проверены";
            RaiseUserMessage(
                "Запись завершена",
                $"В кольцо успешно записаны и проверены обратным чтением {verifiedCells.Length} рабочих ячеек.\n" +
                "Неактивные для текущего прибора рабочие ячейки и служебные Z10…Z31 не изменялись.");
        }
        else
        {
            var failed = string.Join(", ", result.Cells.Where(cell => !cell.Success).Select(cell => $"Z{cell.Index}"));
            AppLogger.WriteDiagnostic(
                "RING_CALIBRATION",
                "PARAMETERS_WRITE_FAILED",
                $"correlation={correlation}|success=false|failed=[{failed}]|elapsed_ms={stopwatch.ElapsedMilliseconds}");
            var failureDescription = string.IsNullOrWhiteSpace(failed) ? "нет ответа от кольца" : failed;
            StatusText = $"Ошибка записи кольца: {failureDescription}";
            RaiseUserMessage(
                "Запись не выполнена",
                $"Кольцо не подтвердило запись: {failureDescription}.\n\n" +
                "Приложение уже выполнило повторные попытки. Проверьте соединение с кольцом.");
        }
    }

    [RelayCommand]
    private void TogglePolling()
    {
        if (!IsConnected)
        {
            return;
        }

        if (IsPolling)
        {
            StopPolling();
        }
        else
        {
            StartPolling();
        }
    }

    [RelayCommand]
    private async Task LocalAirAsync()
    {
        var airId = AppLogger.NextOperationId("ring-air");
        var correlation = BuildDiagnosticCorrelation(airId);
        if (!IsConnected)
        {
            AppLogger.WriteDiagnostic(
                "RING_AIR",
                "BLOCKED",
                $"correlation={correlation}|reason=tool_not_connected");
            StatusText = "Подключите прибор";
            return;
        }

        if (IsResistanceMeasurementRunning)
        {
            AppLogger.WriteDiagnostic(
                "RING_AIR",
                "BLOCKED",
                $"correlation={correlation}|reason=measurement_running|measurement_run={_activeMeasurementRunId}");
            StatusText = "Дождитесь завершения измерения сопротивления";
            return;
        }

        bool isSet;
        lock (_airLock)
        {
            isSet = _localAir.IsSet;
        }
        AppLogger.WriteDiagnostic(
            "RING_AIR",
            "REQUESTED",
            $"correlation={correlation}|currently_set={isSet}|ring_connected={IsRingConnected}|ring_port_open={_ringPort.IsOpen}|selected_samples={SelectedFrameCount}|channels={_channelCount}");

        if (!isSet)
        {
            if (IsRingConnected && _ringPort.IsOpen)
            {
                SetActiveMeasurementPoint(0, 0);
                StatusText = "Отключение R/C перед сбором воздуха...";
                var outcome = await Task.Run(() => RingConnection.SetMeasurementPoint(_ringPort, 0, 0, correlation));
                if (!outcome.IsOk)
                {
                    var reason = outcome.FailureReason ?? "Кольцо не подтвердило точку 0/0.";
                    AppLogger.Write($"LOCAL_AIR: сбор не начат, сброс кольца 0/0 завершился ошибкой: {reason}");
                    AppLogger.WriteDiagnostic(
                        "RING_AIR",
                        "RING_RESET_FAILED",
                        $"correlation={correlation}|result={outcome.Result}|reason={SanitizeDiagnosticValue(reason)}");
                    StatusText = $"Локальный воздух не запущен: {reason}";
                    RaiseUserMessage(
                        "Не удалось отключить меры кольца",
                        $"Сбор локального воздуха не начат.\n\nПричина: {reason}");
                    return;
                }

                AppLogger.Write("LOCAL_AIR: кольцо подтвердило RES 0 / CAP 0, начинается сбор.");
                AppLogger.WriteDiagnostic(
                    "RING_AIR",
                    "RING_RESET_ACKNOWLEDGED",
                    $"correlation={correlation}|R=0|C=0");
            }

            lock (_airLock)
            {
                _localAir.Begin(SelectedFrameCount, correlation);
            }

            RefreshLocalAirUi();
            StatusText = "Сбор локального воздуха";
        }
        else
        {
            lock (_airLock)
            {
                _localAir.RequestReset(correlation);
            }

            RefreshLocalAirUi();
            StatusText = "Сброс локального воздуха";
        }
    }

    private async Task<ToolPortOperationResult<T>> ExecuteToolPortOperationAsync<T>(Func<SerialPort, T> operation)
    {
        var selectedPort = SelectedToolPort;
        await _toolIoLock.WaitAsync();
        try
        {
            return await Task.Run(() =>
            {
                if (_toolPort.IsOpen)
                {
                    return ToolPortOperationResult<T>.Ok(operation(_toolPort));
                }

                if (string.IsNullOrWhiteSpace(selectedPort))
                {
                    return ToolPortOperationResult<T>.Fail("Выберите COM-порт прибора");
                }

                using var port = CreateStandaloneToolPort(selectedPort);
                port.Open();
                return ToolPortOperationResult<T>.Ok(operation(port));
            });
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Ошибка операции с COM-портом прибора");
            return ToolPortOperationResult<T>.Fail(ex.Message);
        }
        finally
        {
            _toolIoLock.Release();
        }
    }

    private static SerialPort CreateStandaloneToolPort(string portName) =>
        new(portName)
        {
            BaudRate = ToolProtocolConstants.ToolBaudRate,
            ReadTimeout = ToolProtocolConstants.ReadTimeoutMs,
            WriteTimeout = ToolProtocolConstants.WriteTimeoutMs
        };

    private void ApplyIdentification(in GpToolMetrology metrology)
    {
        var identity = SondeIdentification.FromMetrology(in metrology);
        ToolTypeName = identity.DeviceTypeName;
        IdentificationPanelWidth = ToolTypeName.Length > ExpandedIdentificationTypeNameLength
            ? ExpandedIdentificationPanelWidth
            : CompactIdentificationPanelWidth;
        SignatureText = $"0x{identity.Signature:X8}";
        SerialText = identity.Serial.ToString();
        TransmitterCount = (int)identity.TransmitterCount;
        TransmitterCountText = identity.TransmitterCount.ToString();
        // Позиция R1 отображается как сопоставление приёмника с передатчиком: 0 -> T1, 1 -> T2.
        RxPositionText = $"R1 -> T{identity.RxPosition + 1}";
        ModificationText = identity.Modification.ToString();
        NumberText = identity.Number.ToString();
        PaletteText = identity.PaletteSignature.ToString();
        SignatureBitsText = identity.SignatureBits;
    }

    private void ApplyMetrology(in GpToolMetrology metrology)
    {
        _metrology = metrology;
        _rxPosition = metrology.RxPosition;
        _sondeParameters = MetrologyConverter.Convert(in _metrology);
        _channelCount = _sondeParameters.Count;
        var metrologyId = AppLogger.NextOperationId("ring-metrology");
        var correlation = BuildDiagnosticCorrelation(metrologyId);
        AppLogger.WriteDiagnostic(
            "RING_CALIBRATION",
            "METROLOGY_APPLIED",
            $"correlation={correlation}|signature=0x{metrology.Signature:X8}|serial={metrology.Serial}|work_type={metrology.WorkType}|rx_position={metrology.RxPosition}|diameter_mm={metrology.SondeDiameterMm}|channels={_channelCount}|tx={_channelCount / MetrologyConverter.FrequencyCount}");
        for (var channel = 0; channel < _sondeParameters.Count; channel++)
        {
            var parameter = _sondeParameters[channel];
            AppLogger.WriteDiagnostic(
                "RING_CALIBRATION",
                "METROLOGY_CHANNEL",
                $"correlation={correlation}|channel={channel}|frequency_Hz={parameter.Frequency:G9}|L1_m={parameter.L1:G9}|L2_m={parameter.L2:G9}");
        }

        ApplyIdentification(in _metrology);
        BuildChannels(_sondeParameters, in _metrology);
        BuildRingCalibrationRows(_sondeParameters);
        CalculateRingCalibration();
        ClearResistanceBuffers();
        ClearResistanceCells();
    }

    private void BuildChannels(IReadOnlyList<SondeParameter> sondeParameters, in GpToolMetrology metrology)
    {
        var table = new MeasurementTable();
        table.SetHeaders(sondeParameters);
        var transmitterCount = table.Headers.Count / MetrologyConverter.FrequencyCount;

        Channels.Clear();
        ResistanceRows.Clear();
        foreach (var header in table.Headers)
        {
            Channels.Add(new ChannelReadingViewModel(header.ChannelIndex, header.Label));
            ResistanceRows.Add(new ResistanceTableRowViewModel(
                header.ChannelIndex,
                header.Label,
                GetAirZzText(in metrology, header.ChannelIndex, transmitterCount))
            {
                IndicatorThreshold = ThresholdPercent
            });
        }

        if (ResistanceRows.Count == 0)
        {
            BuildDefaultResistanceTable();
        }

        RefreshLocalAirUi();
        ClearResistanceCells();
        ClearResistanceBuffers();
    }

    private static string GetAirZzText(in GpToolMetrology metrology, int channelIndex, int transmitterCount)
    {
        if (transmitterCount <= 0)
        {
            return string.Empty;
        }

        var frequencyIndex = channelIndex / transmitterCount;
        var transmitterIndex = channelIndex % transmitterCount;
        if (frequencyIndex >= GpToolMetrology.FrequencyCount ||
            transmitterIndex >= GpToolMetrology.MaxTransmitterCount)
        {
            return string.Empty;
        }

        return metrology.GetAirZz(frequencyIndex, transmitterIndex).ToString();
    }

    private void BuildRingCalibrationRows(IReadOnlyList<SondeParameter> sondeParameters)
    {
        var table = new MeasurementTable();
        table.SetHeaders(sondeParameters);

        RingCalibrationRows.Clear();
        RingGeometryRows.Clear();
        RingFrequencyRows.Clear();
        for (var index = 0; index < RingProtocolConstants.WorkingParameterCellCount; index++)
        {
            var label = index < table.Headers.Count
                ? table.Headers[index].Label
                : DefaultRingCalibrationLabels[index];
            var row = new RingCalibrationRowViewModel(index, label);
            if (index < table.Headers.Count)
            {
                var parameter = sondeParameters[table.Headers[index].ChannelIndex];
                row.L1Text = parameter.L1.ToString("F3");
                row.L2Text = parameter.L2.ToString("F3");
                row.FrequencyText = (parameter.Frequency / 1000f).ToString("F0");
            }

            RingCalibrationRows.Add(row);
        }

        var transmitterCount = Math.Min(sondeParameters.Count / MetrologyConverter.FrequencyCount, GpToolMetrology.MaxTransmitterCount);
        for (var tx = 0; tx < transmitterCount; tx++)
        {
            var parameter = sondeParameters[tx];
            RingGeometryRows.Add(new RingCalibrationRowViewModel(tx, $"T{tx + 1}")
            {
                L1Text = parameter.L1.ToString("F3"),
                L2Text = parameter.L2.ToString("F3")
            });
        }

        for (var frequency = 0; frequency < MetrologyConverter.FrequencyCount; frequency++)
        {
            var channelIndex = frequency * transmitterCount;
            var frequencyText = channelIndex < sondeParameters.Count
                ? (sondeParameters[channelIndex].Frequency / 1000f).ToString("F0")
                : string.Empty;
            RingFrequencyRows.Add(new RingCalibrationRowViewModel(frequency, $"F{frequency + 1}, кГц")
            {
                FrequencyText = frequencyText
            });
        }

        if (RingCalibrationRows.Count == 0)
        {
            BuildDefaultRingCalibrationRows();
            return;
        }

        RefreshRingResistanceRows();
        ApplyRingEditModes();
        ClearResistanceBuffers();
    }

    private void BuildDefaultResistanceTable()
    {
        ResistanceRows.Clear();

        string[] labels =
        {
            "T1 401",
            "T2 401",
            "T3 401",
            "T4 401",
            "T5 401",
            "T1 2001",
            "T2 2001",
            "T3 2001",
            "T4 2001",
            "T5 2001"
        };

        for (var i = 0; i < labels.Length; i++)
        {
            ResistanceRows.Add(new ResistanceTableRowViewModel(i, labels[i], string.Empty)
            {
                IndicatorThreshold = ThresholdPercent
            });
        }

        RefreshLocalAirUi();
        ClearResistanceCells();
    }

    private void BuildDefaultRingCalibrationRows()
    {
        RingCalibrationRows.Clear();
        RingGeometryRows.Clear();
        RingFrequencyRows.Clear();

        for (var i = 0; i < DefaultRingCalibrationLabels.Length; i++)
        {
            RingCalibrationRows.Add(new RingCalibrationRowViewModel(i, DefaultRingCalibrationLabels[i]));
        }

        for (var i = 0; i < GpToolMetrology.MaxTransmitterCount; i++)
        {
            RingGeometryRows.Add(new RingCalibrationRowViewModel(i, $"T{i + 1}"));
        }

        for (var i = 0; i < MetrologyConverter.FrequencyCount; i++)
        {
            RingFrequencyRows.Add(new RingCalibrationRowViewModel(i, $"F{i + 1}, кГц"));
        }

        RefreshRingResistanceRows();
        ApplyRingEditModes();
    }

    private void RefreshRingResistanceRows()
    {
        var existingValues = RingResistanceRows
            .GroupBy(row => row.Index)
            .ToDictionary(group => group.Key, group => group.Last().ResistanceText);

        RingResistanceRows.Clear();

        for (var index = 0; index < MeasurementTable.ResistanceCount; index++)
        {
            var resistanceText = existingValues.TryGetValue(index, out var existingValue)
                ? existingValue
                : index < RingCalibrationRows.Count
                    ? RingCalibrationRows[index].ResistanceText
                    : string.Empty;
            RingResistanceRows.Add(new RingCalibrationRowViewModel(index, $"R{index + 1}")
            {
                ResistanceText = resistanceText,
                IsRingValueReadOnly = !IsRingEditMode
            });
        }
    }

    private void ApplyRingEditModes()
    {
        var activeCalculationCellCount = GetActiveRingCalculationCellCount();
        foreach (var row in RingCalibrationRows)
        {
            row.IsRingValueReadOnly = !IsRingEditMode || row.Index >= activeCalculationCellCount;
        }

        foreach (var row in RingResistanceRows)
        {
            row.IsRingValueReadOnly = !IsRingEditMode;
        }
    }

    private IReadOnlyList<SondeParameter> GetRingCalculationParameters()
    {
        return _sondeParameters;
    }

    private int GetActiveRingCalculationCellCount()
    {
        return _sondeParameters.Count > 0
            ? Math.Min(_sondeParameters.Count, RingProtocolConstants.WorkingParameterCellCount)
            : RingCalibrationRows.Count;
    }

    private void ClearInactiveRingCalculationRows(int activeCellCount)
    {
        for (var index = Math.Max(activeCellCount, 0); index < RingCalibrationRows.Count; index++)
        {
            var row = RingCalibrationRows[index];
            row.L1Text = string.Empty;
            row.L2Text = string.Empty;
            row.FrequencyText = string.Empty;
            row.CapacitanceText = string.Empty;
            row.ReactanceCalculatedText = string.Empty;
            row.ReactanceMeasuredText = string.Empty;
        }
    }

    private IReadOnlyList<RingLoopParameterCell> BuildRingWriteCells()
    {
        var activeCalculationCellCount = GetActiveRingCalculationCellCount();
        var cellCount = RingParameterLayout.GetRequiredWorkingCellCount(activeCalculationCellCount);
        var cells = new List<RingLoopParameterCell>(cellCount);
        for (var index = 0; index < cellCount; index++)
        {
            var resistanceRow = index < RingResistanceRows.Count ? RingResistanceRows[index] : null;
            var calibrationRow = index < activeCalculationCellCount && index < RingCalibrationRows.Count
                ? RingCalibrationRows[index]
                : null;
            var resistance = index < _loopParameters.Count
                ? _loopParameters[index].Resistance
                : index == 0 ? 1f : 0f;
            var reactance = index < _loopParameters.Count
                ? _loopParameters[index].Reactance
                : 0f;

            if (resistanceRow is not null && !string.IsNullOrWhiteSpace(resistanceRow.ResistanceText))
            {
                if (!TryParseDouble(resistanceRow.ResistanceText, out var parsedResistance))
                {
                    throw new FormatException($"Некорректное R в Z{index}");
                }

                resistance = (float)parsedResistance;
            }

            if (RingParameterLayout.IsResistanceRequired(index) && resistance <= 0.000001f)
            {
                throw new InvalidOperationException($"Для Z{index} не задано R{index + 1}.");
            }

            if (calibrationRow is not null &&
                !string.IsNullOrWhiteSpace(calibrationRow.ReactanceMeasuredText) &&
                !TryParseDouble(calibrationRow.ReactanceMeasuredText, out _))
            {
                throw new FormatException($"Некорректное Re meas в Z{index}");
            }

            var measuredReactance = 0d;
            var calculatedReactance = 0d;
            var hasMeasuredReactance =
                calibrationRow is not null &&
                TryParseDouble(calibrationRow.ReactanceMeasuredText, out measuredReactance) &&
                measuredReactance > 0.000001;
            var hasCalculatedReactance =
                calibrationRow is not null &&
                TryParseDouble(calibrationRow.ReactanceCalculatedText, out calculatedReactance) &&
                calculatedReactance > 0.000001;
            if (hasMeasuredReactance)
            {
                reactance = (float)measuredReactance;
            }
            else if (hasCalculatedReactance)
            {
                reactance = (float)calculatedReactance;
            }
            else if (calibrationRow is not null)
            {
                throw new InvalidOperationException(
                    $"Для Z{index} не задано Re meas и нет расчётного RE_calc. Нажмите «Рассчитать».");
            }

            cells.Add(new RingLoopParameterCell(index, resistance, reactance, true));
        }

        return cells;
    }

    private static IReadOnlyList<int> GetRingWriteVerificationFailures(
        IReadOnlyList<RingLoopParameterCell> expectedCells,
        IReadOnlyList<RingLoopParameterCell> actualCells)
    {
        var actualByIndex = actualCells.ToDictionary(cell => cell.Index);
        var failed = new List<int>();
        foreach (var expected in expectedCells)
        {
            if (!actualByIndex.TryGetValue(expected.Index, out var actual) ||
                !actual.HasValue ||
                !RingMemoryValueMatches(expected.Resistance, actual.Resistance) ||
                !RingMemoryValueMatches(expected.Reactance, actual.Reactance))
            {
                failed.Add(expected.Index);
            }
        }

        return failed;
    }

    private static bool RingMemoryValueMatches(float expected, float actual)
    {
        var tolerance = Math.Max(0.0001f, Math.Abs(expected) * 0.00001f);
        return Math.Abs(expected - actual) <= tolerance;
    }

    private void ApplyRingCells(IReadOnlyList<RingLoopParameterCell> cells)
    {
        var activeCalculationCellCount = GetActiveRingCalculationCellCount();
        ClearInactiveRingCalculationRows(activeCalculationCellCount);
        foreach (var cell in cells)
        {
            if (cell.Index >= 0 && cell.Index < RingResistanceRows.Count)
            {
                RingResistanceRows[cell.Index].ResistanceText = cell.HasValue
                    ? cell.Resistance.ToString("F0")
                    : string.Empty;
            }

            if (cell.Index >= 0 &&
                cell.Index < activeCalculationCellCount &&
                cell.Index < RingCalibrationRows.Count)
            {
                RingCalibrationRows[cell.Index].ReactanceMeasuredText = cell.HasValue
                    ? cell.Reactance.ToString("F4")
                    : string.Empty;
            }
        }
    }

    private void RebuildLoopParametersFromRingRows()
    {
        var activeCalculationCellCount = GetActiveRingCalculationCellCount();
        var parameterCount = RingParameterLayout.GetRequiredWorkingCellCount(activeCalculationCellCount);
        var parameters = new List<LoopParameter>(parameterCount);
        for (var index = 0; index < parameterCount; index++)
        {
            double resistance = index < _loopParameters.Count ? _loopParameters[index].Resistance : 0f;
            if (index < RingResistanceRows.Count &&
                !TryParseDouble(RingResistanceRows[index].ResistanceText, out resistance))
            {
                _loopParameters = Array.Empty<LoopParameter>();
                return;
            }

            double reactance = index < _loopParameters.Count ? _loopParameters[index].Reactance : 0f;
            if (index < activeCalculationCellCount && index < RingCalibrationRows.Count)
            {
                var row = RingCalibrationRows[index];
                if (!TryParseDouble(row.ReactanceMeasuredText, out reactance) || reactance <= 0.000001)
                {
                    if (!TryParseDouble(row.ReactanceCalculatedText, out reactance) || reactance <= 0.000001)
                    {
                        _loopParameters = Array.Empty<LoopParameter>();
                        return;
                    }
                }
            }

            parameters.Add(new LoopParameter
            {
                Resistance = (float)resistance,
                Reactance = (float)reactance,
                Radius = 0.5f,
                AdditionalWireResistance = 0.5f
            });
        }

        _loopParameters = parameters;
    }

    private void RecalculateLoopReferences()
    {
        var calculationId = AppLogger.NextOperationId("ring-reference-calc");
        var correlation = BuildDiagnosticCorrelation(calculationId);
        var stopwatch = Stopwatch.StartNew();
        var sondeParameters = GetRingCalculationParameters();
        AppLogger.WriteDiagnostic(
            "RING_CALC",
            "REFERENCES_BEGIN",
            $"correlation={correlation}|sonde_parameters={sondeParameters.Count}|loop_parameters={_loopParameters.Count}|channels={_channelCount}");
        if (sondeParameters.Count == 0 || _loopParameters.Count == 0)
        {
            _phaseReferences = Array.Empty<V10>();
            _resistivityReferences = Array.Empty<V10>();
            AppLogger.WriteDiagnostic(
                "RING_CALC",
                "REFERENCES_SKIPPED",
                $"correlation={correlation}|reason={(sondeParameters.Count == 0 ? "no_sonde_parameters" : "no_loop_parameters")}|elapsed_ms={stopwatch.ElapsedMilliseconds}");
            return;
        }

        for (var index = 0; index < _loopParameters.Count; index++)
        {
            var parameter = _loopParameters[index];
            AppLogger.WriteDiagnostic(
                "RING_CALC",
                "REFERENCE_INPUT",
                $"correlation={correlation}|index={index}|resistance={parameter.Resistance:G9}|reactance={parameter.Reactance:G9}|radius={parameter.Radius:G9}|additional_wire_resistance={parameter.AdditionalWireResistance:G9}");
        }

        _phaseReferences = LoopResistivity.CalcPhaseFromLoop(sondeParameters, _loopParameters);
        _resistivityReferences = LoopResistivity.CalcRoFromLoop(sondeParameters, _phaseReferences);

        AppLogger.Write($"Эталонные фазы кольца: {_phaseReferences.Count} точек, эталонные УЭС: {_resistivityReferences.Count} точек.");
        for (var i = 0; i < _resistivityReferences.Count && i <= MeasurementTable.ResistanceCount; i++)
        {
            var reference = _resistivityReferences[i];
            var values = string.Join(", ", Enumerable.Range(0, Math.Min(_channelCount, V10.ChannelCount)).Select(c => $"[{c}]={reference.GetValue(c):F2}"));
            AppLogger.Write($"R{i}: эталонные УЭС={values}");
            var phaseValues = i < _phaseReferences.Count
                ? FormatV10Diagnostic(_phaseReferences[i], _channelCount, (float)PhysicalConstants.Milligrad, "F3")
                : "none";
            var resistivityValues = FormatV10Diagnostic(reference, _channelCount, 1f, "F3");
            AppLogger.WriteDiagnostic(
                "RING_CALC",
                "REFERENCE_SET",
                $"correlation={correlation}|R={i}|phase_mG=[{phaseValues}]|resistivity_ohm_m=[{resistivityValues}]");
        }
        AppLogger.WriteDiagnostic(
            "RING_CALC",
            "REFERENCES_COMPLETED",
            $"correlation={correlation}|phase_sets={_phaseReferences.Count}|resistivity_sets={_resistivityReferences.Count}|elapsed_ms={stopwatch.ElapsedMilliseconds}");
    }

    private static bool TryGetResistanceIndex(object? parameter, out int resistanceIndex)
    {
        switch (parameter)
        {
            case int value:
                resistanceIndex = value;
                break;
            case string text when int.TryParse(text, CultureInfo.InvariantCulture, out var value):
                resistanceIndex = value;
                break;
            default:
                resistanceIndex = 0;
                return false;
        }

        return resistanceIndex is >= 1 and <= MeasurementTable.ResistanceCount;
    }

    private async Task<bool> PrepareResistanceMeasurementAsync(string modeName)
    {
        var preparationId = AppLogger.NextOperationId("measurement-prepare");
        var correlation = BuildDiagnosticCorrelation(preparationId);
        AppLogger.WriteDiagnostic(
            "RING_MEASUREMENT",
            "PREPARE_BEGIN",
            $"correlation={correlation}|mode={SanitizeDiagnosticValue(modeName)}|tool_connected={IsConnected}|ring_connected={IsRingConnected}|ring_port_open={_ringPort.IsOpen}|local_air_set={IsLocalAirSet()}|loop_parameters={_loopParameters.Count}|phase_reference_sets={_phaseReferences.Count}|resistivity_reference_sets={_resistivityReferences.Count}|channels={_channelCount}");
        if (!IsConnected || !IsRingConnected || !_ringPort.IsOpen)
        {
            AppLogger.Write($"{modeName}: прибор или кольцо не подключены.");
            AppLogger.WriteDiagnostic(
                "RING_MEASUREMENT",
                "PREPARE_BLOCKED",
                $"correlation={correlation}|reason=connection_state|tool_connected={IsConnected}|ring_connected={IsRingConnected}|ring_port_open={_ringPort.IsOpen}");
            StatusText = $"{modeName}: подключите прибор и кольцо";
            return false;
        }

        if (!IsLocalAirSet())
        {
            const string message = "Сначала соберите локальный воздух, затем повторите заполнение таблицы.";
            AppLogger.Write($"{modeName}: запуск заблокирован, локальный воздух не задан.");
            AppLogger.WriteDiagnostic(
                "RING_MEASUREMENT",
                "PREPARE_BLOCKED",
                $"correlation={correlation}|reason=local_air_not_set");
            StatusText = $"{modeName}: соберите локальный воздух";
            RaiseUserMessage("Локальный воздух не задан", message);
            return false;
        }

        if (_loopParameters.Count == 0)
        {
            AppLogger.Write($"{modeName}: параметры кольца не загружены, читаем...");
            await LoadRingParametersAsync();
        }

        if (_channelCount <= 0 || _resistivityReferences.Count <= MeasurementTable.ResistanceCount)
        {
            AppLogger.Write($"{modeName}: нет расчётных эталонов кольца.");
            AppLogger.WriteDiagnostic(
                "RING_MEASUREMENT",
                "PREPARE_BLOCKED",
                $"correlation={correlation}|reason=reference_data_missing|channels={_channelCount}|phase_reference_sets={_phaseReferences.Count}|resistivity_reference_sets={_resistivityReferences.Count}|required_sets={MeasurementTable.ResistanceCount + 1}");
            StatusText = $"{modeName}: нет расчётных эталонов кольца";
            return false;
        }

        AppLogger.WriteDiagnostic(
            "RING_MEASUREMENT",
            "PREPARE_COMPLETED",
            $"correlation={correlation}|loop_parameters={_loopParameters.Count}|phase_reference_sets={_phaseReferences.Count}|resistivity_reference_sets={_resistivityReferences.Count}|channels={_channelCount}");
        return true;
    }

    private bool IsLocalAirSet()
    {
        lock (_airLock)
        {
            return _localAir.IsSet;
        }
    }

    private async Task<bool> RunResistanceMeasurementAsync(int resistanceIndex, string modeName, CancellationToken token)
    {
        ClearResistanceCells(resistanceIndex);
        ClearResistanceBuffer(resistanceIndex);
        AppLogger.WriteDiagnostic(
            "RING_MEASUREMENT",
            "RESISTANCE_BEGIN",
            $"run={_activeMeasurementRunId}|R={resistanceIndex}|mode={SanitizeDiagnosticValue(modeName)}|channel_count={_channelCount}|express={ExpressMode}|scan_strategy={RingMeasurementSequence.GetStrategyName(ExpressMode, resistanceIndex)}");

        for (var capacitanceIndex = 1;
             capacitanceIndex <= _channelCount;
             capacitanceIndex = GetNextCapacitanceIndex(resistanceIndex, capacitanceIndex))
        {
            var pointId = AppLogger.NextOperationId("measurement-point");
            var pointCorrelation = $"{_activeMeasurementRunId}:{pointId}";
            var pointStopwatch = Stopwatch.StartNew();
            var activeChannels = GetActiveMeasurementChannels(resistanceIndex, capacitanceIndex, _channelCount).ToArray();
            var commandCapacitance = RingMeasurementSequence.GetCommandCapacitance(
                _channelCount,
                capacitanceIndex);
            var capacitanceShiftApplied = commandCapacitance != capacitanceIndex;
            token.ThrowIfCancellationRequested();
            SetActiveMeasurementPoint(0, 0);
            AppLogger.WriteDiagnostic(
                "RING_MEASUREMENT",
                "POINT_PREPARE",
                $"run={_activeMeasurementRunId}|point={pointCorrelation}|R={resistanceIndex}|logical_C={capacitanceIndex}|command_C={commandCapacitance}|expected_physical_C={commandCapacitance}|mapping_owner=application|shift_applied={capacitanceShiftApplied}|scan_strategy={RingMeasurementSequence.GetStrategyName(ExpressMode, resistanceIndex)}|active_channels=[{string.Join(",", activeChannels)}]|skip_frames={MeasurementSkipFrames}|averaging={SelectedAveraging}|delay_before_command_ms=200");

            StatusText = $"{modeName}: R{resistanceIndex}, C{capacitanceIndex}";
            LogCapMap(resistanceIndex, capacitanceIndex, commandCapacitance);
            AppLogger.Write(
                $"{modeName}: задержка 200 мс перед установкой R{resistanceIndex}/C{capacitanceIndex} " +
                $"(команда CAP {commandCapacitance}, как в R_Click).");
            await Task.Delay(200, token);

            AppLogger.Write(
                $"{modeName}: установка R{resistanceIndex}/C{capacitanceIndex} " +
                $"(команда CAP {commandCapacitance})...");
            var outcome = await Task.Run(
                () => RingConnection.SetMeasurementPoint(_ringPort, resistanceIndex, commandCapacitance, pointCorrelation),
                token);
            if (!outcome.IsOk)
            {
                _diagnosticFailedPoints++;
                var reason = outcome.FailureReason ?? $"Кольцо не ответило ({outcome.Result}).";
                AppLogger.Write(
                    $"{modeName}: ошибка установки R{resistanceIndex}/C{capacitanceIndex} ({outcome.Result}): {reason}");
                AppLogger.WriteDiagnostic(
                    "RING_MEASUREMENT",
                    "POINT_FAILED",
                    $"run={_activeMeasurementRunId}|point={pointCorrelation}|R={resistanceIndex}|logical_C={capacitanceIndex}|command_C={commandCapacitance}|result={outcome.Result}|reason={SanitizeDiagnosticValue(reason)}|elapsed_ms={pointStopwatch.ElapsedMilliseconds}");
                StatusText = $"{modeName} остановлен: {reason}";
                lock (_autoLock)
                {
                    _autoMeasurementCompleteTcs = null;
                    SetActiveMeasurementPoint(0, 0);
                }

                RaiseUserMessage(
                    "Не удалось установить точку на кольце",
                    $"{modeName} остановлен на точке R{resistanceIndex}/C{capacitanceIndex}.\n\n" +
                    $"Причина: {reason}\n\n" +
                    $"Проверьте питание и подключение кольца (порт {_ringPort.PortName}), затем переподключите кольцо и повторите.");
                return false;
            }

            // Счётчик кадров активируется только после подтверждения RES/CAP.
            // Поэтому ни один кадр старой позиции не попадёт в новую точку.
            var pointCompleteTcs = new TaskCompletionSource<bool>(TaskCreationOptions.RunContinuationsAsynchronously);
            lock (_autoLock)
            {
                _autoMeasurementCompleteTcs = pointCompleteTcs;
                SetActiveMeasurementPoint(resistanceIndex, capacitanceIndex, pointCorrelation);
            }

            AppLogger.Write(
                $"{modeName}: R{resistanceIndex}/C{capacitanceIndex} подтверждён кольцом, ожидание усреднения кадров...");
            AppLogger.WriteDiagnostic(
                "RING_MEASUREMENT",
                "POINT_ACKNOWLEDGED",
                $"run={_activeMeasurementRunId}|point={pointCorrelation}|R={resistanceIndex}|logical_C={capacitanceIndex}|command_C={commandCapacitance}|active_channels=[{string.Join(",", activeChannels)}]|elapsed_ms={pointStopwatch.ElapsedMilliseconds}");
            AppLogger.WriteDiagnostic(
                "RING_FIRMWARE_DIAG",
                "POINT_MONITOR_BEGIN",
                $"workflow={CurrentRingWorkflowId}|run={_activeMeasurementRunId}|point={pointCorrelation}|R={resistanceIndex}|logical_C={capacitanceIndex}|command_C={commandCapacitance}|active_channels=[{string.Join(",", activeChannels)}]|diagnostic_mode=passive_phase_observation|command_ack=true|state_readback_supported=false|protocol_note=no_RESREAD_or_CAPREAD");
            using (token.Register(() => pointCompleteTcs.TrySetCanceled(token)))
            {
                await pointCompleteTcs.Task;
            }
            _diagnosticCompletedPoints++;
            AppLogger.WriteDiagnostic(
                "RING_MEASUREMENT",
                "POINT_COMPLETED",
                $"run={_activeMeasurementRunId}|point={pointCorrelation}|R={resistanceIndex}|logical_C={capacitanceIndex}|command_C={commandCapacitance}|required_frames={MeasurementSkipFrames + SelectedAveraging}|active_channels=[{string.Join(",", activeChannels)}]|cells=[{FormatResistanceCellsDiagnostic(resistanceIndex, activeChannels)}]|elapsed_ms={pointStopwatch.ElapsedMilliseconds}");
        }

        RefreshReportSelectionForResistance(resistanceIndex);
        AppLogger.Write($"{modeName}: R{resistanceIndex} измерено.");
        AppLogger.WriteDiagnostic(
            "RING_MEASUREMENT",
            "RESISTANCE_COMPLETED",
            $"run={_activeMeasurementRunId}|R={resistanceIndex}|selected_for_report={IsReportResistanceSelected(resistanceIndex)}|cells=[{FormatResistanceColumnDiagnostic(resistanceIndex)}]");
        return true;
    }

    private async Task RunAutoModeAsync()
    {
        AppLogger.Write("=== ЗАПУСК РЕЖИМА АВТО ===");
        if (!await PrepareResistanceMeasurementAsync("Режим АВТО"))
        {
            IsAutoMode = false;
            return;
        }

        _autoCts?.Cancel();
        _autoCts = new CancellationTokenSource();
        var token = _autoCts.Token;
        IsResistanceMeasurementRunning = true;
        var diagnosticRun = BeginMeasurementDiagnosticRun("auto", null);
        var diagnosticStatus = "failed";

        ClearResistanceCells();
        ClearResistanceBuffers();
        SetActiveMeasurementPoint(0, 0);
        StatusText = "Режим АВТО: запуск";
        AppLogger.Write("Режим АВТО: начало цикла R1..R8 (эталонный алгоритм).");
        AppLogger.Write(
            $"AUTO_DIAG_CONFIG: channel_count={_channelCount}, skip_frames={MeasurementSkipFrames}, " +
            $"averaging={SelectedAveraging}, express={ExpressMode}, express_group_size={GetExpressGroupSize(_channelCount)}, " +
            $"poll_interval_ms={PollIntervalMs}, threshold_percent={ThresholdPercent}");

        try
        {
            for (var resistanceIndex = 1; resistanceIndex <= MeasurementTable.ResistanceCount; resistanceIndex++)
            {
                token.ThrowIfCancellationRequested();

                AppLogger.Write($"АВТО: старт измерения R{resistanceIndex}.");
                var measured = await RunResistanceMeasurementAsync(resistanceIndex, "Режим АВТО", token);
                if (!measured)
                {
                    return;
                }
            }

            AppLogger.Write("=== РЕЖИМ АВТО ЗАВЕРШЁН УСПЕШНО ===");
            diagnosticStatus = "completed";
        }
        catch (OperationCanceledException)
        {
            diagnosticStatus = "cancelled";
            AppLogger.Write("Режим АВТО: отменён пользователем.");
            StatusText = "Режим АВТО остановлен";
        }
        finally
        {
            lock (_autoLock)
            {
                _autoMeasurementCompleteTcs = null;
            }

            SetActiveMeasurementPoint(0, 0);
            if (_ringPort.IsOpen)
            {
                AppLogger.Write("Режим АВТО: сброс кольца в 0/0.");
                await Task.Run(
                    () => RingConnection.SetMeasurementPoint(_ringPort, 0, 0, diagnosticRun),
                    CancellationToken.None);
            }

            if (IsAutoMode)
            {
                IsAutoMode = false;
            }

            IsResistanceMeasurementRunning = false;
            EndMeasurementDiagnosticRun(diagnosticRun, diagnosticStatus);
        }
    }

    // Завершает сбор текущей подтверждённой точки. Следующая команда кольцу
    // отправляется только ожидающим async-циклом RunResistanceMeasurementAsync.
    private void AdvanceAutoMeasurement()
    {
        TaskCompletionSource<bool>? tcsToComplete = null;

        lock (_autoLock)
        {
            if (!IsResistanceMeasurementRunning || _autoMeasurementCompleteTcs is null)
            {
                return;
            }

            lock (_measurementLock)
            {
                if (_activeResistanceIndex <= 0 || _activeCapacitanceIndex <= 0)
                {
                    return;
                }

                var requiredFrames = MeasurementSkipFrames + SelectedAveraging;
                AppLogger.Write($"АВТО: R={_activeResistanceIndex}, C={_activeCapacitanceIndex}, кадр={_activeMeasurementFrameCount}/{requiredFrames}");
                AppLogger.WriteDiagnostic(
                    "RING_MEASUREMENT",
                    "POINT_FRAME_PROGRESS",
                    $"run={_activeMeasurementRunId}|R={_activeResistanceIndex}|C={_activeCapacitanceIndex}|frame={_activeMeasurementFrameCount}|required_frames={requiredFrames}|stage={(_activeMeasurementFrameCount <= MeasurementSkipFrames ? "settling" : "averaging")}");

                if (_activeMeasurementFrameCount < requiredFrames)
                {
                    return;
                }

                AppLogger.Write(
                    $"АВТО: сбор R={_activeResistanceIndex}, C={_activeCapacitanceIndex} завершён; " +
                    "ожидается подтверждённое переключение следующей точки.");
                AppLogger.WriteDiagnostic(
                    "RING_MEASUREMENT",
                    "POINT_FRAME_TARGET_REACHED",
                    $"run={_activeMeasurementRunId}|R={_activeResistanceIndex}|C={_activeCapacitanceIndex}|frames={_activeMeasurementFrameCount}|required_frames={requiredFrames}");
                LogActiveFirmwarePointSummaryNoLock("frame_target_reached");
                tcsToComplete = _autoMeasurementCompleteTcs;
                _autoMeasurementCompleteTcs = null;
                SetActiveMeasurementPoint(0, 0);
            }
        }

        tcsToComplete?.TrySetResult(true);
    }

    private void StartPolling()
    {
        if (_pollCts is not null)
        {
            return;
        }

        _pollCts = new CancellationTokenSource();
        var token = _pollCts.Token;
        IsPolling = true;
        _ = Task.Run(() => PollLoopAsync(token));
    }

    private void StopPolling()
    {
        _pollCts?.Cancel();
        _pollCts = null;
        IsPolling = false;
    }

    // Фоновый цикл выполняет блокирующее чтение COM вне UI-потока и публикует результат через DispatcherQueue.
    private async Task PollLoopAsync(CancellationToken token)
    {
        AppLogger.Write("--- Запуск фонового опроса прибора ---");
        while (!token.IsCancellationRequested)
        {
            if (_toolPort.IsOpen)
            {
                await _toolIoLock.WaitAsync(token);
                ToolPort.PhaseFrameResult frame;
                try
                {
                    if (!_toolPort.IsOpen)
                    {
                        continue;
                    }

                    frame = ToolPort.GetPhaseFromTool(_toolPort, ToolProtocolConstants.CmdGetData, in _metrology);
                }
                finally
                {
                    _toolIoLock.Release();
                }

                AppLogger.Write($"Получен кадр от прибора: Code={frame.Code}, RxPosition={_rxPosition}");

                if (frame.Code == ToolResponseCode.Ok)
                {
                    V10 air;
                    V10 corrected;
                    lock (_airLock)
                    {
                        corrected = _localAir.ProcessFrame(frame.Phases, _channelCount);
                        air = _localAir.Air;
                    }

                    LogFramePhases("FRAME_PHASES raw_delta_normalized", frame.RawPhases, _channelCount);
                    LogFramePhases("FRAME_PHASES oriented_before_air", frame.Phases, _channelCount);
                    LogFramePhases("FRAME_PHASES local_air", air, _channelCount);
                    LogFramePhases("FRAME_PHASES corrected_after_air", corrected, _channelCount);

                    var resistivity = LoopResistivity.RoDfi(_sondeParameters, corrected);
                    AppLogger.Write("Вычислены УЭС по скорректированным фазам.");
                    LogRingFramePipeline(frame.RawPhases, frame.Phases, air, corrected, resistivity, _frameCounter + 1, frame.Temperature);

                    _frameCounter++;
                    DispatchFrame(frame.Phases, air, corrected, resistivity, _frameCounter, frame.Temperature);
                }
                else
                {
                    AppLogger.Write($"ОШИБКА опроса прибора: {frame.Code}");
                    DispatchStatus($"Ошибка опроса: {frame.Code}");
                }
            }

            try
            {
                await Task.Delay(TimeSpan.FromMilliseconds(Math.Max(700, PollIntervalMs)), token);
            }
            catch (TaskCanceledException)
            {
                AppLogger.Write("--- Фоновый опрос прибора остановлен ---");
                break;
            }
        }
    }

    private void LogFramePhases(string header, V10 phases, int channelCount)
    {
        var count = Math.Min(channelCount, V10.ChannelCount);
        var values = string.Join(", ", Enumerable.Range(0, count).Select(i => $"[{i}]={(phases.GetValue(i) * PhysicalConstants.Milligrad):F1}"));
        AppLogger.Write($"{header} (мГ): {values}");
    }

    private void LogRingFramePipeline(
        V10 raw,
        V10 oriented,
        V10 air,
        V10 corrected,
        V10 resistivity,
        int globalFrame,
        float temperature)
    {
        var active = GetActiveMeasurementSnapshot();
        var pointFrame = active.ResistanceIndex > 0 && active.CapacitanceIndex > 0
            ? active.FrameIndex + 1
            : 0;
        var workflow = CurrentRingWorkflowId;
        var run = _activeMeasurementRunId;
        AppLogger.WriteDiagnostic(
            "RING_FRAME",
            "RAW_DELTA_NORMALIZED",
            $"workflow={workflow}|run={run}|point={active.PointId}|global_frame={globalFrame}|point_frame={pointFrame}|R={active.ResistanceIndex}|C={active.CapacitanceIndex}|temperature_C={temperature:F3}|values_mG=[{FormatV10Diagnostic(raw, _channelCount, (float)PhysicalConstants.Milligrad, "F3")}]");
        AppLogger.WriteDiagnostic(
            "RING_FRAME",
            "ORIENTED_BEFORE_AIR",
            $"workflow={workflow}|run={run}|point={active.PointId}|global_frame={globalFrame}|point_frame={pointFrame}|R={active.ResistanceIndex}|C={active.CapacitanceIndex}|rx_position={_rxPosition}|values_mG=[{FormatV10Diagnostic(oriented, _channelCount, (float)PhysicalConstants.Milligrad, "F3")}]");
        AppLogger.WriteDiagnostic(
            "RING_FRAME",
            "LOCAL_AIR",
            $"workflow={workflow}|run={run}|point={active.PointId}|global_frame={globalFrame}|point_frame={pointFrame}|R={active.ResistanceIndex}|C={active.CapacitanceIndex}|is_set={IsLocalAirSet()}|values_mG=[{FormatV10Diagnostic(air, _channelCount, (float)PhysicalConstants.Milligrad, "F3")}]");
        AppLogger.WriteDiagnostic(
            "RING_FRAME",
            "CORRECTED_AFTER_AIR",
            $"workflow={workflow}|run={run}|point={active.PointId}|global_frame={globalFrame}|point_frame={pointFrame}|R={active.ResistanceIndex}|C={active.CapacitanceIndex}|values_mG=[{FormatV10Diagnostic(corrected, _channelCount, (float)PhysicalConstants.Milligrad, "F3")}]");
        AppLogger.WriteDiagnostic(
            "RING_FRAME",
            "RESISTIVITY",
            $"workflow={workflow}|run={run}|point={active.PointId}|global_frame={globalFrame}|point_frame={pointFrame}|R={active.ResistanceIndex}|C={active.CapacitanceIndex}|values_ohm_m=[{FormatV10Diagnostic(resistivity, _channelCount, 1f, "F3")}]");
    }

    private void DispatchFrame(V10 oriented, V10 air, V10 corrected, V10 resistivity, int frameNumber, float temperature)
    {
        var orientedCopy = oriented;
        var airCopy = air;
        var correctedCopy = corrected;
        var resistivityCopy = resistivity;

        App.DispatcherQueue.TryEnqueue(() =>
        {
            FrameCount = frameNumber;
            TemperatureText = temperature.ToString("F1", CultureInfo.InvariantCulture) + " °C";
            var count = Math.Min(Channels.Count, Math.Min(ResistanceRows.Count, V10.ChannelCount));
            for (var i = 0; i < count; i++)
            {
                Channels[i].RawText = (orientedCopy.GetValue(i) * PhysicalConstants.Milligrad).ToString("F1");
                Channels[i].AirText = (airCopy.GetValue(i) * PhysicalConstants.Milligrad).ToString("F1");
                Channels[i].CorrectedText = (correctedCopy.GetValue(i) * PhysicalConstants.Milligrad).ToString("F1");
                ResistanceRows[i].AirText = (correctedCopy.GetValue(i) * PhysicalConstants.Milligrad).ToString("F0");
            }

            UpdateResistanceRows(resistivityCopy, correctedCopy);
            AdvanceAutoMeasurement();
            RefreshLocalAirUi();
        });
    }

    private void RefreshLocalAirUi()
    {
        bool isSet;
        bool isCollecting;
        bool resetRequested;
        int collectedSampleCount;

        lock (_airLock)
        {
            isSet = _localAir.IsSet;
            isCollecting = _localAir.Collecting;
            resetRequested = _localAir.ResetRequested;
            collectedSampleCount = _localAir.CollectedSampleCount;
        }

        LocalAirState = resetRequested
            ? "Сброс"
            : isSet
                ? "Задан"
                : isCollecting
                    ? $"Сбор: {collectedSampleCount} кадров"
                    : "Не задан";
        var showAirAsSet = isSet && !resetRequested;
        LocalAirButtonText = isCollecting
            ? $"Сбор: {collectedSampleCount} кадров"
            : showAirAsSet
                ? "Воздух: задан"
                : "Локальный воздух";
        LocalAirButtonFontWeight = showAirAsSet ? FontWeights.SemiBold : FontWeights.Normal;

        var airBackground = showAirAsSet ? LocalAirSetBrush : AirColumnDefaultBrush;
        var airForeground = showAirAsSet ? AirColumnSetForegroundBrush : AirColumnDefaultForegroundBrush;
        AirColumnBackground = airBackground;
        AirColumnForeground = airForeground;
    }

    private void UpdateResistanceRows(V10 resistivity, V10 corrected)
    {
        var count = Math.Min(ResistanceRows.Count, Math.Min(_channelCount, V10.ChannelCount));
        var uesValues = string.Join(", ", Enumerable.Range(0, count).Select(i => $"[{i}]={FormatResistivity(resistivity.GetValue(i))}"));
        AppLogger.Write($"УЭС по каналам: {uesValues}");

        for (var i = 0; i < count; i++)
        {
            ResistanceRows[i].UesText = FormatResistivity(resistivity.GetValue(i));
        }

        UpdateReferenceResistanceCells(resistivity, corrected, count);
        UpdateChannelIndicators(corrected, count);
    }

    private void UpdateChannelIndicators(V10 corrected, int channelCount)
    {
        if (_phaseReferences.Count <= MeasurementTable.ResistanceCount || _activeResistanceIndex <= 0)
        {
            for (var i = 0; i < channelCount; i++)
            {
                ResistanceRows[i].IndicatorValue = 0.0;
                ResistanceRows[i].IndicatorAverageValue = 0.0;
            }

            return;
        }

        var reference = _phaseReferences[_activeResistanceIndex];
        for (var i = 0; i < channelCount; i++)
        {
            var referenceValue = reference.GetValue(i);
            var measuredValue = corrected.GetValue(i);
            if (!float.IsFinite(referenceValue) || !float.IsFinite(measuredValue))
            {
                ResistanceRows[i].IndicatorValue = 0.0;
                ResistanceRows[i].IndicatorAverageValue = 0.0;
                continue;
            }

            var currentPercent = CalculatePhasePercentSample(measuredValue, referenceValue);
            ResistanceRows[i].IndicatorValue = currentPercent;

            var buffer = _resistancePercentBuffers[_activeResistanceIndex - 1, i];
            ResistanceRows[i].IndicatorAverageValue = buffer is { Count: > 0 } ? buffer.Average() : 0.0;
        }
    }

    private void UpdateReferenceResistanceCells(in V10 resistivity, in V10 corrected, int channelCount)
    {
        if (_phaseReferences.Count <= MeasurementTable.ResistanceCount)
        {
            return;
        }

        var active = AdvanceActiveMeasurementFrame();
        if (active.ResistanceIndex <= 0 ||
            active.CapacitanceIndex <= 0)
        {
            return;
        }

        var resistanceIndex = active.ResistanceIndex;
        var channelIndex = active.CapacitanceIndex - 1;
        LogAutoFrameDiagnostics(resistivity, corrected, resistanceIndex, active.CapacitanceIndex, active.FrameIndex, channelCount);
        ObserveActiveFirmwarePoint(corrected, resistanceIndex, active.CapacitanceIndex, active.FrameIndex, channelCount);

        if (active.FrameIndex <= MeasurementSkipFrames)
        {
            return;
        }

        var activeChannels = GetActiveMeasurementChannels(resistanceIndex, active.CapacitanceIndex, channelCount).ToArray();
        if (activeChannels.Length > 0)
        {
            foreach (var activeChannel in activeChannels)
            {
                UpdateReferenceResistanceCellForChannel(
                    resistivity,
                    corrected,
                    resistanceIndex,
                    activeChannel,
                    active.CapacitanceIndex,
                    active.FrameIndex);
            }

            return;
        }

        if (resistanceIndex > MeasurementTable.ResistanceCount ||
            channelIndex < 0 ||
            channelIndex >= channelCount ||
            channelIndex >= ResistanceRows.Count)
        {
            return;
        }

        var reference = _resistivityReferences[resistanceIndex];
        var referenceValue = reference.GetValue(channelIndex);
        var measuredValue = resistivity.GetValue(channelIndex);

        var phaseReference = _phaseReferences[resistanceIndex];
        var referencePhase = phaseReference.GetValue(channelIndex);
        var measuredPhase = corrected.GetValue(channelIndex);
        var (freqLabel, txIndex) = GetChannelFrequencyAndTx(channelIndex);

        if (!float.IsFinite(referencePhase) || !float.IsFinite(measuredPhase))
        {
            AppLogger.Write(
                $"R{resistanceIndex} (канал {channelIndex}, {freqLabel} Tx{txIndex}): эталон УЭС={referenceValue:F2}, измерено={measuredValue:F2} — пропуск (некорректные значения).");
            ResistanceRows[channelIndex].SetResistanceCell(resistanceIndex, MeasurementCell.Empty);
            return;
        }

        var percent = CalculatePhasePercentSample(measuredPhase, referencePhase);
        var averaged = PushResistancePercent(resistanceIndex - 1, channelIndex, percent);
        AppLogger.Write(
            $"R{resistanceIndex} (канал {channelIndex}, {freqLabel} Tx{txIndex}): " +
            $"эталон УЭС={referenceValue:F2}, измерено={measuredValue:F2}, отклонение={percent:F2}%, усреднённое={averaged:F2}%; " +
            $"эталон фаза={referencePhase * PhysicalConstants.Milligrad:F1}мГ, измерено фаза={measuredPhase * PhysicalConstants.Milligrad:F1}мГ");
        ResistanceRows[channelIndex].SetResistanceCell(
            resistanceIndex,
            MeasurementCell.Create(averaged, ThresholdPercent));
    }

    private void LogAutoFrameDiagnostics(
        V10 resistivity,
        V10 corrected,
        int resistanceIndex,
        int capacitanceIndex,
        int frameIndex,
        int channelCount)
    {
        var count = Math.Min(channelCount, V10.ChannelCount);
        if (count <= 0)
        {
            return;
        }

        var expectedChannel = capacitanceIndex - 1;
        var activeChannels = GetActiveMeasurementChannels(resistanceIndex, capacitanceIndex, count).ToArray();
        var topChannels = Enumerable.Range(0, count)
            .OrderByDescending(channel => MathF.Abs(corrected.GetValue(channel)))
            .Take(Math.Min(5, count))
            .Select(channel => FormatChannelRuntimeDiagnostic(resistivity, corrected, resistanceIndex, channel));
        var halfCount = Math.Max(1, (count + 1) / 2);
        var lowChannels = Enumerable.Range(0, Math.Min(halfCount, count)).ToArray();
        var highChannels = Enumerable.Range(halfCount, Math.Max(0, count - halfCount)).ToArray();

        AppLogger.Write(
            $"AUTO_FRAME_DIAG: global_frame={_frameCounter}, R={resistanceIndex}, C={capacitanceIndex}, auto_frame={frameIndex}, " +
            $"skip_frames={MeasurementSkipFrames}, averaging={SelectedAveraging}, expected_channel={expectedChannel}, " +
            $"active_channels=[{string.Join(",", activeChannels)}], expected={FormatChannelRuntimeDiagnostic(resistivity, corrected, resistanceIndex, expectedChannel)}, " +
            $"top_abs_phase=[{string.Join("; ", topChannels)}], " +
            $"low_group={FormatPhaseGroupDiagnostic("low", lowChannels, resistivity, corrected)}, " +
            $"high_group={FormatPhaseGroupDiagnostic("high", highChannels, resistivity, corrected)}");
        AppLogger.WriteDiagnostic(
            "RING_FRAME",
            "MEASUREMENT_DIAGNOSTIC",
            $"workflow={CurrentRingWorkflowId}|run={_activeMeasurementRunId}|global_frame={_frameCounter}|R={resistanceIndex}|C={capacitanceIndex}|point_frame={frameIndex}|skip_frames={MeasurementSkipFrames}|averaging={SelectedAveraging}|expected_channel={expectedChannel}|active_channels=[{string.Join(",", activeChannels)}]|expected={FormatChannelRuntimeDiagnostic(resistivity, corrected, resistanceIndex, expectedChannel)}|top_abs_phase=[{string.Join("; ", topChannels)}]|low_group={FormatPhaseGroupDiagnostic("low", lowChannels, resistivity, corrected)}|high_group={FormatPhaseGroupDiagnostic("high", highChannels, resistivity, corrected)}");
    }

    private string FormatPhaseGroupDiagnostic(
        string name,
        IReadOnlyList<int> channels,
        V10 resistivity,
        V10 corrected)
    {
        if (channels.Count == 0)
        {
            return $"{name}:empty";
        }

        var topChannel = channels
            .OrderByDescending(channel => MathF.Abs(corrected.GetValue(channel)))
            .First();
        var averageAbsPhaseMg = channels
            .Average(channel => MathF.Abs(corrected.GetValue(channel) * (float)PhysicalConstants.Milligrad));

        return
            $"{name}:top=ch{topChannel},top_phase_mG={corrected.GetValue(topChannel) * PhysicalConstants.Milligrad:F1}," +
            $"top_ues={FormatDiagnosticResistivity(resistivity.GetValue(topChannel))},avg_abs_phase_mG={averageAbsPhaseMg:F1}";
    }

    private string FormatChannelRuntimeDiagnostic(
        V10 resistivity,
        V10 corrected,
        int resistanceIndex,
        int channelIndex)
    {
        if (channelIndex < 0 || channelIndex >= Math.Min(_channelCount, V10.ChannelCount))
        {
            return "none";
        }

        var (freqLabel, txIndex) = GetChannelFrequencyAndTx(channelIndex);
        var measuredPhaseMg = corrected.GetValue(channelIndex) * (float)PhysicalConstants.Milligrad;
        var measuredResistivity = resistivity.GetValue(channelIndex);
        var referencePhase = _phaseReferences.Count > resistanceIndex
            ? _phaseReferences[resistanceIndex].GetValue(channelIndex)
            : 0f;
        var referenceResistivity = _resistivityReferences.Count > resistanceIndex
            ? _resistivityReferences[resistanceIndex].GetValue(channelIndex)
            : 0f;

        return
            $"ch{channelIndex}:{freqLabel}Tx{txIndex},phase_mG={measuredPhaseMg:F1}," +
            $"ues={FormatDiagnosticResistivity(measuredResistivity)},ref_phase_mG={referencePhase * PhysicalConstants.Milligrad:F1}," +
            $"ref_ues={FormatDiagnosticResistivity(referenceResistivity)}";
    }

    private int GetNextCapacitanceIndex(int resistanceIndex, int capacitanceIndex)
    {
        return RingMeasurementSequence.GetNextLogicalCapacitance(
            ExpressMode,
            resistanceIndex,
            capacitanceIndex,
            _channelCount);
    }

    private IEnumerable<int> GetActiveMeasurementChannels(int resistanceIndex, int capacitanceIndex, int channelCount)
    {
        return RingMeasurementSequence.GetActiveChannels(
            ExpressMode,
            resistanceIndex,
            capacitanceIndex,
            channelCount);
    }

    private static int GetExpressGroupSize(int channelCount)
    {
        return RingMeasurementSequence.GetExpressGroupSize(channelCount);
    }

    private void UpdateReferenceResistanceCellForChannel(
        V10 resistivity,
        V10 corrected,
        int resistanceIndex,
        int channelIndex,
        int capacitanceIndex,
        int frameIndex)
    {
        if (resistanceIndex > MeasurementTable.ResistanceCount ||
            channelIndex < 0 ||
            channelIndex >= ResistanceRows.Count)
        {
            return;
        }

        var referencePhase = _phaseReferences[resistanceIndex].GetValue(channelIndex);
        var measuredPhase = corrected.GetValue(channelIndex);
        if (!float.IsFinite(referencePhase) || !float.IsFinite(measuredPhase))
        {
            ResistanceRows[channelIndex].SetResistanceCell(resistanceIndex, MeasurementCell.Empty);
            return;
        }

        var referenceValue = _resistivityReferences.Count > resistanceIndex
            ? _resistivityReferences[resistanceIndex].GetValue(channelIndex)
            : 0f;
        var measuredValue = resistivity.GetValue(channelIndex);
        var percent = CalculatePhasePercentSample(measuredPhase, referencePhase);
        var averaged = PushResistancePercent(resistanceIndex - 1, channelIndex, percent);
        var bufferCount = _resistancePercentBuffers[resistanceIndex - 1, channelIndex]?.Count ?? 0;

        AppLogger.Write(
            $"R{resistanceIndex} channel={channelIndex}: phase_ref_mG={referencePhase * PhysicalConstants.Milligrad:F1}, " +
            $"phase_measured_mG={measuredPhase * PhysicalConstants.Milligrad:F1}, phase_percent={percent:F1}, average={averaged:F1}, " +
            $"ref_ues={referenceValue:F2}, measured_ues={measuredValue:F2}");
        AppLogger.Write(
            $"AUTO_CELL_WRITE: R={resistanceIndex}, C={capacitanceIndex}, channel={channelIndex}, auto_frame={frameIndex}, " +
            $"buffer_count={bufferCount}, averaging_limit={Math.Clamp(SelectedAveraging, 1, 20)}, " +
            $"value_percent={percent:F1}, averaged_percent={averaged:F1}, " +
            $"cell={FormatChannelRuntimeDiagnostic(resistivity, corrected, resistanceIndex, channelIndex)}");
        AppLogger.WriteDiagnostic(
            "RING_CELL",
            "SAMPLE_WRITTEN",
            $"workflow={CurrentRingWorkflowId}|run={_activeMeasurementRunId}|R={resistanceIndex}|C={capacitanceIndex}|channel={channelIndex}|point_frame={frameIndex}|buffer_count={bufferCount}|averaging_limit={Math.Clamp(SelectedAveraging, 1, 20)}|phase_reference_mG={referencePhase * PhysicalConstants.Milligrad:F3}|phase_measured_mG={measuredPhase * PhysicalConstants.Milligrad:F3}|sample_percent={percent:F3}|averaged_percent={averaged:F3}|reference_resistivity={referenceValue:F3}|measured_resistivity={measuredValue:F3}|quality={MeasurementCell.Create(averaged, ThresholdPercent).Quality}");
        ResistanceRows[channelIndex].SetResistanceCell(
            resistanceIndex,
            MeasurementCell.Create(averaged, ThresholdPercent));
    }

    // Возвращает дискретный образец PH_percent_from_sonde, как в старом WorkForm.h.
    private static float CalculatePhasePercentSample(float measuredPhase, float referencePhase)
    {
        var referenceMg = referencePhase * (float)PhysicalConstants.Milligrad;
        if (MathF.Abs(referenceMg) < 0.001f)
        {
            return (int)(MathF.Abs(measuredPhase * (float)PhysicalConstants.Milligrad) / 6.0f);
        }

        return (int)(100f * (measuredPhase - referencePhase) / referencePhase);
    }

    private float PushResistancePercent(int resistanceIndex, int channelIndex, float value)
    {
        var buffer = _resistancePercentBuffers[resistanceIndex, channelIndex];
        if (buffer is null)
        {
            buffer = new Queue<float>();
            _resistancePercentBuffers[resistanceIndex, channelIndex] = buffer;
        }

        buffer.Enqueue(value);
        var limit = Math.Clamp(SelectedAveraging, 1, 20);
        while (buffer.Count > limit)
        {
            buffer.Dequeue();
        }

        return buffer.Average();
    }

    private void ClearResistanceBuffers()
    {
        for (var r = 0; r < MeasurementTable.ResistanceCount; r++)
        {
            for (var c = 0; c < V10.ChannelCount; c++)
            {
                _resistancePercentBuffers[r, c]?.Clear();
            }
        }
    }

    private void ClearResistanceBuffer(int resistanceIndex)
    {
        if (resistanceIndex < 1 || resistanceIndex > MeasurementTable.ResistanceCount)
        {
            return;
        }

        for (var c = 0; c < V10.ChannelCount; c++)
        {
            _resistancePercentBuffers[resistanceIndex - 1, c]?.Clear();
        }
    }

    private void ClearResistanceCells()
    {
        foreach (var row in ResistanceRows)
        {
            for (var resistanceIndex = 1; resistanceIndex <= MeasurementTable.ResistanceCount; resistanceIndex++)
            {
                row.SetResistanceCell(resistanceIndex, MeasurementCell.Empty);
            }
        }

        ClearReportResistanceSelections();
    }

    private void ClearResistanceCells(int resistanceIndex)
    {
        if (resistanceIndex < 1 || resistanceIndex > MeasurementTable.ResistanceCount)
        {
            return;
        }

        foreach (var row in ResistanceRows)
        {
            row.SetResistanceCell(resistanceIndex, MeasurementCell.Empty);
        }

        SetReportResistanceSelected(resistanceIndex, false);
    }

    private void SetActiveMeasurementPoint(
        int resistanceIndex,
        int capacitanceIndex,
        string? diagnosticPointId = null)
    {
        lock (_measurementLock)
        {
            if (_activeResistanceIndex > 0 &&
                _activeCapacitanceIndex > 0 &&
                !_activeFirmwarePointSummaryLogged &&
                _activeFirmwarePointMonitors.Count > 0)
            {
                LogActiveFirmwarePointSummaryNoLock("interrupted_or_replaced");
            }

            _activeResistanceIndex = resistanceIndex;
            _activeCapacitanceIndex = capacitanceIndex;
            _activeMeasurementFrameCount = 0;
            _activeMeasurementPointId = resistanceIndex > 0 && capacitanceIndex > 0
                ? diagnosticPointId ?? "unassigned"
                : "none";
            _activeMeasurementPointStartedTimestamp = resistanceIndex > 0 && capacitanceIndex > 0
                ? Stopwatch.GetTimestamp()
                : 0;
            _activeFirmwarePointMonitors.Clear();
            _activeFirmwarePointSummaryLogged = resistanceIndex <= 0 || capacitanceIndex <= 0;
            AppLogger.Write($"ACTIVE_POINT: R={resistanceIndex}, C={capacitanceIndex}, frameCounter сброшен.");
            AppLogger.WriteDiagnostic(
                "RING_MEASUREMENT",
                "ACTIVE_POINT_CHANGED",
                $"workflow={CurrentRingWorkflowId}|run={_activeMeasurementRunId}|point={_activeMeasurementPointId}|R={resistanceIndex}|C={capacitanceIndex}|frame_counter=0");
        }
    }

    private (int ResistanceIndex, int CapacitanceIndex, int FrameIndex) AdvanceActiveMeasurementFrame()
    {
        lock (_measurementLock)
        {
            if (_activeResistanceIndex <= 0 || _activeCapacitanceIndex <= 0)
            {
                return (0, 0, 0);
            }

            _activeMeasurementFrameCount++;
            return (_activeResistanceIndex, _activeCapacitanceIndex, _activeMeasurementFrameCount);
        }
    }

    private void ObserveActiveFirmwarePoint(
        V10 corrected,
        int resistanceIndex,
        int capacitanceIndex,
        int frameIndex,
        int channelCount)
    {
        var activeChannels = GetActiveMeasurementChannels(resistanceIndex, capacitanceIndex, channelCount).ToArray();
        if (activeChannels.Length == 0 || _phaseReferences.Count <= resistanceIndex)
        {
            return;
        }

        string pointId;
        double elapsedMilliseconds;
        var channelDiagnostics = new List<string>(activeChannels.Length);
        lock (_measurementLock)
        {
            if (_activeResistanceIndex != resistanceIndex ||
                _activeCapacitanceIndex != capacitanceIndex ||
                _activeMeasurementPointStartedTimestamp == 0)
            {
                return;
            }

            pointId = _activeMeasurementPointId;
            elapsedMilliseconds = Stopwatch.GetElapsedTime(_activeMeasurementPointStartedTimestamp).TotalMilliseconds;
            foreach (var channel in activeChannels)
            {
                var measuredPhaseMg = corrected.GetValue(channel) * PhysicalConstants.Milligrad;
                var referencePhaseMg = _phaseReferences[resistanceIndex].GetValue(channel) * PhysicalConstants.Milligrad;
                if (!_activeFirmwarePointMonitors.TryGetValue(channel, out var monitor))
                {
                    monitor = new RingFirmwarePointMonitor();
                    _activeFirmwarePointMonitors[channel] = monitor;
                }

                monitor.Observe(
                    frameIndex,
                    elapsedMilliseconds,
                    measuredPhaseMg,
                    referencePhaseMg,
                    frameIndex > MeasurementSkipFrames);
                channelDiagnostics.Add(
                    $"ch{channel}:measured_mG={measuredPhaseMg:F3},reference_mG={referencePhaseMg:F3}");
            }
        }

        AppLogger.WriteDiagnostic(
            "RING_FIRMWARE_DIAG",
            "FRAME_OBSERVED",
            $"workflow={CurrentRingWorkflowId}|run={_activeMeasurementRunId}|point={pointId}|R={resistanceIndex}|C={capacitanceIndex}|point_frame={frameIndex}|elapsed_since_ack_ms={elapsedMilliseconds:F3}|stage={(frameIndex <= MeasurementSkipFrames ? "settling" : "averaging")}|channels=[{string.Join(";", channelDiagnostics)}]");
    }

    private void LogActiveFirmwarePointSummaryNoLock(string completionStatus)
    {
        if (_activeFirmwarePointSummaryLogged)
        {
            return;
        }

        _activeFirmwarePointSummaryLogged = true;
        var pointId = _activeMeasurementPointId;
        var resistanceIndex = _activeResistanceIndex;
        var capacitanceIndex = _activeCapacitanceIndex;
        var summaries = _activeFirmwarePointMonitors
            .OrderBy(pair => pair.Key)
            .Select(pair => (Channel: pair.Key, Summary: pair.Value.BuildSummary()))
            .ToArray();

        foreach (var item in summaries)
        {
            var summary = item.Summary;
            AppLogger.WriteDiagnostic(
                "RING_FIRMWARE_DIAG",
                "CHANNEL_SUMMARY",
                $"workflow={CurrentRingWorkflowId}|run={_activeMeasurementRunId}|point={pointId}|R={resistanceIndex}|C={capacitanceIndex}|channel={item.Channel}|completion={completionStatus}|state={FormatFirmwareRetentionState(summary.State)}|observations={summary.ObservationCount}|averaging_observations={summary.AveragingObservationCount}|response_threshold_mG={summary.ResponseThresholdMg:F3}|first_response_frame={summary.FirstResponseFrame}|first_response_elapsed_ms={summary.FirstResponseElapsedMilliseconds:F3}|peak_frame={summary.PeakFrame}|peak_elapsed_ms={summary.PeakElapsedMilliseconds:F3}|peak_phase_mG={summary.PeakPhaseMg:F3}|late_mean_abs_phase_mG={summary.LateMeanAbsPhaseMg:F3}|last_phase_mG={summary.LastPhaseMg:F3}|retention_ratio={summary.RetentionRatio:F6}|reference_phase_mG={summary.ReferencePhaseMg:F3}");
        }

        var pointState = summaries.Length == 0
            ? "inconclusive"
            : summaries.Any(item => item.Summary.State == RingFirmwareRetentionState.Dropped)
                ? "selection_dropped"
                : summaries.Any(item => item.Summary.State == RingFirmwareRetentionState.NoResponse)
                    ? "no_physical_response"
                    : summaries.All(item => item.Summary.State == RingFirmwareRetentionState.Held)
                        ? "selection_held"
                        : "inconclusive";
        var hypothesis = pointState switch
        {
            "selection_dropped" => "command_acknowledged_but_physical_response_not_retained_possible_firmware_timeout_or_relay_reset",
            "no_physical_response" => "command_acknowledged_without_detectable_phase_response_possible_relay_or_mapping_failure",
            "selection_held" => "physical_response_retained_through_averaging_window",
            _ => "insufficient_or_unstable_evidence"
        };
        AppLogger.WriteDiagnostic(
            "RING_FIRMWARE_DIAG",
            "POINT_CLASSIFICATION",
            $"workflow={CurrentRingWorkflowId}|run={_activeMeasurementRunId}|point={pointId}|R={resistanceIndex}|C={capacitanceIndex}|completion={completionStatus}|state={pointState}|channels={summaries.Length}|hypothesis={hypothesis}|command_ack=true|state_readback_supported=false|evidence=corrected_tool_phase");
    }

    private static string FormatFirmwareRetentionState(RingFirmwareRetentionState state) => state switch
    {
        RingFirmwareRetentionState.Held => "held",
        RingFirmwareRetentionState.Dropped => "dropped",
        RingFirmwareRetentionState.NoResponse => "no_response",
        _ => "inconclusive"
    };

    private void NotifyReportBlocked(string message)
    {
        AppLogger.WriteDiagnostic(
            "RING_REPORT",
            "BLOCKED",
            $"workflow={CurrentRingWorkflowId}|measurement_run={_activeMeasurementRunId}|reason={SanitizeDiagnosticValue(message)}");
        StatusText = "Рапорт не сформирован";
        RaiseUserMessage("Рапорт не сформирован", message);
    }

    private IReadOnlyList<int> GetFilledReportResistances()
    {
        var filled = new List<int>(MeasurementTable.ResistanceCount);
        for (var resistanceIndex = 1; resistanceIndex <= MeasurementTable.ResistanceCount; resistanceIndex++)
        {
            if (HasAnyReportData(resistanceIndex))
            {
                filled.Add(resistanceIndex);
            }
        }

        return filled;
    }

    private bool HasAnyReportData(int resistanceIndex)
    {
        foreach (var row in ResistanceRows)
        {
            if (!string.IsNullOrWhiteSpace(row.GetResistanceText(resistanceIndex)))
            {
                return true;
            }
        }

        return false;
    }

    private static string FormatResistanceList(IEnumerable<int> resistanceIndexes) =>
        string.Join(", ", resistanceIndexes.Select(resistanceIndex => $"R{resistanceIndex}"));

    private IReadOnlyList<int> GetSelectedReportResistances()
    {
        var selected = new List<int>(MeasurementTable.ResistanceCount);
        for (var resistanceIndex = 1; resistanceIndex <= MeasurementTable.ResistanceCount; resistanceIndex++)
        {
            if (IsReportResistanceSelected(resistanceIndex))
            {
                selected.Add(resistanceIndex);
            }
        }

        return selected;
    }

    private bool IsReportResistanceSelected(int resistanceIndex) =>
        resistanceIndex switch
        {
            1 => ReportR1Selected,
            2 => ReportR2Selected,
            3 => ReportR3Selected,
            4 => ReportR4Selected,
            5 => ReportR5Selected,
            6 => ReportR6Selected,
            7 => ReportR7Selected,
            8 => ReportR8Selected,
            _ => false
        };

    private void SetReportResistanceSelected(int resistanceIndex, bool value)
    {
        switch (resistanceIndex)
        {
            case 1:
                ReportR1Selected = value;
                break;
            case 2:
                ReportR2Selected = value;
                break;
            case 3:
                ReportR3Selected = value;
                break;
            case 4:
                ReportR4Selected = value;
                break;
            case 5:
                ReportR5Selected = value;
                break;
            case 6:
                ReportR6Selected = value;
                break;
            case 7:
                ReportR7Selected = value;
                break;
            case 8:
                ReportR8Selected = value;
                break;
        }
    }

    private void ClearReportResistanceSelections()
    {
        for (var resistanceIndex = 1; resistanceIndex <= MeasurementTable.ResistanceCount; resistanceIndex++)
        {
            SetReportResistanceSelected(resistanceIndex, false);
        }
    }

    private void RefreshReportSelectionForResistance(int resistanceIndex)
    {
        var count = Math.Min(ResistanceRows.Count, _channelCount);
        if (count <= 0)
        {
            SetReportResistanceSelected(resistanceIndex, false);
            return;
        }

        var isGood = true;
        for (var channelIndex = 0; channelIndex < count; channelIndex++)
        {
            var row = ResistanceRows[channelIndex];
            if (string.IsNullOrWhiteSpace(row.GetResistanceText(resistanceIndex)))
            {
                isGood = false;
                break;
            }

            var quality = row.GetResistanceQuality(resistanceIndex);
            if (quality is not (MeasurementQuality.InTolerance or MeasurementQuality.Warning))
            {
                isGood = false;
                break;
            }
        }

        SetReportResistanceSelected(resistanceIndex, isGood);
        AppLogger.Write($"REPORT_SELECT: R={resistanceIndex}, selected={isGood}");
    }

    private IReadOnlyList<LoopReportChannel> BuildReportChannels(IReadOnlyList<int> selectedResistances)
    {
        var channels = new List<LoopReportChannel>(ResistanceRows.Count);
        for (var channelIndex = 0; channelIndex < ResistanceRows.Count; channelIndex++)
        {
            var row = ResistanceRows[channelIndex];
            var cells = new List<LoopReportCell>(selectedResistances.Count);
            foreach (var resistanceIndex in selectedResistances)
            {
                cells.Add(new LoopReportCell(
                    row.GetResistanceText(resistanceIndex),
                    row.GetResistanceQuality(resistanceIndex)));
            }

            var frequencyKHz = channelIndex < _sondeParameters.Count
                ? _sondeParameters[channelIndex].Frequency / 1000f
                : 0f;
            channels.Add(new LoopReportChannel(row.SondeLabel, frequencyKHz, cells));
        }

        return channels;
    }

    private static string FormatResistivity(float value)
    {
        if (!float.IsFinite(value) || value <= 0f)
        {
            return string.Empty;
        }

        return value < 10f
            ? value.ToString("F2")
            : value.ToString("F0");
    }

    private static string FormatDiagnosticResistivity(float value)
    {
        if (!float.IsFinite(value))
        {
            return "NaN";
        }

        if (value <= 0f)
        {
            return value.ToString("F2", CultureInfo.InvariantCulture);
        }

        return value < 10f
            ? value.ToString("F2", CultureInfo.InvariantCulture)
            : value.ToString("F0", CultureInfo.InvariantCulture);
    }

    private static bool TryParseDouble(string? text, out double value)
    {
        if (string.IsNullOrWhiteSpace(text))
        {
            value = 0.0;
            return false;
        }

        return double.TryParse(text.Trim(), NumberStyles.Float, CultureInfo.InvariantCulture, out value) ||
            double.TryParse(text.Trim(), NumberStyles.Float, CultureInfo.CurrentCulture, out value);
    }

    private static ushort ToUInt16Scaled(double value, double scale, string fieldName)
    {
        var scaled = Math.Round(value * scale, MidpointRounding.AwayFromZero);
        if (scaled < ushort.MinValue || scaled > ushort.MaxValue)
        {
            throw new FormatException($"{fieldName}: значение выходит за диапазон UInt16");
        }

        return (ushort)scaled;
    }

    private (string FrequencyLabel, int TxIndex) GetChannelFrequencyAndTx(int channelIndex)
    {
        var transmitterCount = Math.Max(1, _channelCount / MetrologyConverter.FrequencyCount);
        var halfCount = transmitterCount;
        var freqLabel = channelIndex < halfCount ? "400kHz" : "2000kHz";
        var txIndex = channelIndex % halfCount;
        if (txIndex >= transmitterCount)
        {
            txIndex = transmitterCount - 1;
        }

        return (freqLabel, txIndex);
    }

    private void LogCapMap(int resistanceIndex, int capacitanceIndex, int commandCapacitance)
    {
        var channelIndex = capacitanceIndex - 1;
        var (freqLabel, txIndex) = GetChannelFrequencyAndTx(channelIndex);
        var referencePhase = _phaseReferences.Count > resistanceIndex
            ? _phaseReferences[resistanceIndex].GetValue(channelIndex)
            : 0f;
        var referenceResistivity = _resistivityReferences.Count > resistanceIndex
            ? _resistivityReferences[resistanceIndex].GetValue(channelIndex)
            : 0f;
        var reactance = channelIndex >= 0 && channelIndex < _loopParameters.Count
            ? _loopParameters[channelIndex].Reactance
            : 0f;

        AppLogger.Write(
            $"CAP_MAP: R={resistanceIndex}, logical_C={capacitanceIndex}, command_C={commandCapacitance}, " +
            $"channel={channelIndex}, {freqLabel} Tx{txIndex}, " +
            $"Re={reactance:F4}, reference_phase={referencePhase * PhysicalConstants.Milligrad:F1}мГ, reference_UES={referenceResistivity:F2}Ohm*m");
        AppLogger.Write(
            $"CAP_MAP_DIAG: R={resistanceIndex}, logical_C={capacitanceIndex}, command_RES={resistanceIndex}, command_CAP={commandCapacitance}, " +
            $"expected_channel={channelIndex}, expected={FormatChannelReferenceDiagnostic(resistanceIndex, channelIndex)}, " +
            $"all_reference_phase_mG=[{FormatAllReferencePhases(resistanceIndex)}]");
        AppLogger.WriteDiagnostic(
            "RING_MEASUREMENT",
            "CAP_MAPPING",
            $"workflow={CurrentRingWorkflowId}|run={_activeMeasurementRunId}|R={resistanceIndex}|logical_C={capacitanceIndex}|command_C={commandCapacitance}|expected_physical_C={commandCapacitance}|mapping_owner=application|shift_applied={commandCapacitance != capacitanceIndex}|channel={channelIndex}|frequency={freqLabel}|tx={txIndex}|reactance={reactance:F6}|reference_phase_mG={referencePhase * PhysicalConstants.Milligrad:F3}|reference_resistivity={referenceResistivity:F3}|all_reference_phase_mG=[{FormatAllReferencePhases(resistanceIndex)}]");
    }

    private string BeginMeasurementDiagnosticRun(string mode, int? resistanceIndex)
    {
        var runId = AppLogger.NextOperationId("ring-measurement");
        var run = BuildDiagnosticCorrelation(runId);
        _activeMeasurementRunId = run;
        _activeMeasurementRunStartedTimestamp = Stopwatch.GetTimestamp();
        _diagnosticCompletedPoints = 0;
        _diagnosticFailedPoints = 0;

        V10 air;
        lock (_airLock)
        {
            air = _localAir.Air;
        }

        AppLogger.WriteDiagnostic(
            "RING_MEASUREMENT",
            "RUN_BEGIN",
            $"workflow={CurrentRingWorkflowId}|run={run}|mode={mode}|requested_R={resistanceIndex?.ToString(CultureInfo.InvariantCulture) ?? "all"}|tool_signature=0x{_metrology.Signature:X8}|tool_serial={_metrology.Serial}|tx={_channelCount / MetrologyConverter.FrequencyCount}|channels={_channelCount}|rx_position={_rxPosition}|ring_profile={_ringDeviceInfo.ProtocolVersion}|ring_signature={_ringDeviceInfo.Signature?.ToString(CultureInfo.InvariantCulture) ?? "none"}|ring_memory_cells={_ringDeviceInfo.ImpedanceCellCount}|cap_mapping_owner=application|ring_physical_group_size={RingMeasurementSequence.RingPhysicalGroupSize}|skip_frames={MeasurementSkipFrames}|averaging={SelectedAveraging}|threshold_percent={ThresholdPercent}|express={ExpressMode}|express_full_scan_R_max={RingMeasurementSequence.ExpressFullScanResistanceLimit}|express_group_size={GetExpressGroupSize(_channelCount)}|poll_interval_ms={PollIntervalMs:F0}|local_air_mG=[{FormatV10Diagnostic(air, _channelCount, (float)PhysicalConstants.Milligrad, "F3")}]");
        return run;
    }

    private void EndMeasurementDiagnosticRun(string run, string status)
    {
        var elapsed = _activeMeasurementRunStartedTimestamp == 0
            ? TimeSpan.Zero
            : Stopwatch.GetElapsedTime(_activeMeasurementRunStartedTimestamp);
        var selected = GetSelectedReportResistances();
        AppLogger.WriteDiagnostic(
            "RING_MEASUREMENT",
            "RUN_COMPLETED",
            $"workflow={CurrentRingWorkflowId}|run={run}|status={status}|completed_points={_diagnosticCompletedPoints}|failed_points={_diagnosticFailedPoints}|selected_report_resistances=[{string.Join(",", selected)}]|elapsed_ms={elapsed.TotalMilliseconds:F3}");

        for (var resistanceIndex = 1; resistanceIndex <= MeasurementTable.ResistanceCount; resistanceIndex++)
        {
            if (!HasAnyReportData(resistanceIndex))
            {
                continue;
            }

            AppLogger.WriteDiagnostic(
                "RING_MEASUREMENT",
                "RUN_RESULT_COLUMN",
                $"workflow={CurrentRingWorkflowId}|run={run}|R={resistanceIndex}|selected_for_report={IsReportResistanceSelected(resistanceIndex)}|cells=[{FormatResistanceColumnDiagnostic(resistanceIndex)}]");
        }

        _activeMeasurementRunId = "none";
        _activeMeasurementRunStartedTimestamp = 0;
    }

    private (int ResistanceIndex, int CapacitanceIndex, int FrameIndex, string PointId) GetActiveMeasurementSnapshot()
    {
        lock (_measurementLock)
        {
            return (
                _activeResistanceIndex,
                _activeCapacitanceIndex,
                _activeMeasurementFrameCount,
                _activeMeasurementPointId);
        }
    }

    private string FormatResistanceColumnDiagnostic(int resistanceIndex)
    {
        return string.Join(",", ResistanceRows.Take(Math.Min(ResistanceRows.Count, _channelCount)).Select(row =>
            $"{SanitizeDiagnosticValue(row.SondeLabel)}:{SanitizeDiagnosticValue(row.GetResistanceText(resistanceIndex))}:{row.GetResistanceQuality(resistanceIndex)}"));
    }

    private string FormatResistanceCellsDiagnostic(int resistanceIndex, IReadOnlyList<int> channelIndexes)
    {
        return string.Join(",", channelIndexes.Where(index => index >= 0 && index < ResistanceRows.Count).Select(index =>
        {
            var row = ResistanceRows[index];
            return $"ch{index}:{SanitizeDiagnosticValue(row.GetResistanceText(resistanceIndex))}:{row.GetResistanceQuality(resistanceIndex)}";
        }));
    }

    private static string FormatV10Diagnostic(V10 values, int channelCount, float scale, string format)
    {
        var count = Math.Min(channelCount, V10.ChannelCount);
        return string.Join(",", Enumerable.Range(0, count).Select(index =>
            $"{index}:{(values.GetValue(index) * scale).ToString(format, CultureInfo.InvariantCulture)}"));
    }

    private string BuildDiagnosticCorrelation(string operationId) =>
        $"{CurrentRingWorkflowId}:{operationId}";

    private string CurrentRingWorkflowId =>
        string.IsNullOrWhiteSpace(_ringWorkflowId) ? "none" : _ringWorkflowId;

    private static string SanitizeDiagnosticValue(string value) =>
        value
            .Replace("\r", "\\r", StringComparison.Ordinal)
            .Replace("\n", "\\n", StringComparison.Ordinal)
            .Replace("|", "/", StringComparison.Ordinal)
            .Replace(",", ";", StringComparison.Ordinal);

    private string FormatChannelReferenceDiagnostic(int resistanceIndex, int channelIndex)
    {
        if (channelIndex < 0 || channelIndex >= Math.Min(_channelCount, V10.ChannelCount))
        {
            return "none";
        }

        var (freqLabel, txIndex) = GetChannelFrequencyAndTx(channelIndex);
        var referencePhase = _phaseReferences.Count > resistanceIndex
            ? _phaseReferences[resistanceIndex].GetValue(channelIndex)
            : 0f;
        var referenceResistivity = _resistivityReferences.Count > resistanceIndex
            ? _resistivityReferences[resistanceIndex].GetValue(channelIndex)
            : 0f;
        var reactance = channelIndex >= 0 && channelIndex < _loopParameters.Count
            ? _loopParameters[channelIndex].Reactance
            : 0f;

        return
            $"ch{channelIndex}:{freqLabel}Tx{txIndex},Re={reactance:F4}," +
            $"ref_phase_mG={referencePhase * PhysicalConstants.Milligrad:F1},ref_ues={FormatDiagnosticResistivity(referenceResistivity)}";
    }

    private string FormatAllReferencePhases(int resistanceIndex)
    {
        var count = Math.Min(_channelCount, V10.ChannelCount);
        if (count <= 0 || _phaseReferences.Count <= resistanceIndex)
        {
            return string.Empty;
        }

        var reference = _phaseReferences[resistanceIndex];
        return string.Join(", ", Enumerable.Range(0, count).Select(channel =>
            $"ch{channel}={reference.GetValue(channel) * PhysicalConstants.Milligrad:F1}"));
    }

    private void DispatchStatus(string message)
    {
        App.DispatcherQueue.TryEnqueue(() => StatusText = message);
    }

    // Маршалирует запрос на показ сообщения в UI-поток и дублирует его в журнал для диагностики.
    private void RaiseUserMessage(string title, string message)
    {
        AppLogger.Write($"USER_MESSAGE: {title} — {message}");
        var handler = NotificationRequested;
        if (handler is null)
        {
            return;
        }

        App.DispatcherQueue.TryEnqueue(() => handler(new UserNotification(title, message)));
    }

    private readonly record struct ToolPortOperationResult<T>(bool Success, T Value, string? ErrorMessage)
    {
        public static ToolPortOperationResult<T> Ok(T value) => new(true, value, null);

        public static ToolPortOperationResult<T> Fail(string errorMessage) => new(false, default!, errorMessage);
    }
}

// Модальное уведомление для пользователя: заголовок и текст причины.
public readonly record struct UserNotification(string Title, string Message);
