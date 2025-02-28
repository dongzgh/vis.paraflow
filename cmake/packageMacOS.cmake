# Set CPack variables.
set(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${CMAKE_PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${CMAKE_PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_VENDOR ${CPACK_PACKAGE_NAME})
set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION})
set(CPACK_BUNDLE_ICON ${ICNS_FILE})
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_SOURCE_DIR}/setup/macos")

# Set package generator to Apple Disk Image.
set(CPACK_GENERATOR DragNDrop)

# Include CPack module.
include(CPack)
