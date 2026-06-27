namespace LoopNext.Hardware;

// Константы текстового протокола кольца (loop).
// Перенос из BT_ON и Loop_work (WorkForm.h:6431, 6488).
public static class RingProtocolConstants
{
    public const int BaudRate = 9600;
    public const int TimeoutMs = 500;

    // Команда идентификации кольца и ожидаемый префикс ответа.
    public const string IdentifyCommand = "$\r\n";
    public const string ExpectedReplyPrefix = "Unknown";

    // Шаблоны команд установки сопротивления и ёмкости (Loop_work, WorkForm.h:6499-6500).
    public const string SetResistanceCommandFormat = "RES {0}\r\n";
    public const string SetCapacitanceCommandFormat = "CAP {0}\r\n";
    public const string SetResistanceReplyFormat = "RES={0} OK";
    public const string SetCapacitanceReplyFormat = "CAP={0} OK";

    public const int LoopParameterCellCount = 10;
    public const string ReadLoopParameterCommandFormat = "ZREAD {0}\r\n";
    public const string ReadLoopParameterReplyPrefixFormat = "Z{0} ";
    public const string WriteLoopParameterCommandFormat = "ZWRITE {0} {1} {2}\r\n";
    public const string WriteLoopParameterReplyPrefixFormat = "WRITE Z{0} OK!";
}
