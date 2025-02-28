# Set the install prefix.
if (NOT USE_CPACK)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/deploy/macos" CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
else()
    set(CMAKE_INSTALL_PREFIX "" CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
endif()

# Use shell script to copy the application bundle.
# The application bundle installed by CMake can't be prorperly processed by macdeployqt6, use shell command instead.
install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory \"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.app\" \"${CMAKE_INSTALL_PREFIX}/${CMAKE_PROJECT_NAME}.app\")")
