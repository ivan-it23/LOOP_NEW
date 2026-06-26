using System.Collections.ObjectModel;
using System.IO.Ports;
using System.Linq;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using LoopNext.Core.Models;
using LoopNext.Core.Protocol;
using LoopNext.Hardware;
using LoopNext.Metrology;
using LOOP.Next.Reporting;

namespace LOOP_Next_App.ViewModels;

// ViewModel главного окна: подключение прибора по COM-порту, фоновый опрос кадров,
// применение локальной воздушной поправки и отображение идентификации/фаз.
// Связывает перенесённые слои Hardware и Metrology для сквозного теста логики.
public partial class MainPageViewModel : ObservableObject
{
    private const int MeasurementSkipFrames = 5;

    private readonly SerialPort _toolPort = new();
    private readonly SerialPort _ringPort = new();
    private readonly LocalAirCorrector _localAir = new();
    private readonly object _airLock = new();
    private readonly object _measurementLock = new();

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
    private uint _rxPosition;
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

    // Усреднение и порог отклонения используются логикой таблицы (этап 6);
    // здесь хранятся для отображения и последующей привязки.
    [ObservableProperty]
    public partial int SelectedAveraging { get; set; } = 5;

    [ObservableProperty]
    public partial int ThresholdPercent { get; set; } = 6;

    // Режим АВТО и экспресс-режим: индикаторы интерфейса до переноса оркестрации измерений.
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

    partial void OnIsConnectedChanged(bool value) =>
        ConnectionStateText = value ? "Включено" : "Выключено";

    public bool CanConnect => !IsConnected && !string.IsNullOrWhiteSpace(SelectedToolPort);

    public bool CanConnectRing => !IsRingConnected && !string.IsNullOrWhiteSpace(SelectedRingPort);

    public bool CanToggleTool => IsConnected || !string.IsNullOrWhiteSpace(SelectedToolPort);

    public bool CanToggleRing => IsRingConnected || !string.IsNullOrWhiteSpace(SelectedRingPort);

    public string ToolToggleText => IsConnected ? "Отключить" : "Прибор";

