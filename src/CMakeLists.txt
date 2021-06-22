if(HEADER_ONLY)
    add_library(${EXT} INTERFACE)

    target_compile_definitions(${EXT} INTERFACE
        EXT_VERSION="${PROJECT_VERSION}")

    target_include_directories(${EXT} INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include)
else()
    add_library(${EXT})

    target_compile_definitions(${EXT} PUBLIC
        EXT_VERSION="${PROJECT_VERSION}")

    target_include_directories(${EXT} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include)

    target_sources(${EXT} PRIVATE
        exceptions.cpp)
endif()
