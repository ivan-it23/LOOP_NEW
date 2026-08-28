using System.Globalization;

namespace LoopNext.Core.Logging;

// Минимальный файловый логгер для диагностики подключения и обмена.
// Портирован из старого проекта: WriteLog пишет в LoopDeviceLog.txt (UTF-8) и дублирует в отладчик.
public static class AppLogger
{
    private static readonly string LogFileName = "LoopDeviceLog.txt";
    private static readonly string SessionIdValue =
        $"{DateTime.Now:yyyyMMdd-HHmmssfff}-P{Environment.ProcessId}";
    private static readonly string LogPathValue = Path.Combine(ResolveLogDirectory(), LogFileName);
    private static long _lineSequence;
    private static long _operationSequence;

    private static string LogPath => LogPathValue;

    private static string ResolveLogDirectory()
    {
        var appDirectory = AppContext.BaseDirectory;
        var current = new DirectoryInfo(appDirectory);
        while (current != null)
        {
            var srcCandidate = Path.Combine(current.FullName, "src");
            if (Directory.Exists(srcCandidate) && IsWritable(srcCandidate))
            {
                return srcCandidate;
            }

            if (current.Name.Equals("src", StringComparison.OrdinalIgnoreCase) && IsWritable(current.FullName))
            {
                return current.FullName;
            }

            current = current.Parent;
        }

        if (IsWritable(appDirectory))
        {
            return appDirectory;
        }

        // Каталог установки может быть недоступен для записи: пользователь вправе
        // выбрать при установке путь вида Program Files, а обычной учётной записи
        // туда писать нельзя. Ошибки записи в журнал подавляются, поэтому без
        // этого запасного пути диагностика молча пропадала бы именно на тех
        // машинах, где она нужнее всего.
        var fallback = Path.Combine(
            Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),
            "LOOP_NEXT");

        try
        {
            Directory.CreateDirectory(fallback);
            return fallback;
        }
        catch
        {
            return appDirectory;
        }
    }

    private static bool IsWritable(string directory)
    {
        try
        {
            var probePath = Path.Combine(directory, $".loop_write_probe_{Environment.ProcessId}");
            using (var probe = new FileStream(probePath, FileMode.Create, FileAccess.Write, FileShare.None, 1, FileOptions.DeleteOnClose))
            {
                probe.WriteByte(0);
            }

            return true;
        }
        catch
        {
            return false;
        }
    }

    private static readonly object Lock = new();

    public static string SessionId => SessionIdValue;

    public static string LogFilePath => LogPath;

    public static string NextOperationId(string prefix)
    {
        var sequence = Interlocked.Increment(ref _operationSequence);
        return $"{prefix}-{sequence:D6}";
    }

    public static void Write(string message)
    {
        var sequence = Interlocked.Increment(ref _lineSequence);
        var line =
            $"[{DateTime.Now:yyyy-MM-dd HH:mm:ss.fff}] " +
            $"[session={SessionIdValue}] [seq={sequence:D8}] [thread={Environment.CurrentManagedThreadId}] {message}";
        lock (Lock)
        {
            try
            {
                File.AppendAllText(LogPath, line + Environment.NewLine);
            }
            catch
            {
                // Ошибки записи лога подавляются, чтобы диагностика не прерывала рабочий сценарий.
            }

            System.Diagnostics.Debug.WriteLine(line);
        }
    }

    public static void Write(Exception exception, string context)
    {
        Write($"{context}: {exception.GetType().Name}: {exception.Message}");
    }

    public static void WriteDiagnostic(string category, string eventName)
    {
        Write($"{category}|event={eventName}");
    }

    public static void WriteDiagnostic(string category, string eventName, FormattableString details)
    {
        Write($"{category}|event={eventName}|{details.ToString(CultureInfo.InvariantCulture)}");
    }

    public static void WriteDiagnostic(
        string category,
        string eventName,
        Exception exception,
        FormattableString? details = null)
    {
        var suffix = details is null
            ? string.Empty
            : "|" + details.ToString(CultureInfo.InvariantCulture);
        Write(
            $"{category}|event={eventName}|exception_type={exception.GetType().Name}|" +
            $"exception_message={Sanitize(exception.Message)}{suffix}");
    }

    private static string Sanitize(string value)
    {
        return value
            .Replace("\r", "\\r", StringComparison.Ordinal)
            .Replace("\n", "\\n", StringComparison.Ordinal)
            .Replace("|", "/", StringComparison.Ordinal);
    }
}
