using System.Collections.Generic;
using System.IO;
using System.IO.Ports;
using System.Runtime.InteropServices;
using System.Text;
using LoopNext.Core.Logging;
using LoopNext.Core.Models;
using LoopNext.Core.Protocol;
using LoopNext.Metrology;

namespace LoopNext.Hardware;

// Высокоуровневый опрос прибора по открытому COM-порту.
// Перенос get_ph_from_tool (function.cpp:318) и read_tool_signature (function.cpp:296).
public static class ToolPort
{
    // Нагрузка ответа на 0x16: четыре байта самой сигнатуры.
    private const int SignaturePayloadLength = 4;
    private const int MaxRetries = 1;
    private const int PreFrameReceiveDelayMs = 50;
    private const int SignatureReceiveDelayMs = 50;

    // Полный дамп кадра пишется один раз за сеанс связи — см. LogRawToolData.
    private static bool _loggedFrameDump;

    // Отметка о Картографе с новой прошивкой: одна на сеанс, иначе она шла бы
    // на каждом кадре.
    private static bool _loggedCartographExtendedFirmware;

    /// <summary>
    /// Сбрасывает однократную диагностику, чтобы новый сеанс связи снова
    /// выдал дамп кадра. Иначе при смене прибора без перезапуска приложения
    /// в журнале остался бы дамп предыдущего.
    /// </summary>
    public static void ResetSessionDiagnostics()
    {
        _loggedFrameDump = false;
        _loggedCartographExtendedFirmware = false;
    }

    // Постоянная нагрузка запроса данных, как в эталоне: { 0x05, 0x00 }.
    private static readonly byte[] RequestData = { 0x05, 0x00 };

    public readonly record struct PhaseFrameResult(
        V10 Phases,
        V10 RawPhases,
        ToolResponseCode Code,
        float Temperature,
        uint Condition);

    public readonly record struct SignatureResult(ToolResponseCode Code, uint Signature);

    /// <summary>
    /// Опрос кадра измерений. Транспорт выбирается по типу прибора, а не по
    /// поколению прошивки.
    /// </summary>
    /// <remarks>
    /// Оба поколения отвечают одинаково устроенным пакетом
    /// <c>[CMD][LEN:2 LE][payload][CRC16:2 LE]</c>, где <c>LEN</c> — длина всего
    /// пакета. Старая прошивка ставит туда постоянные 245 (кадр 240 байт), новая
    /// вычисляет <c>struct_size + 5</c>, то есть 325 для кадра 320 байт. Значит,
    /// размер кадра называет сам провод, и угадывать его по сигнатуре метрологии
    /// не нужно: сигнатура служит проверкой, а не источником.
    ///
    /// Исключение одно — Картограф в режиме LWD 4tx **старой прошивки**: вместо
    /// поля длины он шлёт два служебных байта, поэтому его ответ читается
    /// пакетом известной длины.
    ///
    /// После перепрошивки Картограф этой особенности не сохраняет. Подтверждено
    /// прибором 320349001: на запрос кадра он отвечает
    /// <c>11 45 01 49 23 18 13 …</c>, где <c>45 01</c> — поле длины 325, а не
    /// служебная пара <c>05 06</c>. Он же, вопреки прежнему описанию протокола,
    /// отвечает и на <c>0x16</c>. То есть признаком транспорта служит поколение
    /// прошивки, а не тип прибора.
    /// </remarks>
    public static PhaseFrameResult GetPhaseFromTool(SerialPort com, byte cmd, in GpToolMetrology metrology)
    {
        if (!com.IsOpen)
        {
            return EmptyFrame(ToolResponseCode.PortClosed);
        }

        return UsesFixedLengthTransport(ToolIdentity.Decode(metrology.Signature))
            ? GetPhaseFromFixedLengthTool(com, cmd, in metrology)
            : GetPhaseFromFramedTool(com, cmd, in metrology);
    }

    /// <summary>
    /// Прибор отвечает пакетом без поля длины. Единственный такой случай —
    /// Картограф в режиме LWD 4tx со старой прошивкой.
    /// </summary>
    public static bool UsesFixedLengthTransport(in ToolIdentity identity) =>
        identity.IsCartographLwd4TxMode && !identity.IsExtendedFirmware;

