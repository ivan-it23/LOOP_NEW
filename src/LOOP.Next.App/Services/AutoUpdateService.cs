using System.Diagnostics;
using System.Net.Http;
using System.Security.Cryptography;
using LoopNext.Core.Logging;

namespace LOOP_Next_App.Services;

/// <summary>
/// Проверка версии на сайте, загрузка установщика и запуск обновления.
///
/// Ввод-вывод отделён от разбора манифеста: правила формата, сравнение версий и
/// аргументы установщика лежат в <see cref="UpdateManifest" /> и проверяются
/// автономно.
/// </summary>
public static class AutoUpdateService
{
    /// <summary>
    /// Короткий тайм-аут на чтение `koltsa-version.txt`: файл занимает десятки
    /// байт, и при недоступном сайте проверка не должна задерживать запуск.
    /// </summary>
    private static readonly TimeSpan VersionCheckTimeout = TimeSpan.FromSeconds(10);

    /// <summary>
    /// Клиент проверки версии и клиент загрузки разделены намеренно.
    ///
    /// `HttpClient.Timeout` ограничивает всю операцию целиком, включая чтение
    /// тела ответа. Прежний общий клиент с тайм-аутом 8 секунд не мог скачать
    /// установщик размером около 60 МБ ни на одном канале медленнее 8 МБ/с:
    /// загрузка обрывалась по тайм-ауту, и обновление молча не происходило.
    /// У клиента загрузки тайм-аут снят, а прерывание выполняется токеном.
    /// </summary>
    private static readonly HttpClient VersionHttp = new() { Timeout = VersionCheckTimeout };

    private static readonly HttpClient DownloadHttp = new() { Timeout = Timeout.InfiniteTimeSpan };

    public static string SetupLogPath => Path.Combine(Path.GetTempPath(), "koltsa_setup.log");

    /// <summary>
    /// Читает манифест обновления. Сетевые и форматные ошибки возвращаются как
    /// результат, а не исключение: проверка обновлений не должна ломать запуск.
    /// </summary>
    public static async Task<UpdateCheckResult> CheckAsync(CancellationToken cancellationToken = default)
    {
        try
        {
            using var request = new HttpRequestMessage(HttpMethod.Get, ApplicationInfo.VersionFileUrl);
            request.Headers.UserAgent.ParseAdd(ApplicationInfo.UserAgent);

            using var response = await VersionHttp
                .SendAsync(request, HttpCompletionOption.ResponseContentRead, cancellationToken)
                .ConfigureAwait(false);
            response.EnsureSuccessStatusCode();

            var body = await response.Content.ReadAsStringAsync(cancellationToken).ConfigureAwait(false);

            if (!UpdateManifest.TryParse(body, out var manifest, out var parseError) || manifest is null)
            {
                AppLogger.WriteDiagnostic("APP_UPDATE", "MANIFEST_INVALID", $"error={parseError}");
                return UpdateCheckResult.Failed(parseError ?? "Некорректный файл версии.");
            }

            var isNewer = UpdateManifest.IsNewerVersion(manifest.Version, ApplicationInfo.CurrentVersion);

            AppLogger.WriteDiagnostic(
                "APP_UPDATE",
                "MANIFEST_READ",
                $"remote_version={manifest.VersionText}|current_version={ApplicationInfo.CurrentVersionText}|installer={manifest.InstallerFileName}|has_sha256={manifest.Sha256 is not null}|update_available={isNewer}");

            return UpdateCheckResult.Succeeded(manifest, isNewer);
        }
        catch (OperationCanceledException) when (!cancellationToken.IsCancellationRequested)
        {
            AppLogger.WriteDiagnostic("APP_UPDATE", "MANIFEST_TIMEOUT");
            return UpdateCheckResult.Failed("Сервер обновлений не ответил вовремя.");
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Ошибка проверки обновлений");
            return UpdateCheckResult.Failed($"Не удалось подключиться к серверу обновлений: {ex.Message}");
        }
    }

