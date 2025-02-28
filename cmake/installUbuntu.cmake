# Set the install prefix.
if (NOT USE_CPACK)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/deploy/ubuntu" CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
else()
    set(CMAKE_INSTALL_PREFIX "" CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
endif()

# Install target RUNTIME.
install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME_DEPENDENCY_SET RunTimeDependencySet RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/opt/${CMAKE_PROJECT_NAME}/bin/" )

# Install target RUNTIME dependencies.
install(RUNTIME_DEPENDENCY_SET RunTimeDependencySet DESTINATION "${CMAKE_INSTALL_PREFIX}/opt/${CMAKE_PROJECT_NAME}/lib/")

# Get Qt6 lib directory.
get_target_property(Qt6_LIB_DIR Qt6::Core LOCATION)
get_filename_component(Qt6_LIB_DIR ${Qt6_LIB_DIR} DIRECTORY)

# Install Qt6 missing libraries.
file(GLOB LIBWAYLAND "${Qt6_LIB_DIR}/libQt6Wayland*")
install(FILES ${LIBWAYLAND} DESTINATION "${CMAKE_INSTALL_PREFIX}/opt/${CMAKE_PROJECT_NAME}/lib/")
file(GLOB LIBXCB_FILES "${Qt6_LIB_DIR}/libQt6Xcb*")
install(FILES ${LIBXCB} DESTINATION "${CMAKE_INSTALL_PREFIX}/opt/${CMAKE_PROJECT_NAME}/lib/")

# Get Qt6 plugins directory.
set(Qt6_PLUGINS_DIR "${Qt6_LIB_DIR}/../plugins")

# Install the Qt6 plugins.
LIST(APPEND Qt6_PLUGINS_LIST
    "xcbglintegrations"
    "wayland-shell-integration"
    "wayland-graphics-integration-client"
    "wayland-decoration-client"
    "tls"
    "networkinformation"
    "imageformats"
    "iconengines"
    "generic"
    "platforms"
)
foreach(PLUGIN ${Qt6_PLUGINS_LIST})
    install(DIRECTORY "${Qt6_PLUGINS_DIR}/${PLUGIN}" 
    DESTINATION "${CMAKE_INSTALL_PREFIX}/opt/${CMAKE_PROJECT_NAME}/plugins" 
    USE_SOURCE_PERMISSIONS 
    PATTERN "*.debug" EXCLUDE)
endforeach()

# Install app script file
# Cope app script file directly to usr/local/bin sometimes run into permission issues,
# so we copy it to /opt/${CMAKE_PROJECT_NAME} first
install(PROGRAMS "${CMAKE_SOURCE_DIR}/package/ubuntu/app.sh" DESTINATION "${CMAKE_INSTALL_PREFIX}/opt/${CMAKE_PROJECT_NAME}/")

# Install app desktop and icon file
install(FILES "${CMAKE_SOURCE_DIR}/package/ubuntu/app.desktop" DESTINATION "${CMAKE_INSTALL_PREFIX}/usr/share/applications/" RENAME ${CMAKE_PROJECT_NAME}.desktop)
install(FILES "${CMAKE_SOURCE_DIR}/package/ubuntu/app.svg" DESTINATION "${CMAKE_INSTALL_PREFIX}/usr/share/icons/hicolor/48x48/apps/" RENAME ${CMAKE_PROJECT_NAME}.svg)
