add_library(ext INTERFACE)
add_library(ext::core ALIAS ext)

set_target_properties(ext
    PROPERTIES
        OUTPUT_NAME "ext"
        PREFIX      "")

target_sources(ext
    INTERFACE
        FILE_SET HEADERS
            BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../include
            FILES
                ${CMAKE_CURRENT_SOURCE_DIR}/../include/ext/detail/indexed.hpp
                ${CMAKE_CURRENT_SOURCE_DIR}/../include/ext/detail/type_list_ops.hpp
                ${CMAKE_CURRENT_SOURCE_DIR}/../include/ext/ext.hpp
                ${CMAKE_CURRENT_SOURCE_DIR}/../include/ext/type_holder.hpp
                ${CMAKE_CURRENT_SOURCE_DIR}/../include/ext/type_list.hpp)

target_include_directories(ext
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<INSTALL_INTERFACE:include>)
