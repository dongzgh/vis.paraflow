# Set the bundle properties.
set(CMAKE_MACOSX_BUNDLE TRUE)  # Enable macOS app bundle creation

# Set macOS specific resources.
set(ICNS_FILE "${CMAKE_SOURCE_DIR}/package/macos/app.icns")
set_source_files_properties(${ICNS_FILE} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
set(PLIST_FILE "${CMAKE_SOURCE_DIR}/package/macos/Info.plist")

# Add the executable target.
add_executable(${CMAKE_PROJECT_NAME}
    MACOSX_BUNDLE
    ${Implementation_FILES}
    ${Header_FILES}
    ${RESOURCE_FILE}
    ${ICNS_FILE}
)

# Set link libraries.
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets Qt6::Svg)

# Set the bundle Info.plist
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES 
    MACOSX_BUNDLE_INFO_PLIST "${PLIST_FILE}"
    MACOSX_BUNDLE_ICON_FILE "app.icns"
    RESOURCE "${ICNS_FILE}")
