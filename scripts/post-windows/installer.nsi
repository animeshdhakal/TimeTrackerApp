!include "MUI2.nsh"
!include "WinVer.nsh"

!define MUI_ICON "..\..\images\icon.ico"

NAME "TimeTracker"

OutFile "..\..\TimeTracker-Installer.exe"

InstallDir "$PROGRAMFILES\TimeTracker"

RequestExecutionLevel admin

!define MUI_ABORTWARNING

!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_LANGUAGE "English"

Section "Install Section" SecInstall
    SetOutPath "$INSTDIR"
    File /r "..\..\deployment\*"

    WriteUninstaller "$INSTDIR\uninstall.exe"

    createShortCut "$DESKTOP\TimeTracker.lnk" "$INSTDIR\TimeTracker.exe"
SectionEnd


Section "Uninstall"

  Delete "$INSTDIR\uninstall.exe"
  Delete "$DESKTOP\TimeTracker.lnk"
  RMDir /r "$SMPROGRAMS\TimeTracker\"
  RMDir /r $INSTDIR

  RMDir "$INSTDIR"

SectionEnd