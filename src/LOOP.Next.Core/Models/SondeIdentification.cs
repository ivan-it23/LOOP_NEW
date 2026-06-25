namespace LoopNext.Core.Models;

// Идентификация зонда для нижнего блока главного окна и окна поверки.
// Перенос разбора сигнатуры из MetroForm.h:1790-1810 и DeviceTypeDisplayName (WorkForm.h:5667).
public readonly record struct SondeIdentification(
    uint Signature,
    uint Serial,
    uint DeviceType,
    string DeviceTypeName,
    uint TransmitterCount,
    uint Modification,
    uint Number,
    uint RxPosition,
    uint PaletteSignature)
{
    // 32-битное двоичное представление сигнатуры (textBox_sign32, MetroForm.h:1805).
    public string SignatureBits => Convert.ToString(Signature, 2).PadLeft(32, '0');

    public static SondeIdentification FromMetrology(in GpToolMetrology metrology) =>
        Create(metrology.Signature, metrology.Serial, metrology.RxPosition);

    public static SondeIdentification Create(uint signature, uint serial, uint rxPosition)
    {
        var identity = ToolIdentity.Decode(signature);
        return new SondeIdentification(
            signature,
            serial,
            identity.DeviceType,
            BuildDeviceTypeName(identity.DeviceType, identity.TransmitterCount),
            identity.TransmitterCount,
            identity.Modification,
            identity.Number,
            rxPosition == 1 ? 1u : 0u,
            identity.Low20 / 1000u);
    }

    // Текст типа прибора для индикатора. Перенос DeviceTypeDisplayName (WorkForm.h:5667).
    private static string BuildDeviceTypeName(uint deviceType, uint transmitterCount)
    {
        var name = deviceType switch
        {
            ToolIdentity.DeviceTypeAutonom => "Автономный",
            ToolIdentity.DeviceTypeLwd => "LWD",
            _ => "Неизвестно"
        };

        return $"{name} {transmitterCount}TX";
    }
}
