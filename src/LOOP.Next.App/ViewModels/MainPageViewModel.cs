using System.Collections.ObjectModel;
using System.IO.Ports;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using LoopNext.Core.Models;
using LoopNext.Core.Protocol;
using LoopNext.Hardware;
using LoopNext.Metrology;

namespace LOOP_Next_App.ViewModels;

// ViewModel главного окна: подключение прибора по COM-порту, фоновый опрос кадров,
// применение локальной воздушной поправки и отображение идентификации/фаз.
// Связывает перенесённые слои Hardware и Metrology для сквозного теста логики.
public partial class MainPageViewModel : ObservableObject
{
    private readonly SerialPort _toolPort = new();
    private readonly SerialPort _ringPort = new();
    private readonly LocalAirCorrector _localAir = new();
    private readonly object _airLock = new();

    private CancellationTokenSource? _pollCts;
    private IReadOnlyList<SondeParameter> _sondeParameters = Array.Empty<SondeParameter>();
    private int _channelCount;
    private uint _rxPosition;
    private int _frameCounter;

    public MainPageViewModel()
    {
        RefreshPorts();
    }

    public ObservableCollection<string> ToolPorts { get; } = new();

    public ObservableCollection<string> RingPorts { get; } = new();

    public ObservableCollection<ChannelReadingViewModel> Channels { get; } = new();

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
    public partial int SelectedAveraging { get; set; } = 10;

    [ObservableProperty]
    public partial int ThresholdPercent { get; set; } = 5;

    // Режим АВТО и экспресс-режим: индикаторы интерфейса до переноса оркестрации измерений.
    [ObservableProperty]
    public partial bool IsAutoMode { get; set; }

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
        BuildChannels(_sondeParameters);

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
        StatusText = "Кольцо отключено";
    }

    [RelayCommand]
    private void Report()
    {
        StatusText = "Рапорт: формирование отчёта (этап 8)";
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

    private void BuildChannels(IReadOnlyList<SondeParameter> sondeParameters)
    {
        var table = new MeasurementTable();
        table.SetHeaders(sondeParameters);

        Channels.Clear();
        foreach (var header in table.Headers)
        {
            Channels.Add(new ChannelReadingViewModel(header.ChannelIndex, header.Label));
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

                    _frameCounter++;
                    DispatchFrame(frame.Phases, air, corrected, _frameCounter);
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

    private void DispatchFrame(V10 raw, V10 air, V10 corrected, int frameNumber)
    {
        var rawCopy = raw;
        var airCopy = air;
        var correctedCopy = corrected;

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

            LocalAirState = _localAir.IsSet ? "Задан" : _localAir.Collecting ? "Сбор..." : "Не задан";
        });
    }

    private void DispatchStatus(string message)
    {
        App.DispatcherQueue.TryEnqueue(() => StatusText = message);
    }
}
