cmake_minimum_required(VERSION 3.14)

project(Lab5_7 LANGUAGES CXX)

add_compile_options(
        -g
        -Werror

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


include_directories(client_server_infra)

add_subdirectory(client_server_infra)
add_subdirectory(client)
# add_subdirectory(server)