    // Общий путь обоих поколений: длина кадра читается из заголовка ответа.
    private static PhaseFrameResult GetPhaseFromFramedTool(
        SerialPort com,
        byte cmd,
        in GpToolMetrology metrology)
    {
        var lastCode = ToolResponseCode.Timeout;

        for (var retry = 0; retry <= MaxRetries; retry++)
        {
            try
            {
                com.DiscardInBuffer();
            }
            catch (Exception ex) when (ex is InvalidOperationException or IOException)
            {
                AppLogger.Write(ex, "Порт прибора закрыт во время подготовки к опросу кадра.");
                return EmptyFrame(ToolResponseCode.PortClosed);
            }

            if (ToolSerialTransport.SendFramed(com, cmd, ReadOnlySpan<byte>.Empty) != 0)
            {
                return EmptyFrame(ToolResponseCode.Timeout);
            }

            Thread.Sleep(PreFrameReceiveDelayMs);

            byte responseCommand;
            byte[] payload;
            try
            {
                lastCode = ToolSerialTransport.ReceiveFramed(com, out responseCommand, out payload);
            }
            catch (Exception ex) when (ex is InvalidOperationException or IOException)
            {
                AppLogger.Write(ex, "Порт прибора закрыт во время чтения кадра.");
                return EmptyFrame(ToolResponseCode.PortClosed);
            }

            if (lastCode == ToolResponseCode.Ok)
            {
                if (responseCommand != cmd)
                {
                    AppLogger.Write($"TOOL_FRAME_REJECTED: cmd=0x{responseCommand:X2} expected_cmd=0x{cmd:X2}");
                    lastCode = ToolResponseCode.InvalidResponse;
                }
                else if (!TryAcceptFrame(payload, in metrology, out var toolData, out var rejection))
                {
                    AppLogger.Write($"TOOL_FRAME_REJECTED: {rejection}");
                    lastCode = ToolResponseCode.InvalidResponse;
                }
                else
                {
                    LogRawToolData(in toolData, payload);
                    var phaseResult = ToolPhaseProcessor.FillPhaseFromToolData(in toolData, in metrology);
                    return new PhaseFrameResult(
                        phaseResult.Phases,
                        phaseResult.RawPhases,
                        ToolResponseCode.Ok,
                        toolData.Temperature,
                        toolData.Condition);
                }
            }

            if (retry < MaxRetries)
            {
                AppLogger.Write(
                    $"TOOL_FRAME_RETRY: result={lastCode}, attempt={retry + 1}/{MaxRetries + 1}, " +
                    $"metrology_signature=0x{metrology.Signature:X8}");
            }
        }

        return EmptyFrame(lastCode);
    }

    // Картограф в режиме LWD 4tx: поля длины в ответе нет, кадр всегда 240 байт,
    // а CRC на реальных экземплярах приходит нулевым.
    private static PhaseFrameResult GetPhaseFromFixedLengthTool(
        SerialPort com,
        byte cmd,
        in GpToolMetrology metrology)
    {
        const int framePayloadLength = ToolProtocolConstants.ToolPayloadLength;
        var dataIn = new byte[framePayloadLength];
        var lastCode = ToolResponseCode.Timeout;

        for (var retry = 0; retry <= MaxRetries; retry++)
        {
            try
            {
                com.DiscardInBuffer();
            }
            catch (Exception ex) when (ex is InvalidOperationException or IOException)
            {
                AppLogger.Write(ex, "Порт прибора закрыт во время подготовки к опросу кадра.");
                return EmptyFrame(ToolResponseCode.PortClosed);
            }

            if (ToolSerialTransport.Send(com, cmd, RequestData, RequestData.Length) != 0)
            {
                return EmptyFrame(ToolResponseCode.Timeout);
            }

            Thread.Sleep(PreFrameReceiveDelayMs);

            try
            {
                lastCode = (ToolResponseCode)ToolSerialTransport.Receive(
                    com,
                    out _,
                    dataIn,
                    framePayloadLength,
                    acceptZeroCrc: true);
            }
            catch (Exception ex) when (ex is InvalidOperationException or IOException)
            {
                AppLogger.Write(ex, "Порт прибора закрыт во время чтения кадра.");
                return EmptyFrame(ToolResponseCode.PortClosed);
            }

            if (lastCode == ToolResponseCode.Ok)
            {
                // Структура лежит со смещения 2: первые два байта нагрузки —
                // служебные, полем длины они у этого прибора не являются.
                var frame = dataIn.AsSpan(ToolProtocolConstants.ToolServicePrefixLength);
                if (!TryAcceptFrame(frame, in metrology, out var toolData, out var rejection))
                {
                    AppLogger.Write($"CARTOGRAPH_LWD_FRAME_REJECTED: {rejection}");
                    lastCode = ToolResponseCode.CrcError;
                }
                else
                {
                    LogRawToolData(in toolData, frame);
                    var phaseResult = ToolPhaseProcessor.FillPhaseFromToolData(in toolData, in metrology);
                    return new PhaseFrameResult(
                        phaseResult.Phases,
                        phaseResult.RawPhases,
                        ToolResponseCode.Ok,
                        toolData.Temperature,
                        toolData.Condition);
                }
            }

            if (retry < MaxRetries)
            {
                AppLogger.Write(
                    $"TOOL_FRAME_RETRY: result={lastCode}, attempt={retry + 1}/{MaxRetries + 1}, " +
                    $"metrology_signature=0x{metrology.Signature:X8}");
            }
        }

        return EmptyFrame(lastCode);
    }

