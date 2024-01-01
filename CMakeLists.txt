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

# Installation
install(EXPORT ExtTargets
    DESTINATION lib/cmake/ext
    FILE ExtTargets.cmake)

install(TARGETS ext
    EXPORT ExtTargets
    FILE_SET HEADERS
        DESTINATION include)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake/Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/ExtConfig.cmake
    INSTALL_DESTINATION lib/cmake/ext
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/ExtConfigVersion.cmake
    VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
    COMPATIBILITY AnyNewerVersion)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ExtConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/ExtConfigVersion.cmake
    DESTINATION lib/cmake/ext)
