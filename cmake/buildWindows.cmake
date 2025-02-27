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

# Set the target properties.
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIR} 
)
