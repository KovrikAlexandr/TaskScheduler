cmake_minimum_required(VERSION 3.15)
project(labwork10)

add_subdirectory(src)

set(BUILD_EXAMPLE TRUE)
set(BUILD_TESTS TRUE)

if (BUILD_EXAMPLE)
    add_subdirectory(example)
endif()

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()