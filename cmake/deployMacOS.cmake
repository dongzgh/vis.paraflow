# Set the install prefix.
if (NOT USE_CPACK)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/deploy/macos" CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
else()
    set(CMAKE_INSTALL_PREFIX "" CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
endif()

# Install target RUNTIME.
install(TARGETS ${CMAKE_PROJECT_NAME} BUNDLE DESTINATION "${CMAKE_INSTALL_PREFIX}/Applications/")

# Install target RUNTIME dependencies.
install(CODE "execute_process(COMMAND macdeployqt ${CMAKE_INSTALL_PREFIX}/Applications/${CMAKE_PROJECT_NAME}.app -verbose=1)")
