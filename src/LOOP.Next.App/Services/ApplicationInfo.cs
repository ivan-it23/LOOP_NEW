using System.Reflection;

namespace LOOP_Next_App.Services;

internal static class ApplicationInfo
{
    public const string DisplayName = "Кольца";
    public const string Publisher = "ООО НПФ \"АМК ГОРИЗОНТ\"";
    public const string UpdateHost = "ivanbohg.beget.tech";
    public const string UpdateBaseUrl = "http://ivanbohg.beget.tech/FilesForProgramVisualisation/";
    public const string VersionFileUrl = UpdateBaseUrl + "koltsa-version.txt";
    public const string UserAgent = "KoltsaUpdater/1.0";

    public static Version CurrentVersion { get; } =
        Assembly.GetExecutingAssembly().GetName().Version ?? new Version(1, 0, 0);

    public static string CurrentVersionText =>
        $"{CurrentVersion.Major}.{CurrentVersion.Minor}.{CurrentVersion.Build}";
}
