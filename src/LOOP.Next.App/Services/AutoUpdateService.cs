using System.Diagnostics;
using System.Net.Http;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using LoopNext.Core.Logging;

namespace LOOP_Next_App.Services;

internal static class AutoUpdateService
{
    private const int MessageBoxIconInformation = 0x00000040;
    private const int MessageBoxIconQuestion = 0x00000020;
    private const int MessageBoxIconWarning = 0x00000030;
    private const int MessageBoxIconError = 0x00000010;
    private const int MessageBoxOk = 0x00000000;
    private const int MessageBoxYesNo = 0x00000004;
    private const int DialogYes = 6;

    private static readonly HttpClient Http = new()
    {
        Timeout = TimeSpan.FromSeconds(8)
    };

    public static async Task RunUpdateCheckAsync(nint ownerWindow, bool silent)
    {
        try
        {
            var info = await CheckForUpdateAsync().ConfigureAwait(false);

            if (!string.IsNullOrWhiteSpace(info.ErrorMessage))
            {
                if (!silent)
                {
                    ShowMessage(ownerWindow, info.ErrorMessage, "Проверка обновлений", MessageBoxIconWarning | MessageBoxOk);
                }

                return;
            }

            if (!info.Available)
            {
                if (!silent)
                {
                    ShowMessage(
                        ownerWindow,
                        $"Установлена последняя версия ({ApplicationInfo.CurrentVersionText}).",
                        "Проверка обновлений",
                        MessageBoxIconInformation | MessageBoxOk);
                }

                return;
            }

            var prompt =
                $"Доступна новая версия: {info.RemoteVersion}\n" +
                $"Текущая версия: {ApplicationInfo.CurrentVersionText}\n\n" +
                "Скачать и установить обновление?";

            if (ShowMessage(ownerWindow, prompt, "Обновление доступно", MessageBoxIconQuestion | MessageBoxYesNo) != DialogYes)
            {
                return;
            }

            var installerPath = await DownloadUpdateAsync(info, ownerWindow).ConfigureAwait(false);
            if (string.IsNullOrWhiteSpace(installerPath))
            {
                return;
            }

            if (ApplyUpdate(installerPath))
            {
                ShowMessage(
                    ownerWindow,
                    "Обновление скачано. Приложение будет закрыто, установщик обновит файлы и запустит новую версию.",
                    "Обновление",
                    MessageBoxIconInformation | MessageBoxOk);

                App.DispatcherQueue.TryEnqueue(() => App.Window.Close());
            }
            else
            {
                ShowMessage(ownerWindow, "Не удалось запустить процесс обновления.", "Ошибка обновления", MessageBoxIconError | MessageBoxOk);
            }
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Ошибка автообновления");
            if (!silent)
            {
                ShowMessage(ownerWindow, ex.Message, "Ошибка обновления", MessageBoxIconError | MessageBoxOk);
            }
        }
    }

    private static async Task<UpdateInfo> CheckForUpdateAsync()
    {
        using var request = new HttpRequestMessage(HttpMethod.Get, ApplicationInfo.VersionFileUrl);
        request.Headers.UserAgent.ParseAdd(ApplicationInfo.UserAgent);

        string body;
        try
        {
            body = await SendStringRequestAsync(request).ConfigureAwait(false);
        }
        catch (Exception ex)
        {
            return UpdateInfo.Failed($"Не удалось подключиться к серверу обновлений: {ex.Message}");
        }

        var lines = body
            .Split(new[] { "\r\n", "\n" }, StringSplitOptions.None)
            .Select(static line => line.Trim())
            .Where(static line => line.Length > 0)
            .ToArray();

        if (lines.Length < 2)
        {
            return UpdateInfo.Failed("Некорректный формат version.txt. Ожидаются две строки: версия и имя установщика.");
        }

        if (!Version.TryParse(lines[0], out var remoteVersion))
        {
            return UpdateInfo.Failed("Некорректная версия в version.txt.");
        }

        var installerFileName = lines[1];
        if (!installerFileName.EndsWith(".exe", StringComparison.OrdinalIgnoreCase))
        {
            return UpdateInfo.Failed("Во второй строке version.txt должен быть указан exe-файл установщика.");
        }

        var downloadUrl = new Uri(new Uri(ApplicationInfo.UpdateBaseUrl), Uri.EscapeDataString(installerFileName));
        return new UpdateInfo(remoteVersion, installerFileName, downloadUrl, remoteVersion > ApplicationInfo.CurrentVersion, null);
    }

    private static async Task<string> SendStringRequestAsync(HttpRequestMessage request)
    {
        using var response = await Http.SendAsync(request, HttpCompletionOption.ResponseHeadersRead).ConfigureAwait(false);
        response.EnsureSuccessStatusCode();
        return await response.Content.ReadAsStringAsync().ConfigureAwait(false);
    }

