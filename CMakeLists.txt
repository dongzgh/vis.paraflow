cmake_minimum_required(VERSION 3.16)

# Set the project variables.
project(visParaflow)
set(VERSION 1.0.0)
set(DESCRIPTION "A workflow modeling and script generation tool.")
set(AUTHOR "Dong Zhao")

# Set C++ standard.
set(CMAKE_CXX_STANDARD 20)

# Set automoc and other Qt specific flags.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set include directories.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set the output directory for the build executables and libraries.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Ensure the output directory is the same for all configurations
foreach(CONFIGURATION ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${CONFIGURATION} CONFIGURATION_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIGURATION_UPPER} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIGURATION_UPPER} ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIGURATION_UPPER} ${CMAKE_BINARY_DIR}/lib)
endforeach()

# Find Qt6 component for Regular Expression.
find_package(Qt6 COMPONENTS Core Widgets Svg REQUIRED)

# Collect the source files.
file(GLOB Header_FILES "include/*.h")
file(GLOB Implementation_FILES "src/*.cpp")

# Set up standard qt project.
qt_standard_project_setup()

# Add the resource file
qt_add_resources(RESOURCES "res/resources.qrc")

if (APPLE)
    message(STATUS "Building on macOS")

    # Set macOS specific resources.
    set(PLIST_MACOS "deploy/macos/Info.plist")
    set(ICNS_MACOS "deploy/macos/app.icns")

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
elseif (WIN32)
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
endif()

# Set link libraries.
target_link_libraries(${CMAKE_PROJECT_NAME} Qt6::Core Qt6::Widgets Qt6::Svg)

# Set CPack targets.
if (APPLE)
    add_subdirectory(${CMAKE_SOURCE_DIR}/deploy/macos)
elseif (UNIX)
    add_subdirectory(${CMAKE_SOURCE_DIR}/deploy/linux)
elseif (WIN32)
    add_subdirectory(${CMAKE_SOURCE_DIR}/deploy/windows)
endif()
