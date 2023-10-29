add_library(ext)

set_target_properties(ext
    PROPERTIES
        OUTPUT_NAME "ext"
        PREFIX      "")

target_include_directories(ext
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(ext
    PRIVATE
        ext/ext.cpp)
