# Установщик приложения "Кольца"

Установщик собирается через Inno Setup 6 из Release-папки WinUI-приложения. Текущая версия приложения и установщика: `1.0.6`.

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

## Проверенный релиз 1.0.6

Локальный комплект выпуска:

```text
installer\Output\Koltsa-Setup-1.0.6.exe
installer\Output\Koltsa-Setup-1.0.6.exe.sha256
installer\Output\koltsa-version.txt
```

Проверенные метаданные:

- версия приложения: `1.0.6.0`;
- версия установщика: `1.0.6`;
- размер установщика: `62088594` байт;
- SHA-256: `95870CB2E9DDCEDD598434C7BDADC256EFC77C84EDF5B127B820D91DE371BDE8`;
- Release x64: `0` предупреждений, `0` ошибок;
- protocol checks: `18/18 passed`;
- smoke-запуск: окно `Кольца` создано и отвечало системе.

Сценарий `[Files]` исключает из поставки `LoopDeviceLog.txt`, `*.log`, `*.tmp` и `*.pdb`. Runtime-журнал испытаний из `src\LoopDeviceLog.txt` не должен попадать пользователям.

## Автообновление

Приложение проверяет обновления при запуске по адресу:

```text
http://ivanbohg.beget.tech/FilesForProgramVisualisation/koltsa-version.txt
```

Формат `koltsa-version.txt`:

```text
1.0.6
Koltsa-Setup-1.0.6.exe
```

В репозитории поддерживается готовый файл `installer\koltsa-version.txt` с текущей версией. При выпуске новой версии его нужно обновлять синхронно с `LOOP.Next.App.csproj` и `installer\setup.iss`.

Установщик из второй строки должен лежать в той же папке:

```text
http://ivanbohg.beget.tech/FilesForProgramVisualisation/Koltsa-Setup-1.0.6.exe
```

Если версия на сервере выше текущей версии приложения, пользователь получает предложение скачать и установить обновление. Обновление запускается через скачанный Inno Setup установщик, чтобы обновлялись не только `Кольца.exe`, но и все DLL, ресурсы WinUI и ассеты приложения.

## Чек-лист публикации

1. Убедиться, что версии в `LOOP.Next.App.csproj`, `setup.iss` и `koltsa-version.txt` совпадают.
2. Собрать Release x64 и Inno Setup установщик.
3. Проверить имя, версию и SHA-256 полученного файла.
4. Загрузить `Koltsa-Setup-1.0.6.exe` в `FilesForProgramVisualisation`.
5. После установщика загрузить актуальный `koltsa-version.txt` в ту же папку.
6. Проверить прямую доступность обоих URL.
7. Запустить установленную предыдущую версию и убедиться, что она предлагает обновление до `1.0.6`.

На момент актуализации документации локальный установщик собран и проверен, но загрузка файлов на сервер и живая проверка автообновления не подтверждены.
