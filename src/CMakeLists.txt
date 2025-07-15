cmake_minimum_required(VERSION 3.10)
project(twgec VERSION 1.0)
add_executable(twgec
    lib/frontend/ast.cpp
    lib/frontend/lexer.cpp
    lib/frontend/parser.cpp
    lib/transform/aliasInlining.cpp
    lib/transform/argBinding.cpp
    lib/codegen/action.cpp
    lib/codegen/check.cpp
    lib/codegen/codegen.cpp
    lib/codegen/metadata.cpp
    lib/codegen/transfromer.cpp
    lib/codegen/trigger.cpp
    lib/main.cpp
)

target_include_directories(twgec PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/frontend
    ${CMAKE_CURRENT_SOURCE_DIR}/include/transform
    ${CMAKE_CURRENT_SOURCE_DIR}/include/codegen
    ${CMAKE_CURRENT_SOURCE_DIR}/include/utils
)