    public string RingToggleText => IsRingConnected ? "Отключить" : "Кольцо";

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
            StatusText = "Не удалось открыть порт прибора";
            IsConnected = false;
            return;
        }

        _sondeParameters = result.SondeParameters;
        _channelCount = _sondeParameters.Count;
        _rxPosition = result.Metrology.RxPosition;

        ApplyIdentification(result.Metrology);
        BuildChannels(_sondeParameters, result.Metrology);
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
            StatusText = $"Кольцо: {status}";
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
    private void CalculateRingCalibration()
    {
        if (_sondeParameters.Count == 0)
        {
            StatusText = "Нет метрологии зонда для расчёта кольца";
            return;
        }

        var count = Math.Min(RingCalibrationRows.Count, _sondeParameters.Count);
        for (var i = 0; i < count; i++)
        {
            var value = RingCalibrationCalculator.Calculate(_sondeParameters[i]);
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
        StatusText = result.Success && hasRequiredParameters
            ? $"Параметры кольца прочитаны: {_loopParameters.Count}"
            : "Параметры кольца не получены";
    }

    [RelayCommand]
    private void WriteRingParameters()
    {
        StatusText = "Запись параметров кольца будет перенесена следующим шагом";
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
                GetAirZzText(in metrology, header.ChannelIndex, transmitterCount)));
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
            ResistanceRows.Add(new ResistanceTableRowViewModel(i, labels[i], string.Empty));
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
    }

    private void RefreshRingResistanceRows()
    {
        RingResistanceRows.Clear();

        foreach (var row in RingCalibrationRows.Take(8))
        {
            RingResistanceRows.Add(row);
        }
    }

    private void RecalculateLoopReferences()
    {
        if (_sondeParameters.Count == 0 || _loopParameters.Count == 0)
        {
            _phaseReferences = Array.Empty<V10>();
            _resistivityReferences = Array.Empty<V10>();
            return;
        }

        _phaseReferences = LoopResistivity.CalcPhaseFromLoop(_sondeParameters, _loopParameters);
        _resistivityReferences = LoopResistivity.CalcRoFromLoop(_sondeParameters, _phaseReferences);
    }

    private async Task RunAutoModeAsync()
    {
        if (!IsConnected || !IsRingConnected || !_ringPort.IsOpen)
        {
            StatusText = "Режим АВТО: подключите прибор и кольцо";
            IsAutoMode = false;
            return;
        }

        if (_loopParameters.Count == 0)
        {
            await LoadRingParametersAsync();
        }

        if (_channelCount <= 0 || _resistivityReferences.Count <= MeasurementTable.ResistanceCount)
        {
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

        try
        {
            var capacitanceCount = Math.Clamp(_channelCount, 0, V10.ChannelCount);
            for (var resistanceIndex = 1; resistanceIndex <= MeasurementTable.ResistanceCount; resistanceIndex++)
            {
                for (var capacitanceIndex = 1; capacitanceIndex <= capacitanceCount; capacitanceIndex++)
                {
                    token.ThrowIfCancellationRequested();
                    var result = await Task.Run(
                        () => RingConnection.SetMeasurementPoint(_ringPort, resistanceIndex, capacitanceIndex),
                        token);
                    if (result != RingWorkResult.Ok)
                    {
                        StatusText = $"Режим АВТО: ошибка установки R{resistanceIndex}/C{capacitanceIndex} ({result})";
                        return;
                    }

                    SetActiveMeasurementPoint(resistanceIndex, capacitanceIndex);
                    StatusText = $"Режим АВТО: R{resistanceIndex}, C{capacitanceIndex}";
                    var delay = Math.Max(PollIntervalMs * (SelectedAveraging + MeasurementSkipFrames + 1), 1000);
                    await Task.Delay(delay, token);
                }
            }
        }
        catch (OperationCanceledException)
        {
            StatusText = "Режим АВТО остановлен";
        }
        finally
        {
            SetActiveMeasurementPoint(0, 0);
            if (_ringPort.IsOpen)
            {
                await Task.Run(() => RingConnection.SetMeasurementPoint(_ringPort, 0, 0), CancellationToken.None);
            }

            if (IsAutoMode)
            {
                IsAutoMode = false;
            }
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

    // Фоновый цикл опроса: блокирующее чтение COM выполняется вне UI-потока,
    // обновление привязанных свойств маршалируется в UI-поток через DispatcherQueue.
    private async Task PollLoopAsync(CancellationToken token)
    {
        while (!token.IsCancellationRequested)
        {
            if (_toolPort.IsOpen)
            {
                var frame = ToolPort.GetPhaseFromTool(_toolPort, ToolProtocolConstants.CmdGetData, _rxPosition);

                if (frame.Code == ToolResponseCode.Ok)
                {
                    V10 air;
                    V10 corrected;
                    lock (_airLock)
                    {
                        corrected = _localAir.ProcessFrame(frame.Phases, _channelCount);
                        air = _localAir.Air;
                    }

                    var resistivity = LoopResistivity.RoDfi(_sondeParameters, corrected);

                    _frameCounter++;
                    DispatchFrame(frame.Phases, air, corrected, resistivity, _frameCounter);
                }
                else
                {
                    DispatchStatus($"Ошибка опроса: {frame.Code}");
                }
            }

            try
            {
                await Task.Delay(PollIntervalMs, token);
            }
            catch (TaskCanceledException)
            {
                break;
            }
        }
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
            var count = Math.Min(Channels.Count, V10.ChannelCount);
            for (var i = 0; i < count; i++)
            {
                Channels[i].RawText = (rawCopy.GetValue(i) * PhysicalConstants.Milligrad).ToString("F1");
                Channels[i].AirText = (airCopy.GetValue(i) * PhysicalConstants.Milligrad).ToString("F1");
                Channels[i].CorrectedText = (correctedCopy.GetValue(i) * PhysicalConstants.Milligrad).ToString("F1");
            }

            UpdateResistanceRows(resistivityCopy);
            LocalAirState = _localAir.IsSet ? "Задан" : _localAir.Collecting ? "Сбор..." : "Не задан";
        });
    }

    private void UpdateResistanceRows(V10 resistivity)
    {
        var count = Math.Min(ResistanceRows.Count, Math.Min(_channelCount, V10.ChannelCount));
        for (var i = 0; i < count; i++)
        {
            ResistanceRows[i].UesText = FormatResistivity(resistivity.GetValue(i));
        }

        UpdateReferenceResistanceCells(resistivity, count);
    }

    private void UpdateReferenceResistanceCells(in V10 resistivity, int channelCount)
    {
        if (_resistivityReferences.Count <= MeasurementTable.ResistanceCount)
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
        if (!float.IsFinite(referenceValue) || !float.IsFinite(measuredValue) || referenceValue <= 0f || measuredValue <= 0f)
        {
            ResistanceRows[channelIndex].SetResistanceCell(resistanceIndex, MeasurementCell.Empty);
            return;
        }

        var percent = 100f * (measuredValue - referenceValue) / (referenceValue + 0.000000001f);
        var averaged = PushResistancePercent(resistanceIndex - 1, channelIndex, percent);
        ResistanceRows[channelIndex].SetResistanceCell(
            resistanceIndex,
            MeasurementCell.Create(averaged, ThresholdPercent));
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

    private void DispatchStatus(string message)
    {
        App.DispatcherQueue.TryEnqueue(() => StatusText = message);
    }
}
