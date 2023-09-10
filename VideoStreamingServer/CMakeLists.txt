cmake_minimum_required(VERSION 3.12)
project(VideoStreamingServer)

set(CMAKE_CXX_STANDARD 17)

include(ExternalProject)

set(LIBDASH_SOURCE_DIR ${CMAKE_BINARY_DIR}/libdash/src)
set(LIBDASH_BINARY_DIR ${CMAKE_BINARY_DIR}/libdash/build)

if(NOT EXISTS ${LIBDASH_SOURCE_DIR}/libdash_external/libdash)
    ExternalProject_Add(
        libdash_external
        PREFIX ${CMAKE_BINARY_DIR}/libdash
        GIT_REPOSITORY https://github.com/bitmovin/libdash.git
        GIT_TAG master
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
    )
endif()

set(LIBDASH_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/libdash/install/include PARENT_SCOPE)
set(LIBDASH_LIBRARIES ${CMAKE_BINARY_DIR}/libdash/install/lib/libdash.a PARENT_SCOPE)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPQXX REQUIRED libpqxx)
pkg_check_modules(CURL REQUIRED libcurl)

include_directories(${LIBPQXX_INCLUDE_DIRS})
set(CROW_INCLUDE_DIR ./3rdParty/crow)
set (COMMON_INCLUDE_DIR ./Common)
include_directories(${COMMON_INCLUDE_DIR})

file(GLOB_RECURSE SOURCES "*.cpp" "*.h" "*.c" "*.hpp")

add_executable(VideoStreamingServer ${SOURCES})

target_link_libraries(VideoStreamingServer PRIVATE
    ${LIBPQXX_LIBRARIES}
    ${CURL_LIBRARIES}
    ${LIBDASH_LIBRARIES}
)

target_include_directories(VideoStreamingServer PRIVATE
    ${CROW_INCLUDE_DIR}
    ${CURL_INCLUDE_DIRS}
    ${LIBDASH_INCLUDE_DIRS}
    ./Common
    /usr/include/postgresql
)
