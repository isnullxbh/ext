include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_configure(REQUIRES gtest/cci.20210126
    OPTIONS    gtest:build_gmock=True
    GENERATORS cmake)

conan_cmake_autodetect(BUILD_SETTINGS)

conan_cmake_install(PATH_OR_REFERENCE .
    BUILD    missing
    REMOTE   conan-center
    SETTINGS ${BUILD_SETTINGS})

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(EXT_TESTS "${PROJECT_NAME}.tests")

add_executable(${EXT_TESTS})

target_include_directories(${EXT_TESTS} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CONAN_INCLUDE_DIRS})

target_sources(${EXT_TESTS} PRIVATE
    main.cpp
    holder.tests.cpp
    maybe.tests.cpp
    reference.tests.cpp
    result.tests.cpp)

target_link_directories(${EXT_TESTS} PRIVATE
    ${CONAN_LIB_DIRS})

target_link_libraries(${EXT_TESTS} PRIVATE
    ${CONAN_LIBS}
    ${EXT})
