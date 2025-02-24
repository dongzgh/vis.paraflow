# Include the InstallRequiredSystemLibraries module.
include(InstallRequiredSystemLibraries)

# Set the CPack variables.
set(CPACK_PACKAGE_NAME ${PRODUCT})
set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR 1)
set(CPACK_PACKAGE_VERSION_MINOR 0)
set(CPACK_PACKAGE_VERSION_PATCH 0)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${DESCRIPTION}")
set(CPACK_PACKAGE_VENDOR ${PRODUCT})
set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION})
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_SOURCE_DIR}/deploy/macos")

# Set package generator to Apple Disk Image.
set(CPACK_GENERATOR DragNDrop)

# Include the CPack module.
include(CPack)

# Install the executable to the temporary directory
install(DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PRODUCT}.app" DESTINATION /Applications)
