; Inno Setup script for R00M13A-OS.
; The GitHub "Build and Package Installer" workflow fills GAMEDIRECTORY and REDIST
; before compiling this script. GAMEDIRECTORY mirrors the repo layout the game
; expects at runtime (it loads "../Assets", "../Configs" and "../Engine/..."
; relative to the RoombaRampage folder it runs from):
;
;   GAMEDIRECTORY\RoombaRampage\R00M13A-OS.exe (+ DLLs)
;   GAMEDIRECTORY\Assets\
;   GAMEDIRECTORY\Configs\
;   GAMEDIRECTORY\Engine\Dependencies\mono\lib\
;   GAMEDIRECTORY\Engine\ScriptLibrary\GameScript\ScriptCoreDLL\GameScript.dll

#define AppName "R00M13A-OS"
#define ExeName "R00M13A-OS.exe"
#ifndef AppVersion
  #define AppVersion "1.0.0"
#endif

[Setup]
; Uniquely identifies this application; do not reuse in other installers.
AppId={{D33634F6-0480-488B-9777-D7E3D36A1791}
AppName={#AppName}
AppVersion={#AppVersion}
AppVerName={#AppName} {#AppVersion}
AppPublisher=DigiPen Institute of Technology
AppPublisherURL=http://www.digipen.edu/
AppSupportURL=http://www.digipen.edu/

; {autopf} resolves to a per-user folder when installing without admin rights,
; which keeps the install folder writable (the game writes LogFile.txt there).
DefaultDirName={autopf}\DigiPen\{#AppName}
DefaultGroupName=DigiPen\{#AppName}
PrivilegesRequired=lowest
PrivilegesRequiredOverridesAllowed=dialog
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible

OutputDir=.\INSTALLER
OutputBaseFilename=R00M13A-OS_Setup

LicenseFile=INSTALLERFILES\DigiPen_EULA.txt
SetupIconFile=.\INSTALLERFILES\SetupIcon.ico
UninstallDisplayIcon={app}\Icon.ico

Compression=lzma2
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"

[Files]
Source: .\GAMEDIRECTORY\*; DestDir: {app}; Flags: ignoreversion recursesubdirs createallsubdirs
Source: .\REDIST\VC_redist.x64.exe; DestDir: {tmp}; Flags: ignoreversion deleteafterinstall

[Icons]
Name: {group}\{#AppName}; Filename: {app}\RoombaRampage\{#ExeName}; WorkingDir: {app}\RoombaRampage; IconFilename: "{app}\Icon.ico"
Name: {group}\{cm:UninstallProgram,{#AppName}}; Filename: {uninstallexe}; IconFilename: "{app}\Icon.ico"
Name: {autodesktop}\{#AppName}; Filename: {app}\RoombaRampage\{#ExeName}; WorkingDir: {app}\RoombaRampage; IconFilename: "{app}\Icon.ico"; Tasks: desktopicon

[Run]
Filename: {tmp}\VC_redist.x64.exe; Parameters: "/install /quiet /norestart"; StatusMsg: "Installing Visual C++ Redistributable..."; Flags: shellexec waituntilterminated
Filename: {app}\RoombaRampage\{#ExeName}; WorkingDir: {app}\RoombaRampage; Description: {cm:LaunchProgram,{#AppName}}; Flags: nowait postinstall skipifsilent
