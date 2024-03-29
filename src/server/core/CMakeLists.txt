cmake_minimum_required(VERSION 3.14)

project(Core LANGUAGES CXX)

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

add_library(core STATIC server_entity/server_entity.hpp)
set_target_properties(core PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(core PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
    $<INSTALL_INTERFACE:lib>
)
target_link_libraries(core PUBLIC lib)
target_link_libraries(core PUBLIC zmq)

add_executable(calc_tree_node calc_tree_node_main.cpp)
target_link_libraries(calc_tree_node PRIVATE core)

# target_include_directories(core PUBLIC
#     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/server/calc_tree>
#     $<INSTALL_INTERFACE:calc_tree>
# )
# target_link_libraries(core PUBLIC calc_tree)
