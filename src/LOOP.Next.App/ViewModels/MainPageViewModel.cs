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
using Microsoft.UI;
using Microsoft.UI.Xaml.Media;

namespace LOOP_Next_App.ViewModels;

// ViewModel координирует UI главной страницы, аппаратный слой и расчётный конвейер измерений.
public partial class MainPageViewModel : ObservableObject
{
    private static readonly SolidColorBrush DisconnectedBrush = new(Colors.Green);
    private static readonly SolidColorBrush ConnectedBrush = new(Colors.Red);

    private const int MeasurementSkipFrames = 5;
    private const int ExpressFullScanResistanceLimit = 2;

    private readonly SerialPort _toolPort = new();
    private readonly SerialPort _ringPort = new();
    private readonly LocalAirCorrector _localAir = new();
    private readonly object _airLock = new();
    private readonly object _measurementLock = new();
    private readonly SemaphoreSlim _toolIoLock = new(1, 1);

    private CancellationTokenSource? _pollCts;
    private IReadOnlyList<SondeParameter> _sondeParameters = Array.Empty<SondeParameter>();
    private IReadOnlyList<LoopParameter> _loopParameters = Array.Empty<LoopParameter>();
    private IReadOnlyList<V10> _phaseReferences = Array.Empty<V10>();
    private IReadOnlyList<V10> _resistivityReferences = Array.Empty<V10>();
    private readonly Queue<float>[,] _resistancePercentBuffers = new Queue<float>[MeasurementTable.ResistanceCount, V10.ChannelCount];
    private CancellationTokenSource? _autoCts;
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
        BuildDefaultResistanceTable();
        BuildDefaultRingCalibrationRows();
        RefreshPorts();
    }

    public ObservableCollection<string> ToolPorts { get; } = new();

    public ObservableCollection<string> RingPorts { get; } = new();

    public ObservableCollection<ChannelReadingViewModel> Channels { get; } = new();

    public ObservableCollection<ResistanceTableRowViewModel> ResistanceRows { get; } = new();

    public ObservableCollection<RingCalibrationRowViewModel> RingCalibrationRows { get; } = new();

    public ObservableCollection<RingCalibrationRowViewModel> RingResistanceRows { get; } = new();

    public ObservableCollection<RingCalibrationRowViewModel> RingGeometryRows { get; } = new();

    public ObservableCollection<RingCalibrationRowViewModel> RingFrequencyRows { get; } = new();

    public ObservableCollection<int> FrameCountOptions { get; } = new() { 20, 30, 40, 50, 60 };

    public ObservableCollection<int> PollIntervalOptions { get; } = new() { 500, 1000, 1500, 2000 };

    public ObservableCollection<int> AveragingOptions { get; } = new() { 5, 10, 15, 20 };

    public ObservableCollection<int> ThresholdOptions { get; } = new() { 5, 6, 7, 8, 9, 10 };

    [ObservableProperty]
    [NotifyPropertyChangedFor(nameof(CanConnect))]
    [NotifyPropertyChangedFor(nameof(CanToggleTool))]
    public partial string? SelectedToolPort { get; set; }

    [ObservableProperty]
    [NotifyPropertyChangedFor(nameof(CanConnectRing))]
    [NotifyPropertyChangedFor(nameof(CanToggleRing))]
    public partial string? SelectedRingPort { get; set; }

    [ObservableProperty]
    public partial int SelectedFrameCount { get; set; } = LocalAirCorrector.DefaultSampleCount;

    // Усреднение и порог применяются при расчёте отклонений в таблице R1..R8.
    [ObservableProperty]
    public partial int SelectedAveraging { get; set; } = 5;

    [ObservableProperty]
    public partial int ThresholdPercent { get; set; } = 6;

    partial void OnThresholdPercentChanged(int value)
    {
        foreach (var row in ResistanceRows)
        {
            row.IndicatorThreshold = value;
        }
    }

    // Режимы измерений управляют автоциклом кольца и настройкой экспресс-проверки.
    [ObservableProperty]
    public partial bool IsAutoMode { get; set; }

    partial void OnIsAutoModeChanged(bool value)
    {
        if (value)
        {
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
    public partial int PollIntervalMs { get; set; } = 1000;

    [ObservableProperty]
    public partial bool IsRingEditMode { get; set; }

    partial void OnIsRingEditModeChanged(bool value) => ApplyRingEditModes();

    [ObservableProperty]
    [NotifyPropertyChangedFor(nameof(CanConnect))]
    [NotifyPropertyChangedFor(nameof(CanToggleTool))]
    [NotifyPropertyChangedFor(nameof(ToolToggleText))]
    public partial bool IsConnected { get; set; }

    [ObservableProperty]
    [NotifyPropertyChangedFor(nameof(CanConnectRing))]
    [NotifyPropertyChangedFor(nameof(CanToggleRing))]
    [NotifyPropertyChangedFor(nameof(RingToggleText))]
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
        ToolButtonBackground = value ? ConnectedBrush : DisconnectedBrush;
    }

    partial void OnIsRingConnectedChanged(bool value)
    {
        RingButtonBackground = value ? ConnectedBrush : DisconnectedBrush;
    }

    [ObservableProperty]
    public partial Brush ToolButtonBackground { get; set; } = DisconnectedBrush;

    [ObservableProperty]
    public partial Brush RingButtonBackground { get; set; } = DisconnectedBrush;

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
    public partial string ModificationText { get; set; } = "—";

    [ObservableProperty]
    public partial string NumberText { get; set; } = "—";

    [ObservableProperty]
    public partial string PaletteText { get; set; } = "—";

    [ObservableProperty]
    public partial string SignatureBitsText { get; set; } = "—";

    [ObservableProperty]
    public partial string LocalAirState { get; set; } = "Не задан";

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

        ToolPorts.Clear();
        RingPorts.Clear();
        foreach (var port in ports)
        {
            ToolPorts.Add(port);
            RingPorts.Add(port);
        }

        if (SelectedToolPort is null && ToolPorts.Count > 0)
        {
            SelectedToolPort = ToolPorts[0];
        }

        if (SelectedRingPort is null && RingPorts.Count > 0)
        {
            SelectedRingPort = RingPorts[0];
        }
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

    [RelayCommand]
    private void Report()
    {
        if (ResistanceRows.Count == 0)
        {
            StatusText = "Рапорт: нет данных";
            return;
        }

        var report = new LoopReport(
            SerialText,
            DateTime.Now,
            _loopParameters.Take(MeasurementTable.ResistanceCount).Select(parameter => parameter.Resistance).ToArray(),
            BuildReportChannels());
        var rtf = LoopReportRtfBuilder.Build(report);
        var directory = Path.Combine(
            Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments),
            "LOOP Next Reports");
        Directory.CreateDirectory(directory);

        var path = Path.Combine(directory, $"LOOP_report_{DateTime.Now:yyyyMMdd_HHmmss}.rtf");
        File.WriteAllText(path, rtf);
        StatusText = $"Рапорт сохранён: {path}";
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
                LocalAirState = "Сбор...";
                StatusText = "Сбор локального воздуха";
            }
            else
            {
                _localAir.RequestReset();
                LocalAirState = "Сброс";
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
        RxPositionText = identity.RxPosition.ToString();
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

    private async Task RunAutoModeAsync()
    {
        AppLogger.Write("=== ЗАПУСК РЕЖИМА АВТО ===");
        if (!IsConnected || !IsRingConnected || !_ringPort.IsOpen)
        {
            AppLogger.Write("Режим АВТО: прибор или кольцо не подключены.");
            StatusText = "Режим АВТО: подключите прибор и кольцо";
            IsAutoMode = false;
            return;
        }

        if (_loopParameters.Count == 0)
        {
            AppLogger.Write("Режим АВТО: параметры кольца не загружены, читаем...");
            await LoadRingParametersAsync();
        }

        if (_channelCount <= 0 || _resistivityReferences.Count <= MeasurementTable.ResistanceCount)
        {
            AppLogger.Write("Режим АВТО: нет расчётных эталонов кольца.");
            StatusText = "Режим АВТО: нет расчётных эталонов кольца";
            IsAutoMode = false;
            return;
        }

        _autoCts?.Cancel();
        _autoCts = new CancellationTokenSource();
        var token = _autoCts.Token;

        ClearResistanceCells();
        ClearResistanceBuffers();
        SetActiveMeasurementPoint(0, 0);
        StatusText = "Режим АВТО: запуск";
        AppLogger.Write("Режим АВТО: начало цикла R1..R8 (эталонный алгоритм).");

        try
        {
            for (var resistanceIndex = 1; resistanceIndex <= MeasurementTable.ResistanceCount; resistanceIndex++)
            {
                token.ThrowIfCancellationRequested();

                AppLogger.Write($"АВТО: старт измерения R{resistanceIndex}.");
                var tcs = new TaskCompletionSource<bool>();
                lock (_autoLock)
                {
                    _autoMeasurementCompleteTcs = tcs;
                    SetActiveMeasurementPoint(resistanceIndex, 1);
                }

                StatusText = $"Режим АВТО: R{resistanceIndex}, C1";
                LogCapMap(resistanceIndex, 1);
                AppLogger.Write($"АВТО: задержка 200 мс перед установкой R{resistanceIndex}/C1 (как в R_Click).");
                await Task.Delay(200, token);
                AppLogger.Write($"АВТО: установка R{resistanceIndex}/C1...");
                var result = await Task.Run(
                    () => RingConnection.SetMeasurementPoint(_ringPort, resistanceIndex, 1),
                    token);
                if (result != RingWorkResult.Ok)
                {
                    AppLogger.Write($"АВТО: ошибка установки R{resistanceIndex}/C1 ({result}).");
                    StatusText = $"Режим АВТО: ошибка установки R{resistanceIndex}/C1 ({result})";
                    return;
                }

                AppLogger.Write($"АВТО: R{resistanceIndex}/C1 установлена, ожидание усреднения кадров...");
                using (token.Register(() => tcs.TrySetCanceled()))
                {
                    await tcs.Task;
                }

                AppLogger.Write($"АВТО: R{resistanceIndex} измерено.");
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
            if (!IsAutoMode || _autoMeasurementCompleteTcs is null)
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
            _ = Task.Run(() => RingConnection.SetMeasurementPoint(_ringPort, nextR, nextC));
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

                    LogFramePhases("Фазы DELTA_PH", frame.Phases, _channelCount);
                    LogFramePhases("Сырые фазы", frame.RawPhases, _channelCount);
                    LogFramePhases("Локальный воздух", air, _channelCount);
                    LogFramePhases("Скорректированные фазы", corrected, _channelCount);

                    var resistivity = LoopResistivity.RoDfi(_sondeParameters, corrected);
                    AppLogger.Write("Вычислены УЭС по скорректированным фазам.");

                    _frameCounter++;
                    DispatchFrame(frame.RawPhases, air, corrected, resistivity, _frameCounter);
                }
                else
                {
                    AppLogger.Write($"ОШИБКА опроса прибора: {frame.Code}");
                    DispatchStatus($"Ошибка опроса: {frame.Code}");
                }
            }

            try
            {
                await Task.Delay(PollIntervalMs, token);
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

    private void DispatchFrame(V10 raw, V10 air, V10 corrected, V10 resistivity, int frameNumber)
    {
        var rawCopy = raw;
        var airCopy = air;
        var correctedCopy = corrected;
        var resistivityCopy = resistivity;

        App.DispatcherQueue.TryEnqueue(() =>
        {
            FrameCount = frameNumber;
            var count = Math.Min(Channels.Count, Math.Min(ResistanceRows.Count, V10.ChannelCount));
            for (var i = 0; i < count; i++)
            {
                Channels[i].RawText = (rawCopy.GetValue(i) * PhysicalConstants.Milligrad).ToString("F1");
                Channels[i].AirText = (airCopy.GetValue(i) * PhysicalConstants.Milligrad).ToString("F1");
                Channels[i].CorrectedText = (correctedCopy.GetValue(i) * PhysicalConstants.Milligrad).ToString("F1");
                ResistanceRows[i].AirText = (correctedCopy.GetValue(i) * PhysicalConstants.Milligrad).ToString("F0");
            }

            UpdateResistanceRows(resistivityCopy, correctedCopy);
            AdvanceAutoMeasurement();
            LocalAirState = _localAir.IsSet ? "Задан" : _localAir.Collecting ? "Сбор..." : "Не задан";
        });
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
            ResistanceRows[i].IndicatorAverageValue = buffer?.Average() ?? 0.0;
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
            active.CapacitanceIndex <= 0 ||
            active.FrameIndex <= MeasurementSkipFrames)
        {
            return;
        }

        var resistanceIndex = active.ResistanceIndex;
        var channelIndex = active.CapacitanceIndex - 1;
        var activeChannels = GetActiveMeasurementChannels(resistanceIndex, active.CapacitanceIndex, channelCount).ToArray();
        if (activeChannels.Length > 0)
        {
            foreach (var activeChannel in activeChannels)
            {
                UpdateReferenceResistanceCellForChannel(in resistivity, in corrected, resistanceIndex, activeChannel);
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

    private int GetNextCapacitanceIndex(int resistanceIndex, int capacitanceIndex)
    {
        if (ExpressMode && resistanceIndex > ExpressFullScanResistanceLimit)
        {
            return capacitanceIndex + Math.Max(1, _channelCount / 2);
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

        if (ExpressMode && resistanceIndex > ExpressFullScanResistanceLimit)
        {
            var groupSize = Math.Max(1, channelCount / 2);
            var end = Math.Min(channelCount, start + groupSize);
            for (var channel = start; channel < end; channel++)
            {
                yield return channel;
            }

            yield break;
        }

        yield return start;
    }

    private void UpdateReferenceResistanceCellForChannel(in V10 resistivity, in V10 corrected, int resistanceIndex, int channelIndex)
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

        AppLogger.Write(
            $"R{resistanceIndex} channel={channelIndex}: phase_ref_mG={referencePhase * PhysicalConstants.Milligrad:F1}, " +
            $"phase_measured_mG={measuredPhase * PhysicalConstants.Milligrad:F1}, phase_percent={percent:F1}, average={averaged:F1}, " +
            $"ref_ues={referenceValue:F2}, measured_ues={measuredValue:F2}");
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

    private void ClearResistanceCells()
    {
        foreach (var row in ResistanceRows)
        {
            for (var resistanceIndex = 1; resistanceIndex <= MeasurementTable.ResistanceCount; resistanceIndex++)
            {
                row.SetResistanceCell(resistanceIndex, MeasurementCell.Empty);
            }
        }
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

    private IReadOnlyList<LoopReportChannel> BuildReportChannels()
    {
        var channels = new List<LoopReportChannel>(ResistanceRows.Count);
        for (var channelIndex = 0; channelIndex < ResistanceRows.Count; channelIndex++)
        {
            var row = ResistanceRows[channelIndex];
            var cells = new List<LoopReportCell>(MeasurementTable.ResistanceCount);
            for (var resistanceIndex = 1; resistanceIndex <= MeasurementTable.ResistanceCount; resistanceIndex++)
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
    }

    private void DispatchStatus(string message)
    {
        App.DispatcherQueue.TryEnqueue(() => StatusText = message);
    }

    private readonly record struct ToolPortOperationResult<T>(bool Success, T Value, string? ErrorMessage)
    {
        public static ToolPortOperationResult<T> Ok(T value) => new(true, value, null);

        public static ToolPortOperationResult<T> Fail(string errorMessage) => new(false, default!, errorMessage);
    }
}