    /// <summary>
    /// Единая приёмка кадра: размер, сигнатура и согласие с метрологией.
    /// </summary>
    /// <remarks>
    /// Размер кадра, названный сигнатурой, обязан совпадать с фактически
    /// принятой длиной: в пространстве uint32 пересечения двух схем разбора
    /// существуют, и только фактическая длина отличает поколение наверняка.
    ///
    /// Сигнатура кадра сверяется с сигнатурой метрологии для всех приборов, а не
    /// только для Картографа: расхождение означает, что кадр разбирается по
    /// чужой геометрии. Проверка включается лишь когда метрология действительно
    /// считана, иначе она заблокировала бы работу с неразмеченным прибором.
    /// </remarks>
    private static bool TryAcceptFrame(
        ReadOnlySpan<byte> frame,
        in GpToolMetrology metrology,
        out GpToolData toolData,
        out string? rejection)
    {
        toolData = default;
        rejection = null;

        if (!ToolProtocolConstants.IsKnownFrameSize(frame.Length))
        {
            rejection = $"неизвестный размер кадра {frame.Length} Б";
            return false;
        }

        toolData = MemoryMarshal.Read<GpToolData>(frame);
        var identity = ToolIdentity.Decode(toolData.Signature);

        if (!identity.IsValid)
        {
            rejection = $"сигнатура кадра 0x{toolData.Signature:X8} не описывает поддерживаемый прибор";
            return false;
        }

        if (identity.FrameSize != frame.Length)
        {
            rejection =
                $"сигнатура кадра 0x{toolData.Signature:X8} обещает {identity.FrameSize} Б, принято {frame.Length} Б";
            return false;
        }

        if (metrology.Signature != 0 && toolData.Signature != metrology.Signature)
        {
            rejection =
                $"сигнатура кадра 0x{toolData.Signature:X8} не совпадает с метрологией 0x{metrology.Signature:X8}";
            return false;
        }

        // Картограф на новой прошивке отмечается один раз за сеанс: конфигурация
        // редкая, и по журналу должно быть видно, что прибор пошёл по общей
        // ветке транспорта, а не по своей исторической.
        if (!_loggedCartographExtendedFirmware &&
            identity.DeviceType == ToolIdentity.DeviceTypeCartograph &&
            identity.IsExtendedFirmware)
        {
            _loggedCartographExtendedFirmware = true;
            AppLogger.Write(
                $"CARTOGRAPH_EXTENDED_FIRMWARE: signature=0x{toolData.Signature:X8} ({toolData.Signature}), " +
                $"кадр={frame.Length} Б — Картограф с новой прошивкой: транспорт с полем длины, как у остальных приборов");
        }

        return true;
    }

    private static PhaseFrameResult EmptyFrame(ToolResponseCode code)
    {
        var phases = default(V10);
        phases.Clear();
        var rawPhases = default(V10);
        rawPhases.Clear();
        return new PhaseFrameResult(phases, rawPhases, code, 0f, 0u);
    }


