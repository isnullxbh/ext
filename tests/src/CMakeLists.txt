find_package(GTest CONFIG REQUIRED)

add_executable(ext.tests)

target_include_directories(ext.tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include)

target_sources(ext.tests
    PRIVATE
        main.cpp
        ext/fixed_string.test.cpp
        ext/type_list.test.cpp
        ext/value_wrapper.test.cpp)

target_link_libraries(ext.tests
    PRIVATE
        GTest::gtest
        ext)
