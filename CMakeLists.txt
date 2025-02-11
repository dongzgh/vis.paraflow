cmake_minimum_required(VERSION 3.10)

project(visParaflow)

# Set C++ standard.
set(CMAKE_CXX_STANDARD 17)

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

# Add the resource file
qt_add_resources(RESOURCES "res/resources.qrc")
set(RESOURCE_FILES "res/resources.rc")

if (APPLE)
    message(STATUS "Building on macOS")

    # Set the bundle properties.
    set(CMAKE_MACOSX_BUNDLE TRUE)  # Enable macOS app bundle creation

    # Add the executable target.
    add_executable(visParaflow
        MACOSX_BUNDLE
        ${Implementation_FILES}
        ${Header_FILES}
        ${RESOURCES}
        ${RESOURCE_FILES}
        res/workflow.icns  # Add the icon file to the bundle
    )

    # Set the bundle properties.
    set_target_properties(visParaflow PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.vis.Paraflow"
        MACOSX_BUNDLE_BUNDLE_NAME "visParaflow"
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/Info.plist
        RESOURCE "${CMAKE_SOURCE_DIR}/res/workflow.icns" # Set the icon for the app bundle
    )
elseif (UNIX)
    message(STATUS "Building on Unix")

    # Add the executable target.
    add_executable(visParaflow
        ${Implementation_FILES}
        ${Header_FILES}
        ${RESOURCES}
        ${RESOURCE_FILES}
    )
elseif (WIN32)
    message(STATUS "Building on Windows")

    # Add the executable target.
    add_executable(visParaflow
        WIN32
        ${Implementation_FILES}
        ${Header_FILES}
        ${RESOURCES}
        ${RESOURCE_FILES}
    )
endif()

# Set link libraries.
target_link_libraries(visParaflow Qt6::Core Qt6::Widgets Qt6::Svg)