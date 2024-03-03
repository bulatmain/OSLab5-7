cmake_minimum_required(VERSION 3.14)

project(Server LANGUAGES CXX)

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

include_directories(./core)
add_subdirectory(./core)

add_executable(server server.cpp)
target_link_libraries(server PRIVATE core)
