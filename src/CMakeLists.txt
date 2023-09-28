add_library(ext INTERFACE)

target_include_directories(ext
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include)
