using System.Text.Json;
using System.Text.Json.Serialization;
using LoopNext.Core.Logging;

namespace LOOP_Next_App.Services;

public sealed class UserSettings
{
    public string? ToolPort { get; set; }

    public string? RingPort { get; set; }

    /// <summary>
    /// Устаревшее поле версии 1.1.1. После миграции очищается.
    /// </summary>
    public int? WindowScalePosition { get; set; }

    /// <summary>
    /// Относительный масштаб окна. Допустимы только 80, 90, 100, 110 и 120.
    /// </summary>
    public int? WindowScalePercent { get; set; }

    /// <summary>
    /// Версия смысла полей масштаба. Отсутствие версии означает настройку,
    /// созданную до введения DPI-нормализованного масштаба.
    /// </summary>
    public int? WindowScaleSettingsVersion { get; set; }
}

public static class UserSettingsStore
{
    private static readonly object SyncRoot = new();

    private static string SettingsDirectory => Path.Combine(
        Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),
        "LOOP_NEXT");

    private static string SettingsPath => Path.Combine(SettingsDirectory, "settings.json");

    public static UserSettings Load()
    {
        lock (SyncRoot)
        {
            return LoadCore();
        }
    }

    public static void Save(UserSettings settings)
    {
        ArgumentNullException.ThrowIfNull(settings);

        lock (SyncRoot)
        {
            SaveCore(settings);
        }
    }

    /// <summary>
    /// Атомарно изменяет только нужные поля актуальной копии настроек. Это не
    /// позволяет кешу одного компонента затереть масштаб или COM-порт,
    /// сохранённый другим компонентом.
    /// </summary>
    public static void Update(Action<UserSettings> update)
    {
        ArgumentNullException.ThrowIfNull(update);

        lock (SyncRoot)
        {
            var settings = LoadCore();
            update(settings);
            SaveCore(settings);
        }
    }

    private static UserSettings LoadCore()
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

    private static void SaveCore(UserSettings settings)
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
