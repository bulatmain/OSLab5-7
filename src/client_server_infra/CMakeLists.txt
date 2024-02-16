cmake_minimum_required(VERSION 3.14)

project(client_server_infra LANGUAGES CXX)

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

# To include infrastructure in cpp file add "#include <infrastructure_module.hpp>" 
add_library(infrastructure STATIC ./infrastructure_module.hpp)
set_target_properties(infrastructure PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(infrastructure PUBLIC zmq)

