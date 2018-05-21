cmake_minimum_required(VERSION 3.10)
project(schip8 C)

set(CMAKE_C_STANDARD 11)

add_executable(schip8 s_main.c s_chip8.c s_chip8.h)
