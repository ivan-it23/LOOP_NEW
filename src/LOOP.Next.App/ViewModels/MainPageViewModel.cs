using System.Collections.ObjectModel;
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
using Microsoft.UI.Xaml.Media;

namespace LOOP_Next_App.ViewModels;

// ViewModel координирует UI главной страницы, аппаратный слой и расчётный конвейер измерений.
public partial class MainPageViewModel : ObservableObject
{
    private static readonly SolidColorBrush AirColumnDefaultBrush = new(Colors.White);
    private static readonly SolidColorBrush LocalAirSetBrush = new(Colors.Red);
    private static readonly SolidColorBrush AirColumnDefaultForegroundBrush = new(Colors.Black);
    private static readonly SolidColorBrush AirColumnSetForegroundBrush = new(Colors.White);

    private const int MeasurementSkipFrames = 5;

    private readonly SerialPort _toolPort = new();
    private readonly SerialPort _ringPort = new();
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
    private int _channelCount;
    private readonly object _autoLock = new();
    private TaskCompletionSource<bool>? _autoMeasurementCompleteTcs;
    private uint _rxPosition;
    private GpToolMetrology _metrology;
    private int _frameCounter;

    public MainPageViewModel()
    {
        _userSettings = UserSettingsStore.Load();
        BuildDefaultResistanceTable();
        BuildDefaultRingCalibrationRows();
        RefreshPorts();
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
    public partial string SignatureText { get; set; } = "—";

    [ObservableProperty]
    public partial string SerialText { get; set; } = "—";

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
            DisconnectRing();
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
            StatusText = result.ErrorMessage ?? "Не удалось открыть порт прибора";
            IsConnected = false;
            return;
        }

        _sondeParameters = result.SondeParameters;
        _channelCount = _sondeParameters.Count;
        _rxPosition = result.Metrology.RxPosition;
        _metrology = result.Metrology;

        ApplyIdentification(in _metrology);
        BuildChannels(_sondeParameters, in _metrology);
        BuildRingCalibrationRows(_sondeParameters);
        CalculateRingCalibration();
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
        SaveSelectedRingPort(portName);
        StatusText = "Подключение кольца...";

        var status = await Task.Run(() =>
        {
            _ringPort.PortName = portName;
            return RingConnection.Connect(_ringPort);
        });

