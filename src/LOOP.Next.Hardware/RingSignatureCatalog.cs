using System.Globalization;
using LoopNext.Core.Models;

namespace LoopNext.Hardware;

/// <summary>
/// Правила пользовательского представления сигнатуры кольца. Трёхзначная
/// сигнатура кодирует семейство прибора, максимальное число передатчиков и
/// модификацию. Совместимость фиксируется по полной сигнатуре, чтобы приложение
/// не приписывало неизвестной модификации неподтверждённые возможности.
/// </summary>
public static class RingSignatureCatalog
{
    public const uint UniversalAutonomousSignature = 151;

    public const string UniversalAutonomousSupportedToolTypes =
        "Автономный 4-5 TX, LWD 3-4 TX, Картограф->LWD-4TX";

    public static string GetSupportedToolTypes(uint? signature) => signature switch
    {
        UniversalAutonomousSignature => UniversalAutonomousSupportedToolTypes,
        null => "—",
        _ => $"Не определены для сигнатуры {signature.Value.ToString(CultureInfo.InvariantCulture)}"
    };

    /// <summary>
    /// Возвращает профиль физической раскладки конденсаторов. Неизвестные и
    /// ещё не промаркированные кольца продолжают работать по прежнему алгоритму
    /// приложения, пока парк устройств переводится на сигнатуры.
    /// </summary>
    public static RingCapacitanceMappingProfile GetCapacitanceMappingProfile(uint? signature) =>
        signature == UniversalAutonomousSignature
            ? RingCapacitanceMappingProfile.UniversalFiveTransmitter
            : RingCapacitanceMappingProfile.CompatibilityFallback;

    public static RingToolCompatibility GetToolCompatibility(
        uint? ringSignature,
        in ToolIdentity toolIdentity)
    {
        if (ringSignature != UniversalAutonomousSignature)
        {
            return RingToolCompatibility.Unverified;
        }

        if (!toolIdentity.IsValid)
        {
            return RingToolCompatibility.Unsupported;
        }

        var isSupported = toolIdentity.DeviceType switch
        {
            ToolIdentity.DeviceTypeAutonom => toolIdentity.TransmitterCount is 4 or 5,
            ToolIdentity.DeviceTypeLwd => toolIdentity.TransmitterCount is 3 or 4,
            ToolIdentity.DeviceTypeCartograph => toolIdentity.IsCartographLwd4TxMode,
            _ => false
        };

        return isSupported
            ? RingToolCompatibility.Supported
            : RingToolCompatibility.Unsupported;
    }

    public static bool TryParseEditableSignature(
        string? text,
        out uint signature,
        out string? errorMessage)
    {
        signature = 0;
        errorMessage = null;
        var value = text?.Trim();
        if (string.IsNullOrWhiteSpace(value))
        {
            errorMessage = "Введите сигнатуру кольца.";
            return false;
        }

        if (value.Length != 3 ||
            !uint.TryParse(value, NumberStyles.None, CultureInfo.InvariantCulture, out signature) ||
            signature < 100 ||
            signature > 999)
        {
            errorMessage =
                "Сигнатура должна состоять ровно из трёх цифр: тип прибора, максимальное число TX и модификация. Например: 151.";
            signature = 0;
            return false;
        }

        return true;
    }
}

public enum RingCapacitanceMappingProfile
{
    CompatibilityFallback,
    UniversalFiveTransmitter
}

public enum RingToolCompatibility
{
    Unverified,
    Supported,
    Unsupported
}
