cmake_minimum_required(VERSION 3.10)
project(ZweekCompiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

# Main Compiler Executable
add_executable(zweek 
    src/main.cpp 
    src/lexer.cpp 
    src/parser.cpp 
    src/codegen.cpp
)
