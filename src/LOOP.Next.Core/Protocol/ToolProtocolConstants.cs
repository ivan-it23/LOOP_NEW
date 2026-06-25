namespace LoopNext.Core.Protocol;

public static class ToolProtocolConstants
{
    public const byte Key = 0x10;

    public const byte CmdGetData5Tx = 0x15;
    public const byte CmdGetData = 0x11;
    public const byte CmdGetSignature = 0x16;
    public const byte CmdGetMetrology = 0x13;
    public const byte CmdSetMetrology = 0x12;

    public const int ToolBaudRate = 125000;
    public const int SdrBaudRate = 115200;
    public const int ReadTimeoutMs = 2000;
    public const int WriteTimeoutMs = 2000;

    public const int ToolPayloadLength = 242;
    public const int ToolPacketLength = ToolPayloadLength + 3;
    public const int ToolDataSize = 240;
    public const int ToolMetrologySize = 240;

    public const int MaxTransmitters = 5;
    public const int FrequencyCount = 2;
    public const int MaxChannels = MaxTransmitters * FrequencyCount;
}
