using System.Globalization;

namespace LoopNext.Hardware;

// Чистый разбор ответов кольца. Вынесен отдельно, чтобы совместимость старой
// и новой прошивок можно было проверять без физического COM-устройства.
public static class RingProtocolParser
{
    public static bool IsUnknownCommand(string? response) =>
        response?.Trim().StartsWith(RingProtocolConstants.UnknownReplyPrefix, StringComparison.Ordinal) == true;

    public static bool TryParseSignature(string? response, out uint signature)
    {
        signature = 0;
        if (string.IsNullOrWhiteSpace(response))
        {
            return false;
        }

        var value = response.Trim();
        foreach (var prefix in new[] { "SGN=", "SG=" })
        {
            if (value.StartsWith(prefix, StringComparison.Ordinal) &&
                uint.TryParse(value[prefix.Length..], NumberStyles.None, CultureInfo.InvariantCulture, out signature))
            {
                return true;
            }
        }

        return false;
    }

    public static bool TryParseLoopParameterCell(int index, string? response, out RingLoopParameterCell cell)
    {
        cell = new RingLoopParameterCell(index, 0f, 0f, false);
        if (string.IsNullOrWhiteSpace(response))
        {
            return false;
        }

        var parts = response.Trim().Split(' ', StringSplitOptions.RemoveEmptyEntries);
        var valueOffset = 0;
        if (parts.Length == 4 && string.Equals(parts[0], "READ", StringComparison.Ordinal))
        {
            valueOffset = 1;
        }
        else if (parts.Length != 3)
        {
            return false;
        }

        if (!string.Equals(parts[valueOffset], $"Z{index}", StringComparison.Ordinal) ||
            !TryParseFiniteFloat(parts[valueOffset + 1], out var resistance) ||
            !TryParseFiniteFloat(parts[valueOffset + 2], out var reactance))
        {
            return false;
        }

        cell = new RingLoopParameterCell(index, resistance, reactance, true);
        return true;
    }

    /// <summary>
    /// Ответ кольца о пустой ячейке: <c>Z8 -nan -nan</c>.
    /// </summary>
    /// <remarks>
    /// Стёртая флеш-память читается как <c>0xFFFFFFFF</c>, то есть NaN, и
    /// прошивка печатает его через <c>%f</c>. Такой ответ - не сбой связи:
    /// кольцо ответило, просто в ячейку ничего не записано.
    /// </remarks>
    public static bool IsEmptyLoopParameterCell(int index, string? response)
    {
        if (string.IsNullOrWhiteSpace(response))
        {
            return false;
        }

        var parts = response.Trim().Split(' ', StringSplitOptions.RemoveEmptyEntries);
        var valueOffset = parts.Length == 4 && string.Equals(parts[0], "READ", StringComparison.Ordinal) ? 1 : 0;
        if (parts.Length != valueOffset + 3 ||
            !string.Equals(parts[valueOffset], $"Z{index}", StringComparison.Ordinal))
        {
            return false;
        }

        return IsNonFiniteToken(parts[valueOffset + 1]) && IsNonFiniteToken(parts[valueOffset + 2]);
    }

    private static bool IsNonFiniteToken(string token)
    {
        var text = token.TrimStart('+', '-');
        return text.Equals("nan", StringComparison.OrdinalIgnoreCase) ||
               text.Equals("inf", StringComparison.OrdinalIgnoreCase) ||
               text.Equals("infinity", StringComparison.OrdinalIgnoreCase);
    }

    public static bool IsWriteLoopParameterReply(int index, string? response) =>
        IsOptionalExclamationReply($"WRITE Z{index} OK", response);

    public static bool IsResistanceReply(int index, string? response) =>
        IsOptionalExclamationReply($"RES={index} OK", response);

    public static bool IsCapacitanceReply(int index, string? response) =>
        IsOptionalExclamationReply($"CAP={index} OK", response);

    public static bool IsEraseReply(string? response) =>
        IsOptionalExclamationReply("ERASE OK", response);

    public static bool IsWriteSignatureReply(uint signature, string? response) =>
        IsOptionalExclamationReply($"WRITE SGN={signature} OK", response);

    private static bool IsOptionalExclamationReply(string expected, string? response)
    {
        var value = response?.Trim();
        return string.Equals(value, expected, StringComparison.Ordinal) ||
               string.Equals(value, expected + "!", StringComparison.Ordinal);
    }

    private static bool TryParseFiniteFloat(string text, out float value)
    {
        return float.TryParse(text, NumberStyles.Float, CultureInfo.InvariantCulture, out value) &&
               float.IsFinite(value);
    }
}
