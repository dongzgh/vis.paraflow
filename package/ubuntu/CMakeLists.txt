# Set the CPack variables.
set(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${CMAKE_PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${CMAKE_PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_VENDOR ${CPACK_PACKAGE_NAME})
set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION})
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_SOURCE_DIR}/setup/ubuntu")

# Set the CPack generator to DEB.
set(CPACK_GENERATOR DEB)

# Set the CPack variables for the DEB package.
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${AUTHOR}")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.27)")
set(CPACK_DEBIAN_PACKAGE_SECTION utils)
set(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA
"${CMAKE_SOURCE_DIR}/package/ubuntu/debian/control"
"${CMAKE_SOURCE_DIR}/package/ubuntu/debian/postinst"
"${CMAKE_SOURCE_DIR}/package/ubuntu/debian/postrm")

# Install the executable to the temporary directory
install(PROGRAMS "${CMAKE_RUNTIME_OUTPUT_DIR}/${CPACK_PACKAGE_NAME}" DESTINATION /opt/${CPACK_PACKAGE_NAME}/bin/)

# Get Qt6 lib directory
get_target_property(Qt6_LIB_DIR Qt6::Core LOCATION)
get_filename_component(Qt6_LIB_DIR ${Qt6_LIB_DIR} DIRECTORY)

# Install the Qt6 libraries to the temporary directory
install(DIRECTORY "${Qt6_LIB_DIR}" DESTINATION /opt/${CPACK_PACKAGE_NAME}/ USE_SOURCE_PERMISSIONS)

# Get Qt6 plugins directory
set(Qt6_PLUGINS_DIR "${Qt6_LIB_DIR}/../plugins")

# Install the Qt6 plugins to the temporary directory
install(DIRECTORY "${Qt6_PLUGINS_DIR}/platforms" DESTINATION /opt/${CPACK_PACKAGE_NAME}/plugins/ USE_SOURCE_PERMISSIONS)

# Install app script file
# Cope app stript file directly to usr/local/bin sometimes run into permission issues,
# so we copy it to /opt/${CPACK_PACKAGE_NAME} first
install(PROGRAMS "${CMAKE_SOURCE_DIR}/package/ubuntu/app.sh" DESTINATION /opt/${CPACK_PACKAGE_NAME}/)

# Install app desktop and icon file
install(FILES "${CMAKE_SOURCE_DIR}/package/ubuntu/app.desktop" DESTINATION /usr/share/applications/ RENAME ${CPACK_PACKAGE_NAME}.desktop)
install(FILES "${CMAKE_SOURCE_DIR}/package/ubuntu/app.svg" DESTINATION /usr/share/icons/hicolor/48x48/apps/ RENAME ${CPACK_PACKAGE_NAME}.svg)

# Include the CPack module.
include(CPack)
