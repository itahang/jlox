cmake_minimum_required(VERSION 3.15)
project(Jlox)

set(ALL_INCLUDE ${CMAKE_SOURCE_DIR}/include/)

add_library(Lexer  src/Lox.cpp src/Scanner.cpp src/Tokens.cpp) 

target_include_directories(Lexer PUBLIC ${ALL_INCLUDE})

add_executable(main main.cpp )
target_link_libraries(main PRIVATE Lexer)