        if (status == RingConnection.RingConnectStatus.Connected)
        {
            IsRingConnected = true;
            StatusText = "Кольцо подключено";
            if (IsConnected)
            {
                await LoadRingParametersAsync();
            }
        }
        else
        {
            IsRingConnected = false;
            StatusText = status switch
            {
                RingConnection.RingConnectStatus.PortOpenFailed => "Не удается открыть порт кольца!",
                RingConnection.RingConnectStatus.NoResponse => "Нет ответа от кольца !",
                RingConnection.RingConnectStatus.Rejected => "Не удается установить соединение !",
                _ => $"Кольцо: {status}"
            };
        }
    }

    [RelayCommand]
    private void DisconnectRing()
    {
        RingConnection.Disconnect(_ringPort);
        IsRingConnected = false;
        SetActiveMeasurementPoint(0, 0);
        StatusText = "Кольцо отключено";
    }

    public string CreateSuggestedReportFileName() =>
        $"LOOP_report_{DateTime.Now:yyyyMMdd_HHmmss}";

    public bool TryBuildReportRtf(out string rtf)
    {
        rtf = string.Empty;

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
            StatusText = $"Рапорт сохранён: {path}";
            RaiseUserMessage("Рапорт сохранён", $"Файл рапорта успешно сохранён:\n{path}");
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Ошибка сохранения рапорта");
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

        try
        {
            AppLogger.Write($"=== РУЧНОЙ ЗАПУСК R{resistanceIndex} ===");
            await RunResistanceMeasurementAsync(resistanceIndex, $"R{resistanceIndex}", token);
            AppLogger.Write($"=== РУЧНОЙ ПРОХОД R{resistanceIndex} ЗАВЕРШЁН ===");
        }
        catch (OperationCanceledException)
        {
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
                await Task.Run(() => RingConnection.SetMeasurementPoint(_ringPort, 0, 0), CancellationToken.None);
            }

            IsResistanceMeasurementRunning = false;
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

    [RelayCommand]
    private async Task WriteMetrologyToToolAsync()
    {
        GpToolMetrology metrology;
        try
        {
            metrology = BuildMetrologyFromCurrentFields();
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Ошибка подготовки метрологии к записи в зонд");
            StatusText = $"ФАЙЛ МЕТРОЛОГИИ НЕ БЫЛ ЗАПИСАН В ПРИБОР! {ex.Message}";
            return;
        }

        StatusText = "Запись метрологии в зонд...";
        var result = await ExecuteToolPortOperationAsync(port => MetrologyPort.WriteToTool(port, in metrology));
        if (!result.Success || !result.Value)
        {
            StatusText = result.ErrorMessage is null
                ? "ФАЙЛ МЕТРОЛОГИИ НЕ БЫЛ ЗАПИСАН В ПРИБОР!"
                : $"ФАЙЛ МЕТРОЛОГИИ НЕ БЫЛ ЗАПИСАН В ПРИБОР! {result.ErrorMessage}";
            return;
        }

        ApplyMetrology(metrology);
        StatusText = "ФАЙЛ МЕТРОЛОГИИ ЗАПИСАН В ПРИБОР!";
    }

    public async Task ReadMetrologyFromFileAsync(string path)
    {
        try
        {
            var metrology = await Task.Run(() => MetrologyFile.Read(path));
            ApplyMetrology(metrology);
            StatusText = $"Файл метрологии считан: {path}";
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Ошибка чтения файла метрологии");
            StatusText = $"Не удалось открыть файл метрологии: {ex.Message}";
        }
    }

    public async Task WriteMetrologyToFileAsync(string path)
    {
        try
        {
            var metrology = BuildMetrologyFromCurrentFields();
            await Task.Run(() => MetrologyFile.Write(path, in metrology));
            ApplyMetrology(metrology);
            StatusText = $"Файл метрологии записан: {path}";
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Ошибка записи файла метрологии");
            StatusText = $"Не удалось записать файл метрологии: {ex.Message}";
        }
    }

    [RelayCommand]
    private void CalculateRingCalibration()
    {
        var parameters = GetRingCalculationParameters();
        if (parameters.Count == 0)
        {
            StatusText = "Нет метрологии зонда для расчёта кольца";
            return;
        }

        var count = Math.Min(RingCalibrationRows.Count, parameters.Count);
        for (var i = 0; i < count; i++)
        {
            var parameter = parameters[i];
            var value = RingCalibrationCalculator.Calculate(parameter);
            RingCalibrationRows[i].L1Text = parameter.L1.ToString("F3");
            RingCalibrationRows[i].L2Text = parameter.L2.ToString("F3");
            RingCalibrationRows[i].FrequencyText = (parameter.Frequency / 1000f).ToString("F0");
            RingCalibrationRows[i].CapacitanceText = (value.CapacitanceFarads * 1_000_000_000_000f).ToString("F0");
            RingCalibrationRows[i].ReactanceCalculatedText = value.Reactance.ToString("F3");
        }

        RecalculateLoopReferences();
        StatusText = "Расчёт ёмкостей кольца выполнен";
    }

    [RelayCommand]
    private async Task ReadRingParametersAsync()
    {
        if (!IsRingConnected || !_ringPort.IsOpen)
        {
            StatusText = "Подключите кольцо";
            return;
        }

        await LoadRingParametersAsync();
    }

    private async Task LoadRingParametersAsync()
    {
        if (!IsRingConnected || !_ringPort.IsOpen)
        {
            return;
        }

        StatusText = "Чтение параметров кольца...";
        var result = await Task.Run(() => RingConnection.ReadLoopParameters(_ringPort));

        var count = Math.Min(RingCalibrationRows.Count, result.Cells.Count);
        for (var i = 0; i < count; i++)
        {
            var cell = result.Cells[i];
            RingCalibrationRows[i].ResistanceText = cell.HasValue ? cell.Resistance.ToString("F0") : string.Empty;
            RingCalibrationRows[i].ReactanceMeasuredText = cell.HasValue ? cell.Reactance.ToString("F4") : string.Empty;
        }

        var requiredParameterCount = Math.Max(
            MeasurementTable.ResistanceCount,
            Math.Min(_channelCount, V10.ChannelCount));
        var hasRequiredParameters = result.Parameters.Count >= requiredParameterCount;
        _loopParameters = hasRequiredParameters
            ? result.Parameters
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
            }
            StatusText = $"Параметры кольца прочитаны: {_loopParameters.Count}";
        }
        else
        {
            AppLogger.Write("Параметры кольца не получены.");
            StatusText = "Параметры кольца не получены";
        }
    }

    [RelayCommand]
    private async Task WriteRingParametersAsync()
    {
        if (!IsRingConnected || !_ringPort.IsOpen)
        {
            StatusText = "Включите кольцо";
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
            return;
        }

        StatusText = "Запись параметров кольца...";
        var result = await Task.Run(() => RingConnection.WriteLoopParameters(_ringPort, cells));
        if (result.Success)
        {
            ApplyRingCells(cells);
            RebuildLoopParametersFromRingRows();
            RecalculateLoopReferences();
            ClearResistanceBuffers();
            ClearResistanceCells();
            StatusText = "Запись кольца прошла успешно";
        }
        else
        {
            var failed = string.Join(", ", result.Cells.Where(cell => !cell.Success).Select(cell => $"Z{cell.Index}"));
            StatusText = $"Ошибка записи кольца: {failed}";
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
    private void LocalAir()
    {
        if (!IsConnected)
        {
            StatusText = "Подключите прибор";
            return;
        }

        lock (_airLock)
        {
            if (!_localAir.IsSet)
            {
                if (IsRingConnected && _ringPort.IsOpen)
                {
                    SetActiveMeasurementPoint(0, 0);
                    _ = Task.Run(() => RingConnection.SetMeasurementPoint(_ringPort, 0, 0));
                }

                _localAir.Begin(SelectedFrameCount);
                RefreshLocalAirUi();
                StatusText = "Сбор локального воздуха";
            }
            else
            {
                _localAir.RequestReset();
                RefreshLocalAirUi();
                StatusText = "Сброс локального воздуха";
            }
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
        SignatureText = $"0x{identity.Signature:X8}";
        SerialText = identity.Serial.ToString();
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
        foreach (var header in table.Headers)
        {
            var parameter = sondeParameters[header.ChannelIndex];
            RingCalibrationRows.Add(new RingCalibrationRowViewModel(header.ChannelIndex, header.Label)
            {
                L1Text = parameter.L1.ToString("F3"),
                L2Text = parameter.L2.ToString("F3"),
                FrequencyText = (parameter.Frequency / 1000f).ToString("F0")
            });
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
            RingCalibrationRows.Add(new RingCalibrationRowViewModel(i, labels[i]));
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
        RingResistanceRows.Clear();

        foreach (var row in RingCalibrationRows.Take(8))
        {
            RingResistanceRows.Add(row);
        }
    }

    private void ApplyRingEditModes()
    {
        foreach (var row in RingCalibrationRows)
        {
            row.IsRingValueReadOnly = !IsRingEditMode;
        }
    }

    private IReadOnlyList<SondeParameter> GetRingCalculationParameters()
    {
        return _sondeParameters;
    }

    private GpToolMetrology BuildMetrologyFromCurrentFields()
    {
        var metrology = _metrology;
        var geometryCount = Math.Min(RingGeometryRows.Count, GpToolMetrology.MaxTransmitterCount);
        for (var tx = 0; tx < geometryCount; tx++)
        {
            var row = RingGeometryRows[tx];
            if (TryParseDouble(row.L1Text, out var l1))
            {
                metrology.SetL1(tx, ToUInt16Scaled(l1, 1000.0, $"{row.Label} L1"));
            }

            if (TryParseDouble(row.L2Text, out var l2))
            {
                metrology.SetL2(tx, ToUInt16Scaled(l2, 1000.0, $"{row.Label} L2"));
            }
        }

        var frequencyCount = Math.Min(RingFrequencyRows.Count, GpToolMetrology.FrequencyCount);
        for (var frequency = 0; frequency < frequencyCount; frequency++)
        {
            var row = RingFrequencyRows[frequency];
            if (TryParseDouble(row.FrequencyText, out var frequencyKHz))
            {
                metrology.SetFrequency(frequency, ToUInt16Scaled(frequencyKHz, 1.0, row.Label));
            }
        }

        return metrology;
    }

    private IReadOnlyList<RingLoopParameterCell> BuildRingWriteCells()
    {
        var cells = new List<RingLoopParameterCell>(RingProtocolConstants.LoopParameterCellCount);
        for (var index = 0; index < RingProtocolConstants.LoopParameterCellCount; index++)
        {
            var row = index < RingCalibrationRows.Count ? RingCalibrationRows[index] : null;
            var defaultResistance = index == 0 ? 1f : 0f;
            var resistance = defaultResistance;
            var reactance = 0f;

            if (row is not null && !string.IsNullOrWhiteSpace(row.ResistanceText))
            {
                if (!TryParseDouble(row.ResistanceText, out var parsedResistance))
                {
                    throw new FormatException($"Некорректное R в Z{index}");
                }

                resistance = (float)parsedResistance;
            }

            var reactanceText = row is null
                ? string.Empty
                : string.IsNullOrWhiteSpace(row.ReactanceMeasuredText)
                    ? row.ReactanceCalculatedText
                    : row.ReactanceMeasuredText;
            if (!string.IsNullOrWhiteSpace(reactanceText))
            {
                if (!TryParseDouble(reactanceText, out var parsedReactance))
                {
                    throw new FormatException($"Некорректное Re в Z{index}");
                }

                reactance = (float)parsedReactance;
            }

            cells.Add(new RingLoopParameterCell(index, resistance, reactance, true));
        }

        return cells;
    }

    private void ApplyRingCells(IReadOnlyList<RingLoopParameterCell> cells)
    {
        var count = Math.Min(RingCalibrationRows.Count, cells.Count);
        for (var i = 0; i < count; i++)
        {
            var cell = cells[i];
            RingCalibrationRows[i].ResistanceText = cell.Resistance.ToString("F0");
            RingCalibrationRows[i].ReactanceMeasuredText = cell.Reactance.ToString("F4");
        }
    }

    private void RebuildLoopParametersFromRingRows()
    {
        var parameters = new List<LoopParameter>();
        foreach (var row in RingCalibrationRows)
        {
            if (!TryParseDouble(row.ResistanceText, out var resistance) ||
                !TryParseDouble(row.ReactanceMeasuredText, out var reactance) ||
                reactance <= 0.000001)
            {
                continue;
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
        var sondeParameters = GetRingCalculationParameters();
        if (sondeParameters.Count == 0 || _loopParameters.Count == 0)
        {
            _phaseReferences = Array.Empty<V10>();
            _resistivityReferences = Array.Empty<V10>();
            return;
        }

        _phaseReferences = LoopResistivity.CalcPhaseFromLoop(sondeParameters, _loopParameters);
        _resistivityReferences = LoopResistivity.CalcRoFromLoop(sondeParameters, _phaseReferences);

        AppLogger.Write($"Эталонные фазы кольца: {_phaseReferences.Count} точек, эталонные УЭС: {_resistivityReferences.Count} точек.");
        for (var i = 0; i < _resistivityReferences.Count && i <= MeasurementTable.ResistanceCount; i++)
        {
            var reference = _resistivityReferences[i];
            var values = string.Join(", ", Enumerable.Range(0, Math.Min(_channelCount, V10.ChannelCount)).Select(c => $"[{c}]={reference.GetValue(c):F2}"));
            AppLogger.Write($"R{i}: эталонные УЭС={values}");
        }
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
        if (!IsConnected || !IsRingConnected || !_ringPort.IsOpen)
        {
            AppLogger.Write($"{modeName}: прибор или кольцо не подключены.");
            StatusText = $"{modeName}: подключите прибор и кольцо";
            return false;
        }

        if (!IsLocalAirSet())
        {
            const string message = "Сначала соберите локальный воздух, затем повторите заполнение таблицы.";
            AppLogger.Write($"{modeName}: запуск заблокирован, локальный воздух не задан.");
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
            StatusText = $"{modeName}: нет расчётных эталонов кольца";
            return false;
        }

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
        var tcs = new TaskCompletionSource<bool>(TaskCreationOptions.RunContinuationsAsynchronously);
        ClearResistanceCells(resistanceIndex);
        ClearResistanceBuffer(resistanceIndex);

        lock (_autoLock)
        {
            _autoMeasurementCompleteTcs = tcs;
            SetActiveMeasurementPoint(resistanceIndex, 1);
        }

        StatusText = $"{modeName}: R{resistanceIndex}, C1";
        LogCapMap(resistanceIndex, 1);
        AppLogger.Write($"{modeName}: задержка 200 мс перед установкой R{resistanceIndex}/C1 (как в R_Click).");
        await Task.Delay(200, token);

        AppLogger.Write($"{modeName}: установка R{resistanceIndex}/C1...");
        var outcome = await Task.Run(
            () => RingConnection.SetMeasurementPoint(_ringPort, resistanceIndex, 1),
            token);
        if (!outcome.IsOk)
        {
            var reason = outcome.FailureReason ?? $"Кольцо не ответило ({outcome.Result}).";
            AppLogger.Write($"{modeName}: ошибка установки R{resistanceIndex}/C1 ({outcome.Result}): {reason}");
            StatusText = $"{modeName} остановлен: {reason}";
            lock (_autoLock)
            {
                _autoMeasurementCompleteTcs = null;
                SetActiveMeasurementPoint(0, 0);
            }

            RaiseUserMessage(
                "Не удалось установить точку на кольце",
                $"{modeName} остановлен на точке R{resistanceIndex}.\n\nПричина: {reason}\n\n" +
                $"Проверьте питание и подключение кольца (порт {_ringPort.PortName}), затем переподключите кольцо и повторите.");
            return false;
        }

        AppLogger.Write($"{modeName}: R{resistanceIndex}/C1 установлен, ожидание усреднения кадров...");
        using (token.Register(() => tcs.TrySetCanceled(token)))
        {
            await tcs.Task;
        }

        RefreshReportSelectionForResistance(resistanceIndex);
        AppLogger.Write($"{modeName}: R{resistanceIndex} измерено.");
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
        }
        catch (OperationCanceledException)
        {
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
                await Task.Run(() => RingConnection.SetMeasurementPoint(_ringPort, 0, 0), CancellationToken.None);
            }

            if (IsAutoMode)
            {
                IsAutoMode = false;
            }

            IsResistanceMeasurementRunning = false;
        }
    }

    // Метод переводит автоцикл к следующей ёмкости или завершает измерение текущего R.
    private void AdvanceAutoMeasurement()
    {
        TaskCompletionSource<bool>? tcsToComplete = null;
        int nextR = 0;
        int nextC = 0;

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

                if (_activeMeasurementFrameCount < requiredFrames)
                {
                    return;
                }

                nextC = GetNextCapacitanceIndex(_activeResistanceIndex, _activeCapacitanceIndex);
                if (nextC > _channelCount)
                {
                    AppLogger.Write($"АВТО: измерение R={_activeResistanceIndex} завершено (пройдены C=1..{_channelCount}).");
                    tcsToComplete = _autoMeasurementCompleteTcs;
                    _autoMeasurementCompleteTcs = null;
                    SetActiveMeasurementPoint(0, 0);
                }
                else
                {
                    nextR = _activeResistanceIndex;
                    AppLogger.Write($"АВТО: переход к C={nextC} внутри R={nextR}.");
                    StatusText = $"Режим АВТО: R{nextR}, C{nextC}";
                    SetActiveMeasurementPoint(nextR, nextC);
                }
            }
        }

        if (tcsToComplete is not null)
        {
            tcsToComplete.TrySetResult(true);
            return;
        }

        if (nextR > 0 && nextC > 0)
        {
            LogCapMap(nextR, nextC);
            _ = Task.Run(() =>
            {
                AppLogger.Write($"AUTO_SWITCH_SEND: R={nextR}, C={nextC}, command_RES={nextR}, command_CAP={nextC}");
                var outcome = RingConnection.SetMeasurementPoint(_ringPort, nextR, nextC);
                AppLogger.Write($"AUTO_SWITCH_RESULT: R={nextR}, C={nextC}, result={outcome.Result}, reason={outcome.FailureReason}");
            });
        }
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
        LocalAirButtonText = isCollecting
            ? $"Сбор: {collectedSampleCount} кадров"
            : "Локальный воздух";

        var airBackground = isSet ? LocalAirSetBrush : AirColumnDefaultBrush;
        var airForeground = isSet ? AirColumnSetForegroundBrush : AirColumnDefaultForegroundBrush;
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
        if (ExpressMode)
        {
            return capacitanceIndex + GetExpressGroupSize(_channelCount);
        }

        return capacitanceIndex + 1;
    }

    private IEnumerable<int> GetActiveMeasurementChannels(int resistanceIndex, int capacitanceIndex, int channelCount)
    {
        var start = capacitanceIndex - 1;
        if (start < 0 || start >= channelCount)
        {
            yield break;
        }

        if (ExpressMode)
        {
            var groupSize = GetExpressGroupSize(channelCount);
            var end = Math.Min(channelCount, start + groupSize);
            for (var channel = start; channel < end; channel++)
            {
                yield return channel;
            }

            yield break;
        }

        yield return start;
    }

    private static int GetExpressGroupSize(int channelCount)
    {
        return Math.Max(1, channelCount / 2);
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

    private void SetActiveMeasurementPoint(int resistanceIndex, int capacitanceIndex)
    {
        lock (_measurementLock)
        {
            _activeResistanceIndex = resistanceIndex;
            _activeCapacitanceIndex = capacitanceIndex;
            _activeMeasurementFrameCount = 0;
            AppLogger.Write($"ACTIVE_POINT: R={resistanceIndex}, C={capacitanceIndex}, frameCounter сброшен.");
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

    private void NotifyReportBlocked(string message)
    {
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

    private static (string FrequencyLabel, int TxIndex) GetChannelFrequencyAndTx(int channelIndex)
    {
        const int transmitterCount = 5;
        var halfCount = (V10.ChannelCount + 1) / 2;
        var freqLabel = channelIndex < halfCount ? "400kHz" : "2000kHz";
        var txIndex = channelIndex % halfCount;
        if (txIndex >= transmitterCount)
        {
            txIndex = transmitterCount - 1;
        }

        return (freqLabel, txIndex);
    }

    private void LogCapMap(int resistanceIndex, int capacitanceIndex)
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
            $"CAP_MAP: R={resistanceIndex}, C={capacitanceIndex}, channel={channelIndex}, {freqLabel} Tx{txIndex}, " +
            $"Re={reactance:F4}, reference_phase={referencePhase * PhysicalConstants.Milligrad:F1}мГ, reference_UES={referenceResistivity:F2}Ohm*m");
        AppLogger.Write(
            $"CAP_MAP_DIAG: R={resistanceIndex}, C={capacitanceIndex}, command_RES={resistanceIndex}, command_CAP={capacitanceIndex}, " +
            $"expected_channel={channelIndex}, expected={FormatChannelReferenceDiagnostic(resistanceIndex, channelIndex)}, " +
            $"all_reference_phase_mG=[{FormatAllReferencePhases(resistanceIndex)}]");
    }

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
