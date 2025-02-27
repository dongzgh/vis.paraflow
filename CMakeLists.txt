cmake_minimum_required(VERSION 3.16)

# Set the project variables.
project(visParaflow VERSION 1.0.0 LANGUAGES CXX DESCRIPTION "A workflow modeling and script generation tool.")
set(AUTHOR "Dong Zhao")

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

# Append local module path.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Build the application for the target platform.
if (WIN32)
    include (buildWindows)
elseif (APPLE)
    include (buildMacOS)
elseif (UNIX)
    include (buildUbuntu)
endif()

# Set INSTALL option.
set(INSTALL_ONLY ON)

# Install the application for the target platform.
if (WIN32)
    include (installWindows)
elseif (APPLE)
    include (installMacOS)
elseif (UNIX)
    include (installUbuntu)
endif()

# Package the application for the target platform.
if (NOT INSTALL_ONLY)
    if (WIN32)
        include (packageWindows)
    elseif (APPLE)
        include (packageMacOS)
    elseif (UNIX)
        include (packageUbuntu)
    endif()
endif()
