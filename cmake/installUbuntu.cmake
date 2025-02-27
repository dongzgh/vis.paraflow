# Set the install prefix.
if (NOT USE_CPACK)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/deploy/ubuntu" CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
else()
    set(CMAKE_INSTALL_PREFIX "" CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
endif()

# Install RUNTIME targets and dependencies.
install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME_DEPENDENCIES DESTINATION "${CMAKE_INSTALL_PREFIX}/opt/${CMAKE_PROJECT_NAME}/bin/" )

# Install app script file
# Cope app script file directly to usr/local/bin sometimes run into permission issues,
# so we copy it to /opt/${CMAKE_PROJECT_NAME} first
install(PROGRAMS "${CMAKE_SOURCE_DIR}/package/ubuntu/app.sh" DESTINATION "${CMAKE_INSTALL_PREFIX}/opt/${CMAKE_PROJECT_NAME}/")

# Install app desktop and icon file
install(FILES "${CMAKE_SOURCE_DIR}/package/ubuntu/app.desktop" DESTINATION "${CMAKE_INSTALL_PREFIX}/usr/share/applications/" RENAME ${CMAKE_PROJECT_NAME}.desktop)
install(FILES "${CMAKE_SOURCE_DIR}/package/ubuntu/app.svg" DESTINATION "${CMAKE_INSTALL_PREFIX}/usr/share/icons/hicolor/48x48/apps/" RENAME ${CMAKE_PROJECT_NAME}.svg)
