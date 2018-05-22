cmake_minimum_required(VERSION 3.10)
project(schip8 C)

#set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11")

set(SDL2_INCLUDE_DIR "D:/C or C++ Workspace/SDL2mingw/i686-w64-mingw32/include")
set(SDL2_LIB_DIR "D:/C or C++ Workspace/SDL2mingw/i686-w64-mingw32/lib")

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

add_executable(schip8 s_main.c s_chip8.c s_chip8.h)

target_link_libraries(schip8 mingw32 SDL2main SDL2)