; ============================================================================
; Кольца - Inno Setup installer script
;
; Перед сборкой установщика выполните публикацию приложения:
;   dotnet build ..\LOOP_NEXT.sln -c Release -p:Platform=x64 --no-restore
;
; Готовый установщик:
;   installer\Output\Koltsa-Setup-<version>.exe
;
; Для автообновления загрузите на сервер два файла:
;   http://ivanbohg.beget.tech/FilesForProgramVisualisation/koltsa-version.txt
;   http://ivanbohg.beget.tech/FilesForProgramVisualisation/Koltsa-Setup-<version>.exe
;
; Формат koltsa-version.txt:
;   1.0.5
;   Koltsa-Setup-1.0.5.exe
; ============================================================================

#define MyAppId            "{{9A9B3C77-7E18-4F2D-B7E6-2F77770DA5A8}"
#define MyAppName          "Кольца"
#define MyAppVersion       "1.0.5"
#define MyAppPublisher     'ООО НПФ "АМК ГОРИЗОНТ"'
#define MyAppExeName       "Кольца.exe"
#define MyAppDescription   "Приложение поверки и калибровки зондов с RLC-кольцом"
#define MyAppCopyright     'Copyright (C) 2026 ООО НПФ "АМК ГОРИЗОНТ"'

#define SourceDir          ".."
#define PublishDir         SourceDir + "\src\LOOP.Next.App\bin\x64\Release\net10.0-windows10.0.26100.0\win-x64"
#define AppIconPath        SourceDir + "\src\LOOP.Next.App\Assets\AppIcon.ico"

[Setup]
AppId={#MyAppId}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppCopyright={#MyAppCopyright}

PrivilegesRequired=lowest
PrivilegesRequiredOverridesAllowed=dialog

DefaultDirName={localappdata}\Programs\Koltsa
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=auto
DisableDirPage=no
AllowNoIcons=no

ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
MinVersion=10.0

CloseApplications=force
CloseApplicationsFilter={#MyAppExeName}
RestartApplications=no

OutputDir=Output
OutputBaseFilename=Koltsa-Setup-{#MyAppVersion}
SetupIconFile={#AppIconPath}

Compression=lzma2/max
SolidCompression=yes
LZMAUseSeparateProcess=yes

WizardStyle=modern
ShowLanguageDialog=auto
UninstallDisplayName={#MyAppName} {#MyAppVersion}
UninstallDisplayIcon={app}\{#MyAppExeName}

VersionInfoVersion={#MyAppVersion}
VersionInfoProductName={#MyAppName}
VersionInfoProductVersion={#MyAppVersion}
VersionInfoCompany={#MyAppPublisher}
VersionInfoCopyright={#MyAppCopyright}
VersionInfoDescription={#MyAppName} Setup

[Languages]
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "{#PublishDir}\*"; DestDir: "{app}"; Excludes: "LoopDeviceLog.txt,*.log,*.tmp,*.pdb"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; IconFilename: "{app}\Assets\AppIcon.ico"; Comment: "{#MyAppDescription}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; IconFilename: "{app}\Assets\AppIcon.ico"; Comment: "{#MyAppDescription}"

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: filesandordirs; Name: "{app}\*.log"
Type: filesandordirs; Name: "{app}\*.tmp"
