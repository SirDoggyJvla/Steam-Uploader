cmake_minimum_required(VERSION 3.30)

set(SteamUploader_NAME "SteamUploader")
set(SteamUploader_DESC "A CLI tool to update UGC on the Steam workshop")
set(SteamUploader_VERSION 0.6.0)

set(BUILD_SHARED_LIBS ON)
set(BUILD_STATIC_LIBS OFF)
set(BUILD_TESTING OFF)

project(SteamUploader VERSION ${SteamUploader_VERSION} DESCRIPTION ${SteamUploader_DESC})

# Set the CPM cache to .cache/ if there isn't one already.
# Prevents downloading the same things over and over.
if (NOT DEFINED ENV{CPM_SOURCE_CACHE})
    set(ENV{CPM_SOURCE_CACHE} "${CMAKE_SOURCE_DIR}/.cache/CPM")
endif ()

if (WIN32)
    set(OS_ARCH "win64")
    set(cmake_OS_ARCH "win64")

    if (CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "ARM64")
        set(cmake_OS_ARCH "win64a")
    endif ()
elseif (APPLE)
    set(OS_ARCH "osx")
elseif (UNIX)
    set(OS_ARCH "linux64")
else ()
    message(FATAL_ERROR "Unsupported platform")
endif ()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# CPM Dependencies

include(CPM)
include(cpm/spdlog)
include(cpm/nlohmann_json)
# include(cpm/curl)
include(cpm/steamworks_sdk)
include(cpm/cxxopts)

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
        PROJECT_VERSION="${SteamUploader_VERSION}")

target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE
    steamworks_sdk
    nlohmann_json
    # CURL::libcurl
    cxxopts::cxxopts
    spdlog::spdlog
    curl
)

find_package(CURL REQUIRED)
