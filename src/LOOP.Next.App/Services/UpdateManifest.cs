using System.Globalization;

namespace LOOP_Next_App.Services;

/// <summary>
/// Разбор файла `koltsa-version.txt` и правила сравнения версий.
///
/// Формат файла:
/// <code>
/// 1.1.5
/// Koltsa-Setup-1.1.5.exe
/// 9F86D081884C7D659A2FEAA0C55AD015A3BF4F1B2B0B822CD15D6C15B0F00A08
/// </code>
///
/// Третья строка с SHA-256 необязательна: файлы предыдущих выпусков состоят из
/// двух строк и должны продолжать работать. Когда сумма указана, приложение
/// обязано сверить её до запуска установщика.
///
/// Логика вынесена в отдельный класс без ввода-вывода, чтобы её можно было
/// проверить автономно: разбор манифеста определяет, какой файл будет скачан и
/// запущен на компьютере пользователя, и ошибки здесь дороже прочих.
/// </summary>
public sealed record UpdateManifest(Version Version, string InstallerFileName, string? Sha256)
{
    public const int Sha256HexLength = 64;

    public static bool TryParse(string? content, out UpdateManifest? manifest, out string? errorMessage)
    {
        manifest = null;
        errorMessage = null;

        var lines = (content ?? string.Empty)
            .Split(new[] { "\r\n", "\n", "\r" }, StringSplitOptions.None)
            .Select(static line => line.Trim())
            .Where(static line => line.Length > 0)
            .ToArray();

        if (lines.Length < 2)
        {
            errorMessage = "Некорректный формат koltsa-version.txt: ожидаются номер версии и имя установщика.";
            return false;
        }

        if (!Version.TryParse(lines[0], out var version))
        {
            errorMessage = $"Некорректный номер версии в koltsa-version.txt: «{lines[0]}».";
            return false;
        }

        var installerFileName = lines[1];
        if (!IsSafeInstallerFileName(installerFileName))
        {
            errorMessage = "Во второй строке koltsa-version.txt должно быть имя exe-файла установщика без пути.";
            return false;
        }

        string? sha256 = null;
        if (lines.Length >= 3)
        {
            var candidate = lines[2].Replace(" ", string.Empty, StringComparison.Ordinal);
            if (!IsSha256Hex(candidate))
            {
                errorMessage = "Третья строка koltsa-version.txt должна содержать SHA-256 из 64 шестнадцатеричных цифр.";
                return false;
            }

            sha256 = candidate.ToUpperInvariant();
        }

        manifest = new UpdateManifest(version, installerFileName, sha256);
        return true;
    }

    /// <summary>
    /// Имя установщика приходит по незащищённому HTTP, поэтому проверяется строго.
    /// Без этой проверки строка вида `..\..\Startup\evil.exe` увела бы и адрес
    /// загрузки, и путь сохранения за пределы каталога назначения.
    /// </summary>
    public static bool IsSafeInstallerFileName(string? fileName)
    {
        if (string.IsNullOrWhiteSpace(fileName) ||
            !fileName.EndsWith(".exe", StringComparison.OrdinalIgnoreCase))
        {
            return false;
        }

        if (fileName.Contains("..", StringComparison.Ordinal) ||
            fileName.Contains('/', StringComparison.Ordinal) ||
            fileName.Contains('\\', StringComparison.Ordinal) ||
            fileName.Contains(':', StringComparison.Ordinal))
        {
            return false;
        }

        return fileName.IndexOfAny(Path.GetInvalidFileNameChars()) < 0 &&
               Path.GetFileName(fileName) == fileName;
    }

    public static bool IsSha256Hex(string? value)
    {
        if (value is null || value.Length != Sha256HexLength)
        {
            return false;
        }

        foreach (var symbol in value)
        {
            if (!Uri.IsHexDigit(symbol))
            {
                return false;
            }
        }

        return true;
    }

    /// <summary>
    /// Сравнение версий по трём разрядам.
    ///
    /// Сборка объявляет версию как `1.1.4.0`, а файл на сервере — как `1.1.4`.
    /// В .NET у `Version("1.1.4")` разряд Revision равен -1, поэтому прямое
    /// сравнение считает такие версии различными. Сравниваются только major,
    /// minor и build: именно в этих трёх разрядах ведётся нумерация выпусков.
    /// </summary>
    public static bool IsNewerVersion(Version remote, Version current) =>
        Normalize(remote) > Normalize(current);

    public static Version Normalize(Version version) =>
        new(version.Major, version.Minor, Math.Max(version.Build, 0));

    public static string FormatVersion(Version version) =>
        Normalize(version).ToString(3);

    /// <summary>
    /// Имя файла во временном каталоге. Префикс общий для всех загрузок, чтобы
    /// приложение могло убрать за собой недокачанные установщики прошлых сеансов.
    /// </summary>
    public const string DownloadFilePrefix = "koltsa_update_";

    public static string GetDownloadFileName(UpdateManifest manifest) =>
        DownloadFilePrefix + manifest.InstallerFileName;

    /// <summary>
    /// Аргументы тихой установки при автообновлении.
    ///
    /// `/CURRENTUSER` намеренно не передаётся: Inno Setup принимает эту команду
    /// только при `PrivilegesRequiredOverridesAllowed=commandline`, а иначе
    /// прекращает установку. Режим установки корректнее оставить самому Inno —
    /// он берёт его из записи предыдущей установки.
    ///
    /// `/RESTARTAPPLICATIONS` тоже не передаётся: возврат приложения после
    /// обновления выполняет запись `[Run]` установщика по флагу `/RESTARTAPP=1`,
    /// и второй механизм привёл бы к запуску двух копий.
    /// </summary>
    public static string BuildSilentInstallArguments(string setupLogPath) =>
        "/VERYSILENT /SUPPRESSMSGBOXES /NORESTART /CLOSEAPPLICATIONS /RESTARTAPP=1 " +
        "/LOG=\"" + setupLogPath + "\"";

    public string VersionText => FormatVersion(Version);

    public Uri BuildDownloadUri(string baseUrl) =>
        new(new Uri(baseUrl), Uri.EscapeDataString(InstallerFileName));

    public override string ToString() =>
        string.Create(CultureInfo.InvariantCulture, $"{VersionText} ({InstallerFileName})");
}
