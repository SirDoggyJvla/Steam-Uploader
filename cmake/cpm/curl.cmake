if (cmake_OS_ARCH)
    CPMAddPackage(
            NAME CURL
            URL "https://curl.se/windows/dl-8.15.0_4/curl-8.15.0_4-${cmake_OS_ARCH}-mingw.zip"
            VERSION "8.15.0_4"
    )
endif ()

if (CURL_ADDED)
    file(GLOB CURL_STATIC_LIBS "${CURL_SOURCE_DIR}/lib/*.a")
    add_library(CURL::libcurl INTERFACE IMPORTED)
    target_include_directories(CURL::libcurl INTERFACE "${CURL_SOURCE_DIR}/include")
    target_link_libraries(CURL::libcurl INTERFACE "${CURL_STATIC_LIBS}")
else()
    find_package(CURL REQUIRED)
endif ()
