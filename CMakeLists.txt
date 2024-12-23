cmake_minimum_required(VERSION 3.10)

project(VMCWorkflow)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6 component for Regular Expression.
find_package(Qt6 COMPONENTS Core Widgets Svg REQUIRED)

file(GLOB Header_FILES "include/*.h")
file(GLOB Implementation_FILES "src/*.cpp")

# Add the resource file
qt_add_resources(RESOURCES "res/resources.qrc")
set(RESOURCE_FILES "res/resources.rc")

add_executable(VMCWorkflow
    WIN32
    ${Implementation_FILES}
    ${Header_FILES}
    ${RESOURCES}
    ${RESOURCE_FILES}
 )

target_link_libraries(VMCWorkflow Qt::Core Qt6::Widgets Qt6::Svg)
