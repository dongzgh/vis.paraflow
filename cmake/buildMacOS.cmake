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

# Set link libraries.
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets Qt6::Svg)

# Set the target properties.
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIR} 
)