    /// <summary>
    /// Скачивает установщик во временный каталог и проверяет его до запуска.
    /// </summary>
    public static async Task<UpdateDownloadResult> DownloadAsync(
        UpdateManifest manifest,
        IProgress<double>? progress = null,
        CancellationToken cancellationToken = default)
    {
        ArgumentNullException.ThrowIfNull(manifest);

        var localPath = Path.Combine(Path.GetTempPath(), UpdateManifest.GetDownloadFileName(manifest));

        try
        {
            var downloadUri = manifest.BuildDownloadUri(ApplicationInfo.UpdateBaseUrl);
            AppLogger.WriteDiagnostic("APP_UPDATE", "DOWNLOAD_BEGIN", $"url={downloadUri}|target={localPath}");

            using var request = new HttpRequestMessage(HttpMethod.Get, downloadUri);
            request.Headers.UserAgent.ParseAdd(ApplicationInfo.UserAgent);

            using var response = await DownloadHttp
                .SendAsync(request, HttpCompletionOption.ResponseHeadersRead, cancellationToken)
                .ConfigureAwait(false);
            response.EnsureSuccessStatusCode();

            var expectedLength = response.Content.Headers.ContentLength;

            await using (var remote = await response.Content.ReadAsStreamAsync(cancellationToken).ConfigureAwait(false))
            await using (var local = new FileStream(localPath, FileMode.Create, FileAccess.Write, FileShare.None))
            {
                var buffer = new byte[81920];
                long copied = 0;
                int read;
                while ((read = await remote.ReadAsync(buffer, cancellationToken).ConfigureAwait(false)) > 0)
                {
                    await local.WriteAsync(buffer.AsMemory(0, read), cancellationToken).ConfigureAwait(false);
                    copied += read;
                    if (expectedLength is > 0)
                    {
                        progress?.Report(Math.Clamp((double)copied / expectedLength.Value, 0.0, 1.0));
                    }
                }
            }

            var actualLength = new FileInfo(localPath).Length;
            if (expectedLength is > 0 && actualLength != expectedLength.Value)
            {
                return FailDownload(localPath, $"Файл скачан не полностью: {actualLength} из {expectedLength.Value} байт.");
            }

            if (!IsWindowsExecutable(localPath))
            {
                return FailDownload(localPath, "Скачанный файл не является установщиком Windows.");
            }

            if (manifest.Sha256 is { } expectedHash)
            {
                var actualHash = ComputeSha256(localPath);
                if (!string.Equals(actualHash, expectedHash, StringComparison.OrdinalIgnoreCase))
                {
                    AppLogger.WriteDiagnostic(
                        "APP_UPDATE",
                        "DOWNLOAD_HASH_MISMATCH",
                        $"expected={expectedHash}|actual={actualHash}");
                    return FailDownload(
                        localPath,
                        "Контрольная сумма скачанного установщика не совпала с указанной на сервере. Установка отменена.");
                }

                AppLogger.WriteDiagnostic("APP_UPDATE", "DOWNLOAD_HASH_VERIFIED", $"sha256={actualHash}");
            }

            RemoveMarkOfTheWeb(localPath);

            AppLogger.WriteDiagnostic("APP_UPDATE", "DOWNLOAD_COMPLETED", $"path={localPath}|bytes={actualLength}");
            return UpdateDownloadResult.Succeeded(localPath);
        }
        catch (OperationCanceledException)
        {
            TryDelete(localPath);
            throw;
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Ошибка скачивания обновления");
            return FailDownload(localPath, $"Не удалось скачать обновление: {ex.Message}");
        }
    }

    /// <summary>
    /// Запускает установщик и сообщает, началась ли установка.
    ///
    /// Промежуточный `.cmd`-сценарий убран намеренно. Он ждал завершения
    /// процесса и затем перезапускал приложение строкой
    /// `if exist "…\Кольца.exe" start "" "…\Кольца.exe"`. Имя исполняемого файла
    /// содержит кириллицу, сценарий сохранялся в UTF-8 с BOM, а `cmd.exe`
    /// разбирает `.cmd` в кодовой странице консоли: путь превращался в мусор,
    /// `if exist` не срабатывал, и приложение после обновления не открывалось.
    /// Теперь возврат приложения выполняет сам установщик записью `[Run]` по
    /// флагу `/RESTARTAPP=1`, а закрытие работающих копий берёт на себя
    /// `/CLOSEAPPLICATIONS`.
    /// </summary>
    public static bool StartInstaller(string installerPath)
    {
        try
        {
            var arguments = UpdateManifest.BuildSilentInstallArguments(SetupLogPath);

            AppLogger.WriteDiagnostic(
                "APP_UPDATE",
                "INSTALLER_START",
                $"path={installerPath}|arguments={arguments}|setup_log={SetupLogPath}");

            var startInfo = new ProcessStartInfo
            {
                FileName = installerPath,
                Arguments = arguments,
                // UseShellExecute=true запускает установщик как обычный двойной
                // клик пользователя: корректно отрабатывают ассоциации, запрос
                // повышения прав и рабочий каталог.
                UseShellExecute = true,
                WorkingDirectory = Path.GetTempPath()
            };

            var process = Process.Start(startInfo);
            if (process is null)
            {
                AppLogger.WriteDiagnostic("APP_UPDATE", "INSTALLER_START_FAILED", $"reason=process_is_null");
                return false;
            }

            return true;
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Не удалось запустить установщик обновления");
            return false;
        }
    }

