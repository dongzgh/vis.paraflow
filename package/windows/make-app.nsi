# Define program name variable
!define PROGRAM_NAME "visParaflow"

# Define version variable
!define PROGRAM_VERSION "1.0.0"

# Define publisher name variable
!define PUBLISHER_NAME "visParaflow"

# Name of the installer
Outfile "../../setup/windows/${PROGRAM_NAME}-${PROGRAM_VERSION}.exe"

# Set installer icon
Icon ".\app.ico"
UninstallIcon ".\app.ico"

# Default installation directory
InstallDir $PROGRAMFILES64\${PROGRAM_NAME}

# Use registry key to remember the installation directory
InstallDirRegKey HKCU "Software\${PROGRAM_NAME}\${PROGRAM_VERSION}" "Install_Dir"

# Page settings
Page directory
Page instfiles

# Section for installation
Section "Install"
  # Set output path to the installation directory
  SetOutPath $INSTDIR

  # Include files to be installed
  File /r /x "*.pdb" "..\..\deploy\windows\*.*"

  # Write the installation path to the registry
  WriteRegStr HKCU "Software\${PROGRAM_NAME}\${PROGRAM_VERSION}" "Install_Dir" "$INSTDIR"

  # Create a shortcut in the Start Programs menu
  CreateShortCut "$SMPROGRAMS\${PROGRAM_NAME}.lnk" "$INSTDIR\${PROGRAM_NAME}.exe"

  # Create a shortcut on the Public Desktop
  CreateShortcut "$DESKTOP\${PROGRAM_NAME}.lnk" "$INSTDIR\${PROGRAM_NAME}.exe"

  # Write uninstall information to the registry
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "DisplayName" "${PROGRAM_NAME}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "UninstallString" "$INSTDIR\uninstall.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "InstallLocation" "$INSTDIR"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "DisplayVersion" "${PROGRAM_VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "Publisher" "${PUBLISHER_NAME}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "DisplayIcon" "$INSTDIR\${PROGRAM_NAME}.exe"
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "NoRepair" 1

  # Write the uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

# Section for uninstallation
Section "Uninstall"
  # Remove the shortcut from the Start Programs menu
  Delete "$SMPROGRAMS\${PROGRAM_NAME}.lnk"

  # Remove the shortcut from the Public Desktop
  Delete "$DESKTOP\${PROGRAM_NAME}.lnk"

  # Remove the registry entries
  DeleteRegKey HKCU "Software\${PROGRAM_NAME}\${PROGRAM_VERSION}"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}"

  # Remove the installation directory
  RMDir /r "$INSTDIR"
SectionEnd