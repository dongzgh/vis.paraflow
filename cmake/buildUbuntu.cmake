# Add the executable target.
add_executable(${CMAKE_PROJECT_NAME}
    ${Implementation_FILES}
    ${Header_FILES}
    ${RESOURCE_FILE}
)

# Set link libraries.
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets Qt6::Svg)
