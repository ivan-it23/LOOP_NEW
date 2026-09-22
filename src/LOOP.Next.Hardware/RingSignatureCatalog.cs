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
    // Универсальная мера, рассчитанная на максимальную конфигурацию Автономный 5 Tx.
    public const uint UniversalAutonomousSignature = 151;

    // Мера семейства LWD на 4 передатчика: 2 - тип прибора LWD, 4 - максимальное
    // число передатчиков, 1 - модификация. Приборы Автономный ею не обслуживаются.
    public const uint LwdFourTransmitterSignature = 241;

    public const string UniversalAutonomousSupportedToolTypes =
        "Автономный 4-5 TX, LWD 3-4 TX, Картограф->LWD-4TX";

    public const string LwdFourTransmitterSupportedToolTypes =
        "LWD 4 TX, Картограф->LWD-4TX";

    /// <summary>
    /// Сигнатура присутствует в каталоге, то есть её набор поддерживаемых приборов
    /// подтверждён. Для остальных значений приложение не приписывает кольцу
    /// неподтверждённые возможности и сохраняет режим совместимости.
    /// </summary>
    public static bool IsKnownSignature(uint? signature) =>
        signature is UniversalAutonomousSignature or LwdFourTransmitterSignature;

    public static string GetSupportedToolTypes(uint? signature) => signature switch
    {
        UniversalAutonomousSignature => UniversalAutonomousSupportedToolTypes,
        LwdFourTransmitterSignature => LwdFourTransmitterSupportedToolTypes,
        null => "—",
        _ => $"Не определены для сигнатуры {signature.Value.ToString(CultureInfo.InvariantCulture)}"
    };

    /// <summary>
    /// Возвращает профиль физической раскладки конденсаторов.
    /// </summary>
    /// <remarks>
    /// Кольцо 151 — универсальная мера на десять ёмкостей: банки C1..C5 и
    /// C6..C10. Прибор с тремя или четырьмя передатчиками занимает в каждом
    /// банке не все позиции, поэтому его высокочастотная группа сдвигается на
    /// начало второго банка.
    ///
    /// Кольцо 241 рассчитано только на четырёхпередатчиковый прибор и несёт
    /// **восемь** ёмкостей: банки C1..C4 и C5..C8. Сдвигать здесь нечего и
    /// некуда — девятой позиции у него физически нет, и команда <c>CAP 9</c>
    /// квитируется, но ни одного реле не переключает.
    ///
    /// Неизвестные и ещё не промаркированные кольца сохраняют прежнее поведение
    /// приложения, пока парк устройств переводится на сигнатуры.
    /// </remarks>
    public static RingCapacitanceMappingProfile GetCapacitanceMappingProfile(uint? signature) => signature switch
    {
        UniversalAutonomousSignature => RingCapacitanceMappingProfile.UniversalFiveTransmitter,
        LwdFourTransmitterSignature => RingCapacitanceMappingProfile.CompactFourTransmitter,
        _ => RingCapacitanceMappingProfile.CompatibilityFallback
    };

    public static RingToolCompatibility GetToolCompatibility(
        uint? ringSignature,
        in ToolIdentity toolIdentity)
    {
        if (!IsKnownSignature(ringSignature))
        {
            return RingToolCompatibility.Unverified;
        }

        if (!toolIdentity.IsValid)
        {
            return RingToolCompatibility.Unsupported;
        }

        var isSupported = ringSignature switch
        {
            UniversalAutonomousSignature => IsSupportedByUniversalAutonomous(in toolIdentity),
            LwdFourTransmitterSignature => IsSupportedByLwdFourTransmitter(in toolIdentity),
            _ => false
        };

        return isSupported
            ? RingToolCompatibility.Supported
            : RingToolCompatibility.Unsupported;
    }

    // Кольцо 151: универсальная мера семейства Автономный.
    private static bool IsSupportedByUniversalAutonomous(in ToolIdentity toolIdentity) =>
        toolIdentity.DeviceType switch
        {
            ToolIdentity.DeviceTypeAutonom => toolIdentity.TransmitterCount is 4 or 5,
            ToolIdentity.DeviceTypeLwd => toolIdentity.TransmitterCount is 3 or 4,
            ToolIdentity.DeviceTypeCartograph => toolIdentity.IsCartographLwd4TxMode,
            _ => false
        };

    // Кольцо 241: только приборы на четыре передатчика семейства LWD - собственно
    // LWD 24n и картограф 34n в режиме LWD 4tx. Номер модификации прибора не влияет
    // на совместимость. Автономный не поддерживается ни в одной конфигурации.
    private static bool IsSupportedByLwdFourTransmitter(in ToolIdentity toolIdentity) =>
        toolIdentity.DeviceType switch
        {
            ToolIdentity.DeviceTypeLwd => toolIdentity.TransmitterCount == 4,
            ToolIdentity.DeviceTypeCartograph => toolIdentity.IsCartographLwd4TxMode,
            _ => false
        };

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
    /// <summary>Кольцо без подтверждённой сигнатуры: прежнее поведение приложения.</summary>
    CompatibilityFallback,

    /// <summary>Универсальная мера на десять ёмкостей, банки C1..C5 и C6..C10.</summary>
    UniversalFiveTransmitter,

    /// <summary>Мера на восемь ёмкостей под четыре передатчика, банки C1..C4 и C5..C8.</summary>
    CompactFourTransmitter
}

public enum RingToolCompatibility
{
    Unverified,
    Supported,
    Unsupported
}
