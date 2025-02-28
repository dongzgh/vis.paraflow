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

# Set link libraries.
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets Qt6::Svg)
