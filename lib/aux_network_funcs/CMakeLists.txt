cmake_minimum_required(VERSION 3.14)

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

add_library(aux_network_funcs STATIC 
    ./get_ip_func/get_ip_func.hpp
)
set_target_properties(aux_network_funcs PROPERTIES LINKER_LANGUAGE CXX)
