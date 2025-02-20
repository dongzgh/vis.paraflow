# Define version variable
!define VERSION "1.0.0"

# Define program name variable
!define PROGRAM_NAME "visParaflow"

# Define publisher name variable
!define PUBLISHER_NAME "visParaflow"

# Name of the installer
Outfile "${PROGRAM_NAME}-${VERSION}-setup.exe"

# Set the installer icon
Icon "..\res\workflow.ico"

# Default installation directory
InstallDir $PROGRAMFILES64\${PROGRAM_NAME}\${VERSION}

# Use registry key to remember the installation directory
InstallDirRegKey HKCU "Software\${PROGRAM_NAME}\${VERSION}" "Install_Dir"

# Page settings
Page directory
Page instfiles
Page uninstConfirm

# Section for the installation
Section "Install"
  # Set output path to the installation directory
  SetOutPath $INSTDIR

  # Include files to be installed
  File /r /x "*.pdb" "..\build\release\bin\*.*"

  # Write the installation path to the registry
  WriteRegStr HKCU "Software\${PROGRAM_NAME}\${VERSION}" "Install_Dir" "$INSTDIR"

  # Create a shortcut in the Start Programs menu
  CreateDirectory "$SMPROGRAMS\${PROGRAM_NAME}"
  CreateShortCut "$SMPROGRAMS\${PROGRAM_NAME}\${PROGRAM_NAME}.lnk" "$INSTDIR\${PROGRAM_NAME}.exe"

  # Write uninstall information to the registry
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "DisplayName" "${PROGRAM_NAME} ${VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "UninstallString" "$INSTDIR\uninstall.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "InstallLocation" "$INSTDIR"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "DisplayVersion" "${VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "Publisher" "${PUBLISHER_NAME}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "DisplayIcon" "$INSTDIR\workflow.ico"
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "NoRepair" 1

  # Write the uninstaller
  WriteUninstaller "$INSTDIR\uninstall.exe"
SectionEnd

# Section for uninstallation
Section "Uninstall"
  # Remove the installed files
  Delete "$INSTDIR\*.*"

  # Remove the shortcut from the Start Programs menu
  Delete "$SMPROGRAMS\${PROGRAM_NAME}\${PROGRAM_NAME}.lnk"
  RMDir "$SMPROGRAMS\${PROGRAM_NAME}"

  # Remove the registry entries
  DeleteRegKey HKCU "Software\${PROGRAM_NAME}\${VERSION}"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}"

  # Remove the installation directory
  RMDir "$INSTDIR"
SectionEnd