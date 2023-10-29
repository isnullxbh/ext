cmake_minimum_required(VERSION 3.23)
project(ext VERSION 0.0.0
            DESCRIPTION "The C++ standard library extensions"
            HOMEPAGE_URL "https://github.com/isnullxbh/ext"
            LANGUAGES C CXX)

option(EXT_BUILD_TESTS "Create a build target for the unit tests" OFF)

add_subdirectory(src)

if(EXT_BUILD_TESTS)
    add_subdirectory(tests)
endif()

add_custom_target(ext.generate-docs
    COMMAND doxygen docs/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
