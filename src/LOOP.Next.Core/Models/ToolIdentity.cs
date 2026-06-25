namespace LoopNext.Core.Models;

// Идентификация прибора по сигнатуре. Точный перенос decode_tool_signature (function.cpp:15).
// low20 = младшие 20 бит сигнатуры; toolCode = low20/1000 = XYZ:
//   X (тип): low20/100000, Y (число Tx): (low20%100000)/10000, Z (мод): (low20%10000)/1000.
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

    public static ToolIdentity Decode(uint signature)
    {
        var low20 = (signature << 12) >> 12;
        var deviceType = low20 / 100000u;
        var txCount = (low20 % 100000u) / 10000u;
        var modification = (low20 % 10000u) / 1000u;
        var number = low20 % 1000u;

        // Валидность определяется до клампинга n_tx, как в эталонном коде.
        var isValid = (deviceType == DeviceTypeAutonom || deviceType == DeviceTypeLwd)
                      && txCount is >= 3 and <= 5;

        if (txCount is < 3 or > 5)
        {
            txCount = 5;
        }

        return new ToolIdentity(
            signature,
            low20,
            deviceType,
            txCount,
            modification,
            number,
            isValid);
    }
}
