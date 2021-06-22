cmake_minimum_required(VERSION 3.18)
project(ext VERSION 0.1.3
            DESCRIPTION "The C++ standard library extensions"
            HOMEPAGE_URL "https://github.com/isnullxbh/ext"
            LANGUAGES CXX)

option(WITH_TESTS "Build the library with unit tests" OFF)

set(EXT "${PROJECT_NAME}")

add_definitions(
    -include ${CMAKE_CURRENT_SOURCE_DIR}/include/ext/platform.hpp)

add_subdirectory(src)

if(WITH_TESTS)
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
        file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
                      "${CMAKE_BINARY_DIR}/conan.cmake"
                      EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
                      TLS_VERIFY ON)
    endif()
    add_subdirectory(tests)
endif()
