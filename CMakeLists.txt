cmake_minimum_required(VERSION 3.5)
project(fractus)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    main.cpp
    Scanner.cpp
    Reader.cpp
    Parser.cpp
    Fraction.cpp
    Scope.cpp
    SemanticAnalyzer.cpp)

add_executable(fractus ${SOURCE_FILES})