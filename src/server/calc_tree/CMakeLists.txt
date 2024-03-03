cmake_minimum_required(VERSION 3.14)

project(CalcTree LANGUAGES CXX)

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

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add_library(calc_tree STATIC server_entity/server_entity.hpp)
# set_target_properties(calc_tree PROPERTIES LINKER_LANGUAGE CXX)
# target_include_directories(calc_tree PUBLIC 
#     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
#     $<INSTALL_INTERFACE:lib>
# )
# target_link_libraries(calc_tree PUBLIC lib)

# target_include_directories(calc_tree PUBLIC
#     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/server/calc_tree>
#     $<INSTALL_INTERFACE:calc_tree>
# )
# target_link_libraries(calc_tree PUBLIC calc_tree)