    // Полный разбор принятого кадра: все поля структуры измерений и, для новой
    // прошивки, её хвост с амплитудными величинами.
    //
    // Хвост читается прямо из принятой нагрузки: расчёты этого приложения идут
    // только по разности фаз, поэтому амплитудные поля не заведены в структуру
    // данных, а в журнал попадают ради проверки обмена.
    private static void LogRawToolData(in GpToolData data, ReadOnlySpan<byte> frame)
    {
        var identity = ToolIdentity.Decode(data.Signature);
        var txCount = (int)identity.TransmitterCount;
        var frameSize = frame.Length;

        AppLogger.Write(
            $"TOOL_FRAME: signature=0x{data.Signature:X8} ({data.Signature}) | {DescribeDevice(identity)} | " +
            $"прошивка={(identity.IsExtendedFirmware ? "новая" : "старая")} кадр={identity.FrameSize}Б принято={frameSize}Б | " +
            $"frame={data.Frame} | temperature={data.Temperature:F3} °C");
        AppLogger.Write(
            $"TOOL_FRAME_CONDITION: raw=0x{data.Condition:X8} | {DescribeCondition(data.Condition, txCount)}");

        AppLogger.Write($"TOOL_FRAME_RHO_SMT (Ohm*m): {FormatChannels(data.GetRhoSmt, txCount, 1.0f, "F2")}");
        AppLogger.Write(
            $"TOOL_FRAME_PHASE_SMT (mG): " +
            $"{FormatChannels(data.GetPhaseSmt, txCount, (float)PhysicalConstants.Milligrad, "F1")}");
        AppLogger.Write($"TOOL_FRAME_AM_RX_1: {FormatChannels(data.GetAmRx1, txCount, 1.0f, "F4")}");
        AppLogger.Write($"TOOL_FRAME_AM_RX_2: {FormatChannels(data.GetAmRx2, txCount, 1.0f, "F4")}");
        AppLogger.Write(
            $"TOOL_FRAME_DELTA_PH (mG): " +
            $"{FormatChannels(data.GetDeltaPhase, txCount, (float)PhysicalConstants.Milligrad, "F1")}");
        AppLogger.Write(
            $"TOOL_FRAME_SILENT: ZERO_AM_RX_1=[{data.GetZeroAmRx1(0):F4}, {data.GetZeroAmRx1(1):F4}] " +
            $"ZERO_AM_RX_2=[{data.GetZeroAmRx2(0):F4}, {data.GetZeroAmRx2(1):F4}] " +
            $"ZERO_dPH_mG=[{data.GetZeroDeltaPhase(0) * PhysicalConstants.Milligrad:F1}, " +
            $"{data.GetZeroDeltaPhase(1) * PhysicalConstants.Milligrad:F1}]");

        LogExtendedTail(frame, txCount);

        // Полный дамп принятых байт пишется один раз за сессию: по нему
        // сверяются смещения полей, а повторять его на каждом кадре означало бы
        // утопить журнал.
        if (!_loggedFrameDump)
        {
            _loggedFrameDump = true;
            AppLogger.Write(ToolSerialTransport.FormatHexDump("TOOL_FRAME_DUMP (первый кадр сессии)", frame));
        }
    }

    // Амплитудные величины новой прошивки: rho_att_smt со смещения 240 и
    // att_dB_smt со смещения 280 внутри кадра.
    private static void LogExtendedTail(ReadOnlySpan<byte> frame, int txCount)
    {
        if (frame.Length != ToolIdentity.ExtendedFrameSize)
        {
            return;
        }

        AppLogger.Write(
            $"TOOL_FRAME_RHO_ATT_SMT (Ohm*m): " +
            $"{FormatRawChannels(frame, ExtendedFrameFields.TryReadRhoAttSmt, txCount, "F2")}");
        AppLogger.Write(
            $"TOOL_FRAME_ATT_DB_SMT (dB): " +
            $"{FormatRawChannels(frame, ExtendedFrameFields.TryReadAttDbSmt, txCount, "F5")}");
    }

    private static string DescribeDevice(ToolIdentity identity)
    {
        var typeName = identity.DeviceType switch
        {
            ToolIdentity.DeviceTypeAutonom => "Автономный",
            ToolIdentity.DeviceTypeLwd => "LWD",
            ToolIdentity.DeviceTypeCartograph => "Картограф",
            _ => "Неизвестно"
        };
        return $"{typeName} {identity.TransmitterCount}TX мод.{identity.Modification} № {identity.Number} " +
               $"(валидна={identity.IsValid})";
    }

    // Раскладку байта состояния держит GpToolData: она одна на оба поколения
    // прошивки, и второй её копии здесь быть не должно.
    private static string DescribeCondition(uint condition, int txCount)
    {
        var builder = new StringBuilder("рабочие каналы:");
        for (var freq = 0; freq < GpToolData.FrequencyCount; freq++)
        {
            builder.Append(freq == 0 ? " 400 кГц " : " | 2000 кГц ");
            for (var tx = 0; tx < txCount && tx < GpToolData.MaxTransmitterCount; tx++)
            {
                builder.Append($"T{tx + 1}={(GpToolData.IsTransmitterActive(condition, freq, tx) ? 1 : 0)} ");
            }
        }

        return builder.ToString().TrimEnd();
    }

