cmake_minimum_required(VERSION 3.14)

project(client LANGUAGES CXX)

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

add_executable(client client.cpp)
target_include_directories(client PRIVATE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib/infrastructure>
    $<INSTALL_INTERFACE:infrastructure>
)
target_link_libraries(client PRIVATE infrastructure)

add_executable(test test.cpp)
target_include_directories(test PRIVATE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib/infrastructure>
    $<INSTALL_INTERFACE:infrastructure>
)
target_link_libraries(test PRIVATE infrastructure)