cmake_minimum_required(VERSION 3.14)

project(core LANGUAGES CXX)

add_compile_options(
        -g
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

add_executable(server server.cpp)
target_include_directories(server PRIVATE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
    $<INSTALL_INTERFACE:lib>
)
target_link_libraries(server PRIVATE lib)
