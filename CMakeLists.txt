cmake_minimum_required(VERSION 3.5)
project(fractus)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_SOURCE_DIR})

set(SOURCE_FILES
    Fraction.cpp
    Reader.cpp
    Scanner.cpp
    Ast.cpp
    Scope.cpp
    Parser.cpp
    SemanticAnalyzer.cpp
    Interpreter.cpp
    main.cpp
)

add_executable(fractus ${SOURCE_FILES})
target_compile_options(fractus PRIVATE "-Wall")