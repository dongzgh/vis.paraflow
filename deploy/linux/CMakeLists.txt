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
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_SOURCE_DIR}/deploy/linux")

# Set the CPack generator to DEB.
set(CPACK_GENERATOR DEB)

# Set the CPack variables for the DEB package.
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${AUTHOR}")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.27)")
set(CPACK_DEBIAN_PACKAGE_SECTION utils)
set(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA
"${CMAKE_SOURCE_DIR}/deploy/linux/debian/control"
"${CMAKE_SOURCE_DIR}/deploy/linux/debian/postinst"
"${CMAKE_SOURCE_DIR}/deploy/linux/debian/postrm")

# Include the CPack module.
include(CPack)

# Install the executable to the temporary directory
install(PROGRAMS "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PRODUCT}" DESTINATION /opt/${PRODUCT}/bin/)

# Get Qt6 lib directory
get_target_property(Qt6_LIB_DIR Qt6::Core LOCATION)
get_filename_component(Qt6_LIB_DIR ${Qt6_LIB_DIR} DIRECTORY)

# Install the Qt6 libraries to the temporary directory
install(DIRECTORY "${Qt6_LIB_DIR}" DESTINATION /opt/${PRODUCT}/ USE_SOURCE_PERMISSIONS)

# Get Qt6 plugins directory
set(Qt6_PLUGINS_DIR "${Qt6_LIB_DIR}/../plugins")

# Install the Qt6 plugins to the temporary directory
install(DIRECTORY "${Qt6_PLUGINS_DIR}/platforms" DESTINATION /opt/${PRODUCT}/plugins/ USE_SOURCE_PERMISSIONS)

# Install app script file
# Cope app stript file directly to usr/local/bin sometimes run into permission issues,
# so we copy it to /opt/${PRODUCT} first
install(PROGRAMS "${CMAKE_SOURCE_DIR}/deploy/linux/app.sh" DESTINATION /opt/${PRODUCT}/)

# Install app desktop and icon file
install(FILES "${CMAKE_SOURCE_DIR}/deploy/linux/app.desktop" DESTINATION /usr/share/applications/ RENAME ${PRODUCT}.desktop)
install(FILES "${CMAKE_SOURCE_DIR}/deploy/linux/app.svg" DESTINATION /usr/share/icons/hicolor/48x48/apps/ RENAME ${PRODUCT}.svg)
