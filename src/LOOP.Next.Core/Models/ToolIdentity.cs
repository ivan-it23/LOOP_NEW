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

    /// <summary>
    /// Картограф, работающий как LWD с четырьмя передатчиками.
    /// </summary>
    /// <remarks>
    /// Модификация в признак не входит: у новой прошивки она не участвует ни в
    /// одном решении, а у старой её роль уже учтена при разборе сигнатуры -
    /// туда непригодный Картограф не дойдёт, потому что окажется невалидным.
    /// Проверка валидности здесь обязательна: неподдержанный прибор не должен
    /// уводить обмен в исторический транспорт фиксированной длины.
    /// </remarks>
    public bool IsCartographLwd4TxMode =>
        IsValid &&
        DeviceType == DeviceTypeCartograph &&
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
        var normalizedTxCount = NormalizeTransmitterCount(deviceType, txCount, modification, frameSize);

        // Валидность определяется до клампинга n_tx, как в эталонном коде.
        var isValid = IsSupportedDeviceType(deviceType, txCount, modification, frameSize)
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

    // Нормализация числа передатчиков существует только ради старого Картографа:
    // в парке встречались коды 349 и 359, и оба работали как 4 Tx. У новой
    // прошивки нормализовать нечего - она берёт N_Tx из сигнатуры буквально
    // (get_sonde_id: N_Tx = (signature % 100000) / 10000) и столько передатчиков
    // и опрашивает. Подогнать 5 под 4 здесь означало бы читать пять каналов как
    // четыре.
    private static uint NormalizeTransmitterCount(
        uint deviceType,
        uint txCount,
        uint modification,
        int frameSize)
    {
        if (frameSize == LegacyFrameSize && IsLegacyCartographLwd4Tx(deviceType, txCount, modification))
        {
            return 4;
        }

        return txCount;
    }

    /// <summary>
    /// Прибор описан поддерживаемым сочетанием типа и числа передатчиков.
    /// </summary>
    /// <remarks>
    /// У старой прошивки Картограф принимался только в режиме LWD 4tx, и этот
    /// режим помечался модификацией 9.
    ///
    /// Новая прошивка общая: Картограф в режиме LWD перепрошит тем же файлом,
    /// что Автономный и LWD (<c>LWD_1_DDS_ATT_UES (2).c</c>). Её
    /// <c>get_sonde_id</c> читает тип и число передатчиков, а модификацию в
    /// выборе ветки не использует вовсе - никакой особой обработки Картографа в
    /// ней нет. Поэтому для нового поколения тип прибора перестаёт что-либо
    /// менять, и Картограф принимается на тех же условиях, что LWD.
    /// </remarks>
    private static bool IsSupportedDeviceType(
        uint deviceType,
        uint txCount,
        uint modification,
        int frameSize) =>
        deviceType switch
        {
            DeviceTypeAutonom => txCount is >= 3 and <= 5,
            DeviceTypeLwd => txCount is >= 3 and <= 5,
            DeviceTypeCartograph => frameSize == LegacyFrameSize
                ? IsLegacyCartographLwd4Tx(deviceType, txCount, modification)
                : txCount is >= 3 and <= 5,
            _ => false
        };

    private static bool IsLegacyCartographLwd4Tx(uint deviceType, uint txCount, uint modification) =>
        deviceType == DeviceTypeCartograph &&
        modification == CartographLwd4TxModification &&
        txCount is 4 or 5;
}
