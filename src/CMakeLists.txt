cmake_minimum_required(VERSION 3.10)
project(twgec VERSION 1.0)
add_executable(twgec
    lib/codegen.cpp
    lib/main.cpp
    lib/ast.cpp
    lib/lexer.cpp
    lib/parser.cpp
)

target_include_directories(twgec PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)