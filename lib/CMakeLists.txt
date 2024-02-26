cmake_minimum_required(VERSION 3.14)

project(Lab5_7 LANGUAGES CXX)

add_compile_options(
        -g
        -lzmq
        # -Werror

        -Wall
        -Wextra
        -Wpedantic

        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wenum-compare
        -Wfloat-equal
        -Wredundant-decls
        -Wsign-conversion
)

if(NOT CMAKE_C_EXTENSIONS)
    set(CMAKE_C_EXTENSIONS OFF)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(zmq)

include_directories(infrastructure)
add_subdirectory(infrastructure)