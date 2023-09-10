cmake_minimum_required(VERSION 3.12)
project(VideoStreamingServer)

set(CMAKE_CXX_STANDARD 17)

include(ExternalProject)
 
 
if(NOT EXISTS ${LIBDASH_SOURCE_DIR}/libdash/src/libdash)
    set(LIBDASH_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/libdash/src)

    execute_process(
        COMMAND git clone https://github.com/bitmovin/libdash.git
        WORKING_DIRECTORY ${LIBDASH_SOURCE_DIR}
    )

endif()
 
ExternalProject_Add(
    libdash_external
    PREFIX libdash
    SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/libdash/src/libdash
    GIT_REPOSITORY "https://github.com/bitmovin/libdash.git"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
)

ExternalProject_Get_Property(libdash_external SOURCE_DIR)
if(NOT EXISTS ${LIBDASH_SOURCE_DIR}/libdash/libdash/build/bin)


message(${LIBDASH_SOURCE_DIR}/libdash/libdash/build)
file(MAKE_DIRECTORY ${LIBDASH_SOURCE_DIR}/libdash/libdash/build)

execute_process(
  #  COMMAND ${CMAKE_COMMAND} -E chdir ${LIBDASH_SOURCE_DIR}/libdash/libdash/build bash -c "cmake ../ && make"
)
endif()

set(LIBDASH_BINARY_LIB ${CMAKE_CURRENT_BINARY_DIR}/libdash/src/libdash/libdash/build/bin)
set(LIBDASH_BINARY_INCLUDE ${CMAKE_CURRENT_BINARY_DIR}/libdash/src/libdash/libdash/libdash/include)

add_library(libdash INTERFACE)
target_include_directories(libdash INTERFACE ${SOURCE_DIR}/libdash/include)
target_link_libraries(libdash INTERFACE ${BINARY_DIR}/libdash/src/libdash-build/src/libdash.a)
add_dependencies(libdash libdash_external)

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
    ${LIBDASH_BINARY_LIB}/libdash.so
)

target_include_directories(VideoStreamingServer PRIVATE
    ${CROW_INCLUDE_DIR}
    ${CURL_INCLUDE_DIRS}
    ${LIBDASH_BINARY_INCLUDE}
    ./Common
    /usr/include/postgresql
)
