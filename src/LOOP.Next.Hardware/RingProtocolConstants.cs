namespace LoopNext.Hardware;

// Константы текстового протокола кольца. Новая прошивка расширяет служебную
// память и добавляет SGN-команды, но рабочие R/C-команды остаются совместимыми.
public static class RingProtocolConstants
{
    public const int BaudRate = 9600;
    public const int TimeoutMs = 500;

    // Число попыток установки точки R/C с переоткрытием порта при сбое обмена.
    public const int SetPointMaxAttempts = 5;
    public const int MemoryCommandMaxAttempts = 3;
    public const int RetryDelayMs = 150;
    public const int ReopenDelayMs = 200;
    public const int CommandDelayMs = 50;

    public const int MinimumResistanceIndex = 0;
    public const int MaximumResistanceIndex = 8;
    public const int MinimumCapacitanceIndex = 0;
    public const int MaximumCapacitanceIndex = 10;

    // Первые 10 ячеек имеют прежний прикладной смысл и участвуют в расчётах.
    // Новая прошивка физически предоставляет 32 ячейки; Z10..Z31 сохраняются
    // приложением без изменения при штатной записи параметров поверки.
    public const int WorkingParameterCellCount = 10;
    public const int LegacyMemoryCellCount = 10;
    public const int ExtendedMemoryCellCount = 32;

    public const string LegacyIdentifyCommand = "$\r\n";
    public const string UnknownReplyPrefix = "Unknown";

    public const string ReadSignatureCommand = "SGNREAD\r\n";
    public const string WriteSignatureCommandFormat = "SGNWRITE {0}\r\n";
    // Прошивка из каталога zbox4 использует более раннее написание SGREAD/SGWRITE.
    // Приложение поддерживает оба варианта, чтобы сигнатуру можно было обслуживать
    // у всех выпущенных колец без перепрошивки только ради имени команды.
    public const string ReadCompatibleSignatureCommand = "SGREAD\r\n";
    public const string WriteCompatibleSignatureCommandFormat = "SGWRITE {0}\r\n";
    public const string EraseLoopParametersCommand = "ZERASE\r\n";

    public const string SetResistanceCommandFormat = "RES {0}\r\n";
    public const string SetCapacitanceCommandFormat = "CAP {0}\r\n";
    public const string SetResistanceReplyFormat = "RES={0} OK";
    public const string SetCapacitanceReplyFormat = "CAP={0} OK";

    public const string ReadLoopParameterCommandFormat = "ZREAD {0}\r\n";
    public const string WriteLoopParameterCommandFormat = "ZWRITE {0} {1} {2}\r\n";
}
