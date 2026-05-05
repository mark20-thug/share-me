cmake_minimum_required(VERSION 3.10)
project(FileTransferApp VERSION 2.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable
add_executable(folder_app folder.cpp)

# Platform-specific settings
if(WIN32)
    message(STATUS "Building for Windows")
    target_link_libraries(folder_app PRIVATE ws2_32)
    
elseif(APPLE)
    message(STATUS "Building for macOS")
    target_compile_options(folder_app PRIVATE -fPIC)
    
elseif(UNIX AND NOT APPLE)
    message(STATUS "Building for Linux")
    target_compile_options(folder_app PRIVATE -fPIC)
    
endif()

# Compiler flags for all platforms
if(MSVC)
    target_compile_options(folder_app PRIVATE /W4)
else()
    target_compile_options(folder_app PRIVATE -Wall -Wextra -pedantic)
endif()

# Installation rules
install(TARGETS folder_app 
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION Applications)

install(FILES README.md BUILD_INSTRUCTIONS.md 
    DESTINATION share/doc/folder_app)

# CPack configuration for creating installers
set(CPACK_PACKAGE_NAME "FileTransferApp")
set(CPACK_PACKAGE_VENDOR "File Transfer")
set(CPACK_PACKAGE_DESCRIPTION "Cross-platform file transfer application for sharing files with neighbors over network")
set(CPACK_PACKAGE_VERSION "2.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "2")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")

# Linux packaging
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Your Name <your@email.com>")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://example.com")
set(CPACK_DEBIAN_PACKAGE_SECTION "utilities")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")

# Windows installer
set(CPACK_NSIS_INSTALL_ROOT "C:\\\\Program Files")
set(CPACK_NSIS_CREATE_ICONS_EXTRA "CreateDirectory \\\"$SMPROGRAMS\\\\FileTransferApp\\\"
  CreateShortCut \\\"$SMPROGRAMS\\\\FileTransferApp\\\\File Transfer App.lnk\\\" \\\"$INSTDIR\\\\bin\\\\folder_app.exe\\\"")

# macOS installer
set(CPACK_BUNDLE_NAME "FileTransferApp")
set(CPACK_BUNDLE_ICON "")
set(CPACK_DMG_DISABLE_APPLICATIONS_SYMLINK ON)

include(CPack)
