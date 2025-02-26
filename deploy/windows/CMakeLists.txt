# Include the InstallRequiredSystemLibraries module.
include(InstallRequiredSystemLibraries)

# Set the CPack variables.
set(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${CMAKE_PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${CMAKE_PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_VENDOR ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_SOURCE_DIR}/deploy/windows")

# Set the CPack generator to NSIS.
set(CPACK_GENERATOR NSIS)

# Set the CPack variables for the NSIS package.
set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/deploy/windows/app.ico")
set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/deploy/windows/app.ico")
set(CPACK_NSIS_DISPLAY_NAME ${CPACK_PACKAGE_NAME})
set(CPACK_NSIS_IGNORE_LICENSE_PAGE ON)
set(CPACK_NSIS_INSTALLED_ICON_NAME ${CMAKE_PROJECT_NAME}.exe)

# Get user public desktop directory
string(REPLACE "\\" "\\\\" PDESKTOP "$ENV{PUBLIC}\\Desktop")
# message(STATUS "PDESKTOP: ${PDESKTOP}")

# Get start menu directory
string(REPLACE "\\" "\\\\" SMPROGRAMS "$ENV{APPDATA}\\Microsoft\\Windows\\Start Menu\\Programs")
# message(STATUS "SMPROGRAMS: ${SMPROGRAMS}")

# Create shortcuts
set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
CreateShortCut '${SMPROGRAMS}\\\\${CMAKE_PROJECT_NAME}.lnk' '$INSTDIR\\\\${CMAKE_PROJECT_NAME}.exe'
CreateShortCut '${PDESKTOP}\\\\${CMAKE_PROJECT_NAME}.lnk' '$INSTDIR\\\\${CMAKE_PROJECT_NAME}.exe'")

# Delete shortcuts
set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
Delete '${SMPROGRAMS}\\\\${CMAKE_PROJECT_NAME}.lnk'
Delete '${PDESKTOP}\\\\${CMAKE_PROJECT_NAME}.lnk'")  

# Include the CPack module
include(CPack) # has to be after shortcuts instructions are set, otherwise the shortcuts won't be created

# Install the executable to the temporary directory
install(DIRECTORY "${CMAKE_INSTALL_PREFIX}/"
  DESTINATION /
  FILES_MATCHING PATTERN * PATTERN *.pdb EXCLUDE)
