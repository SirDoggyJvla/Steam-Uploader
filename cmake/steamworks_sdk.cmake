if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(steamworks_sdk_OS_ARCH "win64")
    set(steamworks_sdk_LIB_NAME "steam_api64")
    set(steamworks_sdk_LIB_EXT "dll")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(steamworks_sdk_OS_ARCH "linux64")
    set(steamworks_sdk_LIB_NAME "libsteam_api")
    set(steamworks_sdk_LIB_EXT "so")
else () # Commonly Darwin for mac
    set(steamworks_sdk_OS_ARCH "osx")
    set(steamworks_sdk_LIB_NAME "libsteam_api")
    set(steamworks_sdk_LIB_EXT "dylib")
endif ()

# Use local Steamworks SDK directory
set(steamworks_sdk_SOURCE_DIR "${CMAKE_SOURCE_DIR}/packages/steam")

add_library(steamworks_sdk SHARED IMPORTED)

set(steamworks_sdk_LIBDIR "${steamworks_sdk_SOURCE_DIR}/redistributable_bin/${steamworks_sdk_OS_ARCH}")
set(steamworks_sdk_LIBFILE "${steamworks_sdk_LIBDIR}/${steamworks_sdk_LIB_NAME}.${steamworks_sdk_LIB_EXT}")
set_target_properties(steamworks_sdk PROPERTIES
        IMPORTED_LOCATION ${steamworks_sdk_LIBFILE}
        INTERFACE_INCLUDE_DIRECTORIES "${steamworks_sdk_SOURCE_DIR}/public"
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set_target_properties(steamworks_sdk PROPERTIES
            IMPORTED_IMPLIB "${steamworks_sdk_LIBDIR}/${steamworks_sdk_LIB_NAME}.lib"
    )
endif ()
