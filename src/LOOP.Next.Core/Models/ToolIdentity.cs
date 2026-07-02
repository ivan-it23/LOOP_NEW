namespace LoopNext.Core.Models;

// Идентификация прибора по сигнатуре. Точный перенос decode_tool_signature (function.cpp:15).
// low20 = младшие 20 бит сигнатуры; toolCode = low20/1000 = XYZ:
//   X (тип): low20/100000, Y (число Tx): (low20%100000)/10000, Z (мод): (low20%10000)/1000.
// Для картографа в режиме LWD 4tx модификация 9 является отдельной сигнатурной веткой:
// встречаются коды 349/359, но оба должны работать как 4 Tx без изменения структуры кадра.
public readonly record struct ToolIdentity(
    uint RawSignature,
    uint Low20,
    uint DeviceType,
    uint TransmitterCount,
    uint Modification,
    uint Number,
    bool IsValid)
{
    // Типы прибора кодируются первой цифрой toolCode (variable.h:49-50).
    public const uint DeviceTypeAutonom = 1;
    public const uint DeviceTypeLwd = 2;
    public const uint DeviceTypeCartograph = 3;
    public const uint CartographLwd4TxModification = 9;

    public bool IsCartographLwd4TxMode =>
        DeviceType == DeviceTypeCartograph &&
        Modification == CartographLwd4TxModification &&
        TransmitterCount == 4;

    public static ToolIdentity Decode(uint signature)
    {
        var low20 = (signature << 12) >> 12;
        var deviceType = low20 / 100000u;
        var txCount = (low20 % 100000u) / 10000u;
        var modification = (low20 % 10000u) / 1000u;
        var number = low20 % 1000u;
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
            low20,
            deviceType,
            normalizedTxCount,
            modification,
            number,
            isValid);
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
