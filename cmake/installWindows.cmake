# Set the install prefix.
if (NOT USE_CPACK)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/deploy/windows" CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
else()
    set(CMAKE_INSTALL_PREFIX "." CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
endif()

# Install RUNTIME targets
install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}")

# Install the system runtime libraries.
set (CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)
include (InstallRequiredSystemLibraries)
install (PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION "${CMAKE_INSTALL_PREFIX}")

# Generate a Qt deployment script to be executed at install time.
qt_generate_deploy_script(
    TARGET ${CMAKE_PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
    CONTENT "
qt_deploy_runtime_dependencies(
    EXECUTABLE \"${CMAKE_RUNTIME_OUTPUT_DIR}/$<TARGET_FILE_NAME:${CMAKE_PROJECT_NAME}>\"
    BIN_DIR \"${CMAKE_INSTALL_PREFIX}\" 
    PLUGINS_DIR \"${CMAKE_INSTALL_PREFIX}\"
    NO_TRANSLATIONS)
")

# Install the Qt deployment script.
install(SCRIPT ${deploy_script})
