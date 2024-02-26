cmake_minimum_required(VERSION 3.14)

project(ConnectionWrapper LANGUAGES CXX)

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

# To include connection_wrapper in cpp file add "#include <connection_module.hpp>" 
add_library(connection_wrapper STATIC ./connection_module.hpp)
set_target_properties(connection_wrapper PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(connection_wrapper PUBLIC zmq)
target_include_directories(connection_wrapper PRIVATE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib/infrastructure/message>
    $<INSTALL_INTERFACE:message>
)
target_link_libraries(connection_wrapper PRIVATE message)