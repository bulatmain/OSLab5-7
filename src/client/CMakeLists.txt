cmake_minimum_required(VERSION 3.14)

project(client LANGUAGES CXX)

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

add_executable(client client.cpp)

set_target_properties(cs_infra PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(client PUBLIC cs_infra)