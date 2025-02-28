# Set the install prefix.
if (NOT USE_CPACK)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/deploy/macos" CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
else()
    set(CMAKE_INSTALL_PREFIX "" CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
endif()

# Install target RUNTIME.
install(DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIR}/${CMAKE_PROJECT_NAME}.app" DESTINATION "${CMAKE_INSTALL_PREFIX}/Applications")
