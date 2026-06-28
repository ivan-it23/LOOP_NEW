using System.Text.Json;
using System.Text.Json.Serialization;
using LoopNext.Core.Logging;

namespace LOOP_Next_App.Services;

public sealed class UserSettings
{
    public string? ToolPort { get; set; }

    public string? RingPort { get; set; }
}

public static class UserSettingsStore
{
    private static string SettingsDirectory => Path.Combine(
        Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),
        "LOOP_NEXT");

    private static string SettingsPath => Path.Combine(SettingsDirectory, "settings.json");

    public static UserSettings Load()
    {
        try
        {
            if (!File.Exists(SettingsPath))
            {
                return new UserSettings();
            }

            var json = File.ReadAllText(SettingsPath);
            return JsonSerializer.Deserialize(json, UserSettingsJsonContext.Default.UserSettings) ?? new UserSettings();
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Не удалось прочитать пользовательские настройки");
            return new UserSettings();
        }
    }

    public static void Save(UserSettings settings)
    {
        try
        {
            Directory.CreateDirectory(SettingsDirectory);
            var json = JsonSerializer.Serialize(settings, UserSettingsJsonContext.Default.UserSettings);
            File.WriteAllText(SettingsPath, json);
        }
        catch (Exception ex)
        {
            AppLogger.Write(ex, "Не удалось сохранить пользовательские настройки");
        }
    }
}

[JsonSourceGenerationOptions(WriteIndented = true)]
[JsonSerializable(typeof(UserSettings))]
internal partial class UserSettingsJsonContext : JsonSerializerContext;
