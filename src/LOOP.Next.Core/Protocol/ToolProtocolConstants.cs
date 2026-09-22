namespace LoopNext.Core.Protocol;

public static class ToolProtocolConstants
{
    public const byte Key = 0x10;

    public const byte CmdGetData5Tx = 0x15;
    public const byte CmdGetData = 0x11;
    public const byte CmdGetSignature = 0x16;
    public const byte CmdGetMetrology = 0x13;

    // Запись метрологии. У старой прошивки отдельного кода нет: запись идёт тем
    // же 0x13, что и чтение, и отличается только длиной нагрузки. Новая прошивка
    // развела чтение и запись по разным кодам. Приложение метрологию не пишет;
    // константа заведена, чтобы будущая запись не ушла старым кодом в новый
    // прибор, у которого обработчика 0x13 с нагрузкой нет.
    public const byte CmdSetMetrologyExtended = 0x12;

    public const int ToolBaudRate = 125000;
    public const int SdrBaudRate = 115200;
    public const int ReadTimeoutMs = 2000;
    public const int WriteTimeoutMs = 2000;

    // Проба сигнатуры командой 0x16 отвечает сразу либо не отвечает вовсе:
    // Картограф в режиме LWD 4tx этой команды не имеет. Отдельный короткий
    // тайм-аут нужен, чтобы его подключение не ждало общие две секунды впустую.
    public const int SignatureProbeTimeoutMs = 500;

    // Нагрузка ответа состоит из двух служебных байт и самой структуры.
    public const int ToolServicePrefixLength = 2;

    public const int ToolPayloadLength = 242;
    public const int ToolPacketLength = ToolPayloadLength + 3;

    // Кадр измерений старой прошивки. Метрология занимает 240 байт в обоих
    // поколениях, поэтому её длина остаётся отдельной константой.
    public const int ToolDataSize = 240;
    public const int ToolMetrologySize = 240;

    // Кадр измерений новой прошивки: те же 240 байт, за которыми идут
    // амплитудные УЭС и симметризованные затухания. Общая часть не сдвинулась
    // ни на байт, поэтому разбор первых 240 байт одинаков для обоих поколений,
    // а расчёты этого приложения ведутся только по разности фаз и лежат в этой
    // общей части.
    public const int ExtendedToolDataSize = 320;

    public const int ExtendedToolPayloadLength = ToolServicePrefixLength + ExtendedToolDataSize;

    // Длина нагрузки ответа с кадром измерений указанного размера.
    public static int GetToolPayloadLength(int frameSize) => ToolServicePrefixLength + frameSize;

    // Размеры кадра, которые приложение умеет разбирать. Список расширяется
    // только вместе с поддержкой соответствующей структуры измерений.
    public static bool IsKnownFrameSize(int frameSize) =>
        frameSize == ToolDataSize || frameSize == ExtendedToolDataSize;

    public const int MaxTransmitters = 5;
    public const int FrequencyCount = 2;
    public const int MaxChannels = MaxTransmitters * FrequencyCount;
}
