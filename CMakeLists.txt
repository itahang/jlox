cmake_minimum_required(VERSION 3.15)
project(Jlox)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(ALL_INCLUDE ${CMAKE_SOURCE_DIR}/include/)

add_library(Lexer src/Lox.cpp src/Scanner.cpp src/Tokens.cpp)
target_include_directories(Lexer PUBLIC ${ALL_INCLUDE})

add_executable(main main.cpp)
target_link_libraries(main PRIVATE Lexer)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
