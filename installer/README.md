# Установщик приложения "Кольца"

Установщик собирается через Inno Setup 6 из Release-папки WinUI-приложения. Текущая версия приложения и установщика: `1.0.4`.

## Сборка

1. Опубликовать приложение:

```powershell
dotnet build LOOP_NEXT.sln -c Release -p:Platform=x64 --no-restore
```

2. Собрать установщик:

```powershell
& "C:\Program Files (x86)\Inno Setup 6\ISCC.exe" installer\setup.iss
```

Готовый файл появится в `installer\Output\Koltsa-Setup-<version>.exe`.

Папка `installer\Output` является локальным релизным выводом и игнорируется git. Готовый `.exe` нужно загружать на хостинг или прикладывать к релизу вручную.

## Автообновление

Приложение проверяет обновления при запуске по адресу:

```text
http://ivanbohg.beget.tech/FilesForProgramVisualisation/koltsa-version.txt
```

Формат `koltsa-version.txt`:

```text
1.0.4
Koltsa-Setup-1.0.4.exe
```

В репозитории поддерживается готовый файл `installer\koltsa-version.txt` с текущей версией. При выпуске новой версии его нужно обновлять синхронно с `LOOP.Next.App.csproj` и `installer\setup.iss`.

Установщик из второй строки должен лежать в той же папке:

```text
http://ivanbohg.beget.tech/FilesForProgramVisualisation/Koltsa-Setup-1.0.4.exe
```

Если версия на сервере выше текущей версии приложения, пользователь получает предложение скачать и установить обновление. Обновление запускается через скачанный Inno Setup установщик, чтобы обновлялись не только `Кольца.exe`, но и все DLL, ресурсы WinUI и ассеты приложения.
