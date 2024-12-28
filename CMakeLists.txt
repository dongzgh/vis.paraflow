cmake_minimum_required(VERSION 3.10)

project(visflow)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

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

file(GLOB Header_FILES "include/*.h")
file(GLOB Implementation_FILES "src/*.cpp")

# Add the resource file
qt_add_resources(RESOURCES "res/resources.qrc")
set(RESOURCE_FILES "res/resources.rc")

add_executable(visflow
    WIN32
    ${Implementation_FILES}
    ${Header_FILES}
    ${RESOURCES}
    ${RESOURCE_FILES}
 )

target_link_libraries(visflow Qt::Core Qt6::Widgets Qt6::Svg)
