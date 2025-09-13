cmake_minimum_required(VERSION 3.30)

message(STATUS "Configuring project for system: ${CMAKE_SYSTEM_NAME}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/dist")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
# These are for VS because it requires them set for each configuration
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin")

# Vcpkg toolchain file
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

# Project details
set(SteamUploader_NAME "SteamUploader")
set(SteamUploader_DESC "A CLI tool to update UGC on the Steam workshop")
set(SteamUploader_VERSION 0.6.0)
set(SteamUploader_VENDOR "SirDoggyJvla")

# CMake build variables
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
set(BUILD_STATIC_LIBS ON)
set(BUILD_TESTING OFF)

project(${SteamUploader_NAME} VERSION ${SteamUploader_VERSION} DESCRIPTION ${SteamUploader_DESC})

# Adding sources manually gives much faster builds than globbing the entire source dir
add_executable("${CMAKE_PROJECT_NAME}" ${SOURCES}
        src/main.cpp
        src/Uploader.cpp
        src/Uploader.h
        src/Enum/EItemUpdateStatus.h
        src/Enum/EResult.h
        src/Util/AppID.cpp
        src/Util/AppID.h
        src/Util/HasLanguage.h
        src/Util/ReadFile.cpp
        src/Util/ReadFile.h
        src/Util/Updater.h
        src/Util/WarningHook.cpp
        src/Util/WarningHook.h
)
target_compile_features("${CMAKE_PROJECT_NAME}" PRIVATE cxx_std_23)
target_compile_definitions("${CMAKE_PROJECT_NAME}" PRIVATE
        PROJECT_NAME="${SteamUploader_NAME}"
        PROJECT_DESCRIPTION="${SteamUploader_DESC}"
        PROJECT_VERSION="${SteamUploader_VERSION}"
        PROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        PROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR}
        PROJECT_VERSION_PATCH=${PROJECT_VERSION_PATCH}
)

# Project dependencies

find_package(cxxopts CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(CURL REQUIRED)

include(steamworks_sdk)

target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE
        steamworks_sdk
        nlohmann_json::nlohmann_json
        CURL::libcurl
        cxxopts::cxxopts
        spdlog::spdlog
)
target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -static -static-libgcc -static-libstdc++)

# Moves all of the dynamic libraries needed to the runtime output bin/ folder.
if (WIN32)
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${CMAKE_PROJECT_NAME}> $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
            COMMAND_EXPAND_LISTS
    )
elseif (UNIX AND NOT APPLE)
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/packages/steam/redistributable_bin/linux64/libsteam_api.so"
            $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
    )
endif ()

# CPack install setup
install(TARGETS ${CMAKE_PROJECT_NAME}
        RUNTIME DESTINATION /
        LIBRARY DESTINATION /
)
install(FILES $<TARGET_RUNTIME_DLLS:${CMAKE_PROJECT_NAME}> DESTINATION /)

set(CPACK_PACKAGE_NAME ${SteamUploader_NAME})
set(CPACK_PACKAGE_VERSION ${SteamUploader_VERSION})
set(CPACK_PACKAGE_VENDOR ${SteamUploader_VENDOR})
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CMAKE_SYSTEM_NAME}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CMAKE_SYSTEM_NAME}-source")
include(CPack)
