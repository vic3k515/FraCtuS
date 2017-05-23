cmake_minimum_required(VERSION 3.5)
project(fractus)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    main.cpp
    Scanner.cpp
    #Scanner.h
    Reader.cpp
    #Reader.h
    Parser.cpp
    Fraction.cpp)
    #Fraction.h)

add_executable(fractus ${SOURCE_FILES})