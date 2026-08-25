namespace LoopNext.Core.Models;

// Идентификация прибора по сигнатуре. Точный перенос decode_tool_signature (function.cpp:15).
// low20 = младшие 20 бит сигнатуры; toolCode = low20/1000 = XYZ:
//   X (тип): low20/100000, Y (число Tx): (low20%100000)/10000, Z (мод): (low20%10000)/1000.
// Для картографа в режиме LWD 4tx модификация 9 является отдельной сигнатурной веткой:
// встречаются коды 349/359, но оба должны работать как 4 Tx без изменения структуры кадра.
//
// Приборы после перепрошивки кодируют в сигнатуре ещё и размер кадра измерений:
//   сигнатура = размер_кадра*1000000 + тип*100000 + Tx*10000 + мод*1000 + номер.
// Если старшие разряды нулевые, прибор работает под старой прошивкой и кадр
// занимает 240 байт. Схема взята из отладочного проекта разработчика новой
// прошивки (get_sonde_id / get_signature).
public readonly record struct ToolIdentity(
    uint RawSignature,
    uint Low20,
    uint DeviceType,
    uint TransmitterCount,
    uint Modification,
    uint Number,
    bool IsValid,
    int FrameSize)
{
    // Типы прибора кодируются первой цифрой toolCode (variable.h:49-50).
    public const uint DeviceTypeAutonom = 1;
    public const uint DeviceTypeLwd = 2;
    public const uint DeviceTypeCartograph = 3;
    public const uint CartographLwd4TxModification = 9;

    // Размер кадра измерений: 240 байт у старой прошивки, 320 у новой.
    // Метрология в обоих поколениях занимает 240 байт.
    public const int LegacyFrameSize = 240;
    public const int ExtendedFrameSize = 320;

    private const uint FrameSizeScale = 1_000_000u;

    public bool IsCartographLwd4TxMode =>
        DeviceType == DeviceTypeCartograph &&
        Modification == CartographLwd4TxModification &&
        TransmitterCount == 4;

    // Прибор работает под новой прошивкой: кадр измерений 320 байт.
    public bool IsExtendedFirmware => FrameSize == ExtendedFrameSize;

    public static ToolIdentity Decode(uint signature)
    {
        // Ветка новой прошивки принимается, только если в старших разрядах
        // стоит известный размер кадра и остальные разряды описывают
        // поддерживаемый прибор. Без этой проверки сигнатура старого прибора с
        // мусором в старших битах (встречаются значения вида 0x3143ADD1) была
        // бы принята за новую и разобрана неверно.
        var frameSizeDigits = signature / FrameSizeScale;
        if (frameSizeDigits is ExtendedFrameSize or LegacyFrameSize)
        {
            var extendedPayload = signature % FrameSizeScale;
            var extended = DecodePayload(signature, extendedPayload, (int)frameSizeDigits);
            if (extended.IsValid)
            {
                return extended;
            }
        }

        var low20 = (signature << 12) >> 12;
        return DecodePayload(signature, low20, LegacyFrameSize);
    }

    // Разбор шести десятичных разрядов, одинаковый для обоих поколений.
    private static ToolIdentity DecodePayload(uint signature, uint payload, int frameSize)
    {
        var deviceType = payload / 100000u;
        var txCount = (payload % 100000u) / 10000u;
        var modification = (payload % 10000u) / 1000u;
        var number = payload % 1000u;
        var normalizedTxCount = NormalizeTransmitterCount(deviceType, txCount, modification);

        // Валидность определяется до клампинга n_tx, как в эталонном коде.
        var isValid = IsSupportedDeviceType(deviceType, txCount, modification)
                      && normalizedTxCount is >= 3 and <= 5;

        if (normalizedTxCount is < 3 or > 5)
        {
            normalizedTxCount = 5;
        }

        return new ToolIdentity(
            signature,
            payload,
            deviceType,
            normalizedTxCount,
            modification,
            number,
            isValid,
            frameSize);
    }

    private static uint NormalizeTransmitterCount(uint deviceType, uint txCount, uint modification)
    {
        if (IsCartographLwd4Tx(deviceType, txCount, modification))
        {
            return 4;
        }

        return txCount;
    }

    private static bool IsSupportedDeviceType(uint deviceType, uint txCount, uint modification) =>
        deviceType switch
        {
            DeviceTypeAutonom => txCount is >= 3 and <= 5,
            DeviceTypeLwd => txCount is >= 3 and <= 5,
            DeviceTypeCartograph => IsCartographLwd4Tx(deviceType, txCount, modification),
            _ => false
        };

    private static bool IsCartographLwd4Tx(uint deviceType, uint txCount, uint modification) =>
        deviceType == DeviceTypeCartograph &&
        modification == CartographLwd4TxModification &&
        txCount is 4 or 5;
}