    private static async Task<string?> DownloadUpdateAsync(UpdateInfo info, nint ownerWindow)
    {
        var localPath = Path.Combine(Path.GetTempPath(), "koltsa_update_" + info.InstallerFileName);

        try
        {
            using var request = new HttpRequestMessage(HttpMethod.Get, info.DownloadUrl);
            request.Headers.UserAgent.ParseAdd(ApplicationInfo.UserAgent);
            using var response = await Http.SendAsync(request, HttpCompletionOption.ResponseHeadersRead).ConfigureAwait(false);
            response.EnsureSuccessStatusCode();

            await using (var remote = await response.Content.ReadAsStreamAsync().ConfigureAwait(false))
            await using (var local = File.Create(localPath))
            {
                await remote.CopyToAsync(local).ConfigureAwait(false);
            }

            if (!IsValidPeFile(localPath))
            {
                File.Delete(localPath);
                ShowMessage(ownerWindow, "Скачанный файл повреждён или не является установщиком.", "Ошибка обновления", MessageBoxIconError | MessageBoxOk);
                return null;
            }

            RemoveMarkOfTheWeb(localPath);
            return localPath;
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Ошибка скачивания обновления");
            ShowMessage(ownerWindow, $"Не удалось скачать обновление: {ex.Message}", "Ошибка обновления", MessageBoxIconError | MessageBoxOk);
            return null;
        }
    }

    private static bool ApplyUpdate(string installerPath)
    {
        var currentExe = Environment.ProcessPath ?? Assembly.GetExecutingAssembly().Location;
        var currentPid = Environment.ProcessId;
        var scriptPath = Path.Combine(Path.GetTempPath(), "koltsa_updater.cmd");
        var logPath = Path.Combine(Path.GetTempPath(), "koltsa_updater.log");

        var installerArguments =
            "/VERYSILENT /SUPPRESSMSGBOXES /NORESTART /CURRENTUSER /CLOSEAPPLICATIONS /RESTARTAPPLICATIONS";

        var script = new StringBuilder();
        script.AppendLine("@echo off");
        script.AppendLine("chcp 65001 >NUL");
        script.AppendLine("setlocal EnableDelayedExpansion");
        script.AppendLine($"set \"LOG={logPath}\"");
        script.AppendLine($"(echo [%date% %time%] Koltsa updater started)> \"%LOG%\" 2>&1");
        script.AppendLine($"(echo Installer: {installerPath})>> \"%LOG%\" 2>&1");
        script.AppendLine($"(echo Current  : {currentExe})>> \"%LOG%\" 2>&1");
        script.AppendLine($"(echo PID      : {currentPid})>> \"%LOG%\" 2>&1");
        script.AppendLine(":wait_loop");
        script.AppendLine($"tasklist /FI \"PID eq {currentPid}\" 2>NUL | find \"{currentPid}\" >NUL");
        script.AppendLine("if not errorlevel 1 (");
        script.AppendLine("    timeout /t 1 /nobreak >NUL");
        script.AppendLine("    goto wait_loop");
        script.AppendLine(")");
        script.AppendLine("timeout /t 2 /nobreak >NUL");
        script.AppendLine($"\"{installerPath}\" {installerArguments} >> \"%LOG%\" 2>&1");
        script.AppendLine("(echo [%date% %time%] installer finished errorlevel=!errorlevel!)>> \"%LOG%\" 2>&1");
        script.AppendLine($"if exist \"{currentExe}\" start \"\" \"{currentExe}\"");
        script.AppendLine($"del \"{installerPath}\" >NUL 2>&1");
        script.AppendLine("(goto) 2>nul & del \"%~f0\"");

        File.WriteAllText(scriptPath, script.ToString(), new UTF8Encoding(encoderShouldEmitUTF8Identifier: true));

        var startInfo = new ProcessStartInfo
        {
            FileName = "cmd.exe",
            Arguments = $"/C \"{scriptPath}\"",
            CreateNoWindow = true,
            UseShellExecute = false,
            WindowStyle = ProcessWindowStyle.Hidden
        };

        Process.Start(startInfo);
        return true;
    }

    private static bool IsValidPeFile(string path)
    {
        using var stream = File.OpenRead(path);
        if (stream.Length < 1024)
        {
            return false;
        }

        Span<byte> header = stackalloc byte[2];
        return stream.Read(header) == 2 && header[0] == (byte)'M' && header[1] == (byte)'Z';
    }

    private static void RemoveMarkOfTheWeb(string path)
    {
        try
        {
            File.Delete(path + ":Zone.Identifier");
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Не удалось удалить Zone.Identifier у обновления");
        }
    }

    private static int ShowMessage(nint ownerWindow, string text, string caption, int type) =>
        MessageBoxW(ownerWindow, text, caption, type);

    [DllImport("user32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
    private static extern int MessageBoxW(nint hWnd, string text, string caption, int type);

    private sealed record UpdateInfo(
        Version RemoteVersion,
        string InstallerFileName,
        Uri DownloadUrl,
        bool Available,
        string? ErrorMessage)
    {
        public static UpdateInfo Failed(string error) =>
            new(new Version(0, 0, 0), string.Empty, new Uri(ApplicationInfo.UpdateBaseUrl), false, error);
    }
}
