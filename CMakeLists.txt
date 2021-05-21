cmake_minimum_required(VERSION 3.18)
project(ext VERSION 0.1.0
            DESCRIPTION "The C++ standard library extensions"
            HOMEPAGE_URL "https://github.com/isnullxbh/ext"
            LANGUAGES CXX)

set(EXT "${PROJECT_NAME}")

add_subdirectory(src)
