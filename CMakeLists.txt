cmake_minimum_required(VERSION 3.6)
project(nesis)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES
        cpu/6502.c
        cpu/6502.h
        memory/memory.c
        memory/memory.h
        main.c)

add_executable(nesis ${SOURCE_FILES})