cmake_minimum_required(VERSION 3.16)

# Set the project variables.
project(visParaflow VERSION 1.0.0 LANGUAGES CXX DESCRIPTION "A workflow modeling and script generation tool.")

# Set C++ standard.
set(CMAKE_CXX_STANDARD 20)

# Set the output directory for the build executables and libraries.
set(CMAKE_RUNTIME_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIR ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/lib)

# Ensure the output directory is the same for all configurations
foreach(CONFIGURATION ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${CONFIGURATION} CONFIGURATION_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIGURATION_UPPER} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIGURATION_UPPER} ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIGURATION_UPPER} ${CMAKE_BINARY_DIR}/lib)
endforeach()

# Set include directories.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find Qt6 component for Regular Expression.
find_package(Qt6 COMPONENTS Core Widgets Svg REQUIRED)

# Set automoc and other Qt specific flags.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Add the resource file
qt_add_resources(RESOURCES "res/resources.qrc")

# Collect the source files.
file(GLOB Header_FILES "include/*.h")
file(GLOB Implementation_FILES "src/*.cpp")

# Set the build type.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
endif()

# Set the executable target.
if (WIN32)
    message(STATUS "Building on Windows")

    # Set windows specific resources.
    set(RESOURCE_WIN32 "res/resources.rc")

    # Add the executable target.
    add_executable(${CMAKE_PROJECT_NAME}
        WIN32
        ${Implementation_FILES}
        ${Header_FILES}
        ${RESOURCES}
        ${RESOURCE_WIN32}
    )
elseif (APPLE)
    message(STATUS "Building on macOS")

    # Set macOS specific resources.
    set(PLIST_MACOS "package/macos/Info.plist")
    set(ICNS_MACOS "package/macos/app.icns")

    # Set the bundle properties.
    set(CMAKE_MACOSX_BUNDLE TRUE)  # Enable macOS app bundle creation

    # Add the executable target.
    add_executable(${CMAKE_PROJECT_NAME}
        MACOSX_BUNDLE
        ${Implementation_FILES}
        ${Header_FILES}
        ${RESOURCES}
        ${ICNS_MACOS}
    )

    # Set the bundle icon
    set(MACOSX_BUNDLE_ICON_FILE "${CMAKE_SOURCE_DIR}/${ICNS_MACOS}")
    set_source_files_properties(${MACOSX_BUNDLE_ICON_FILE} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    # Set the bundle Info.plist
    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/${PLIST_MACOS}")

    # Include icon in the bundle
    target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${MACOSX_BUNDLE_ICON_FILE})
elseif (UNIX)
    message(STATUS "Building on Unix")

    # Add the executable target.
    add_executable(${CMAKE_PROJECT_NAME}
        ${Implementation_FILES}
        ${Header_FILES}
        ${RESOURCES}
    )
endif()

# Set link libraries.
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets Qt6::Svg)

# Set the install prefix.
if(WIN32)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/deploy/windows" CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
elseif(APPLE)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/deploy/macos" CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
elseif(UNIX)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/deploy/ubuntu" CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
endif()

# Install the executable target.
if(WIN32)
    install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}")
elseif (APPLE)
    install(TARGETS ${CMAKE_PROJECT_NAME} BUNDLE DESTINATION "${CMAKE_INSTALL_PREFIX}")
elseif(UNIX)
    install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()

# Install the system runtime libraries.
# The destination of system libraries can be customized as follows
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

# Set package targets.
if (WIN32)
    add_subdirectory(${CMAKE_SOURCE_DIR}/package/windows)
elseif (APPLE)
    add_subdirectory(${CMAKE_SOURCE_DIR}/package/macos)
elseif (UNIX)
    add_subdirectory(${CMAKE_SOURCE_DIR}/package/ubuntu)
endif()