    /// <summary>
    /// Удаляет установщики, оставшиеся во временном каталоге от прерванных
    /// обновлений. Раньше уборкой занимался `.cmd`-сценарий, который до неё мог
    /// и не дойти, а файл занимает около 60 МБ.
    /// </summary>
    public static void CleanupStaleDownloads()
    {
        try
        {
            var temp = Path.GetTempPath();
            foreach (var file in Directory.EnumerateFiles(temp, UpdateManifest.DownloadFilePrefix + "*.exe"))
            {
                try
                {
                    // Файл текущего обновления мог быть запущен минуту назад,
                    // поэтому свежие загрузки не трогаем.
                    if (DateTime.UtcNow - File.GetLastWriteTimeUtc(file) > TimeSpan.FromHours(1))
                    {
                        File.Delete(file);
                        AppLogger.WriteDiagnostic("APP_UPDATE", "STALE_DOWNLOAD_REMOVED", $"path={file}");
                    }
                }
                catch (Exception ex)
                {
                    AppLogger.Write(ex, "Не удалось удалить старый файл обновления");
                }
            }
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Не удалось просмотреть временный каталог обновлений");
        }
    }

    private static UpdateDownloadResult FailDownload(string path, string message)
    {
        TryDelete(path);
        return UpdateDownloadResult.Failed(message);
    }

    private static void TryDelete(string path)
    {
        try
        {
            if (File.Exists(path))
            {
                File.Delete(path);
            }
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Не удалось удалить временный файл обновления");
        }
    }

    private static string ComputeSha256(string path)
    {
        using var stream = File.OpenRead(path);
        return Convert.ToHexString(SHA256.HashData(stream));
    }

    private static bool IsWindowsExecutable(string path)
    {
        try
        {
            using var stream = File.OpenRead(path);
            if (stream.Length < 1024)
            {
                return false;
            }

            Span<byte> header = stackalloc byte[2];
            return stream.Read(header) == 2 && header[0] == (byte)'M' && header[1] == (byte)'Z';
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Не удалось проверить формат скачанного установщика");
            return false;
        }
    }

    /// <summary>
    /// Снимает метку загрузки из интернета, если она была проставлена. Файл
    /// скачивается напрямую и метки обычно не получает, но при работе через
    /// прокси или антивирус она появляется, а вместе с ней и запрос SmartScreen
    /// посреди тихой установки.
    /// </summary>
    private static void RemoveMarkOfTheWeb(string path)
    {
        try
        {
            var zoneStream = path + ":Zone.Identifier";
            if (File.Exists(zoneStream))
            {
                File.Delete(zoneStream);
                AppLogger.WriteDiagnostic("APP_UPDATE", "MARK_OF_THE_WEB_REMOVED", $"path={path}");
            }
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Не удалось снять метку загрузки у установщика");
        }
    }
}

public sealed record UpdateCheckResult(
    UpdateManifest? Manifest,
    bool UpdateAvailable,
    string? ErrorMessage)
{
    public bool HasError => ErrorMessage is not null;

    public static UpdateCheckResult Succeeded(UpdateManifest manifest, bool updateAvailable) =>
        new(manifest, updateAvailable, null);

    public static UpdateCheckResult Failed(string errorMessage) =>
        new(null, false, errorMessage);
}

public sealed record UpdateDownloadResult(string? InstallerPath, string? ErrorMessage)
{
    public bool Success => InstallerPath is not null;

    public static UpdateDownloadResult Succeeded(string installerPath) => new(installerPath, null);

    public static UpdateDownloadResult Failed(string errorMessage) => new(null, errorMessage);
}
