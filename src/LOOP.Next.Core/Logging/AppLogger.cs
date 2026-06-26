using System.Globalization;

namespace LoopNext.Core.Logging;

// Минимальный файловый логгер для диагностики подключения и обмена.
// Портирован из старого проекта: WriteLog пишет в LoopDeviceLog.txt (UTF-8) и дублирует в отладчик.
public static class AppLogger
{
    private static readonly string LogFileName = "LoopDeviceLog.txt";

    private static string LogPath
    {
        get
        {
            var logDirectory = ResolveLogDirectory();
            return Path.Combine(logDirectory, LogFileName);
        }
    }

    private static string ResolveLogDirectory()
    {
        var appDirectory = AppContext.BaseDirectory;
        var current = new DirectoryInfo(appDirectory);
        while (current != null)
        {
            var srcCandidate = Path.Combine(current.FullName, "src");
            if (Directory.Exists(srcCandidate))
            {
                return srcCandidate;
            }

            if (current.Name.Equals("src", StringComparison.OrdinalIgnoreCase))
            {
                return current.FullName;
            }

            current = current.Parent;
        }

        return appDirectory;
    }

    private static readonly object Lock = new();

    public static void Write(string message)
    {
        var line = $"[{DateTime.Now:yyyy-MM-dd HH:mm:ss.fff}] {message}";
        lock (Lock)
        {
            try
            {
                File.AppendAllText(LogPath, line + Environment.NewLine);
            }
            catch
            {
                // Логирование не должно ломать рабочий процесс.
            }

            System.Diagnostics.Debug.WriteLine(line);
        }
    }

    public static void Write(Exception exception, string context)
    {
        Write($"{context}: {exception.GetType().Name}: {exception.Message}");
    }
}