    private static string FormatChannels(
        Func<int, int, float> valueProvider,
        int txCount,
        float scale,
        string format)
    {
        var builder = new StringBuilder();
        for (var freq = 0; freq < GpToolData.FrequencyCount; freq++)
        {
            builder.Append(freq == 0 ? "400 кГц [" : " | 2000 кГц [");
            for (var tx = 0; tx < txCount; tx++)
            {
                if (tx > 0)
                {
                    builder.Append(", ");
                }

                builder.Append($"T{tx + 1}=").Append((valueProvider(freq, tx) * scale).ToString(format));
            }

            builder.Append(']');
        }

        return builder.ToString();
    }

    private delegate bool ExtendedFieldReader(ReadOnlySpan<byte> frame, int frequencyIndex, int transmitterIndex, out float value);

    private static string FormatRawChannels(
        ReadOnlySpan<byte> frame,
        ExtendedFieldReader reader,
        int txCount,
        string format)
    {
        var builder = new StringBuilder();
        for (var freq = 0; freq < GpToolData.FrequencyCount; freq++)
        {
            builder.Append(freq == 0 ? "400 кГц [" : " | 2000 кГц [");
            for (var tx = 0; tx < txCount; tx++)
            {
                if (tx > 0)
                {
                    builder.Append(", ");
                }

                builder.Append($"T{tx + 1}=")
                    .Append(reader(frame, freq, tx, out var value) ? value.ToString(format) : "—");
            }

            builder.Append(']');
        }

        return builder.ToString();
    }

    /// <summary>
    /// Чтение сигнатуры прибора командой <c>0x16</c>.
    /// </summary>
    /// <remarks>
    /// Обе прошивки отвечают пакетом из девяти байт
    /// <c>[CMD][09][00][signature:4][CRC:2]</c>. Это единственный источник
    /// идентификации, не зависящий от того, размечена ли метрология.
    ///
    /// Картограф в режиме LWD 4tx этой команды не имеет и просто молчит,
    /// поэтому проба идёт с укороченным тайм-аутом: иначе его подключение ждало
    /// бы общие две секунды впустую. Исходное значение тайм-аута
    /// восстанавливается в любом случае.
    /// </remarks>
    public static SignatureResult ReadSignature(SerialPort com)
    {
        if (!com.IsOpen)
        {
            return new SignatureResult(ToolResponseCode.PortClosed, 0);
        }

        var previousTimeout = com.ReadTimeout;
        try
        {
            com.ReadTimeout = ToolProtocolConstants.SignatureProbeTimeoutMs;
            com.DiscardInBuffer();

            if (ToolSerialTransport.SendFramed(com, ToolProtocolConstants.CmdGetSignature, ReadOnlySpan<byte>.Empty) != 0)
            {
                return new SignatureResult(ToolResponseCode.Timeout, 0);
            }

            Thread.Sleep(SignatureReceiveDelayMs);

            var result = ToolSerialTransport.ReceiveFramed(com, out var recvCmd, out var payload);
            if (result != ToolResponseCode.Ok)
            {
                return new SignatureResult(result, 0);
            }

            if (recvCmd != ToolProtocolConstants.CmdGetSignature || payload.Length != SignaturePayloadLength)
            {
                AppLogger.Write(
                    $"TOOL_SIGNATURE_REJECTED: cmd=0x{recvCmd:X2} payload={payload.Length} " +
                    $"expected_cmd=0x{ToolProtocolConstants.CmdGetSignature:X2} expected_payload={SignaturePayloadLength}");
                return new SignatureResult(ToolResponseCode.InvalidResponse, 0);
            }

            return new SignatureResult(ToolResponseCode.Ok, BitConverter.ToUInt32(payload, 0));
        }
        catch (Exception ex) when (ex is InvalidOperationException or IOException)
        {
            AppLogger.Write(ex, "Порт прибора закрыт во время чтения сигнатуры.");
            return new SignatureResult(ToolResponseCode.PortClosed, 0);
        }
        finally
        {
            try
            {
                com.ReadTimeout = previousTimeout;
            }
            catch (InvalidOperationException)
            {
                // Порт успели закрыть — восстанавливать нечего.
            }
        }
    }
}
