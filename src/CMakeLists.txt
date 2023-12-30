add_library(ext INTERFACE)

set_target_properties(ext
    PROPERTIES
        OUTPUT_NAME "ext"
        PREFIX      "")

target_include_directories(ext
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include)
