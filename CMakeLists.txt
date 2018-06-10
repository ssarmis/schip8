cmake_minimum_required(VERSION 3.10)
project(schip8 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11")

set(S_CHIP8_INCLUDE "./include")

if (WIN32)
    set(SDL2_INCLUDE_DIR "D:/C or C++ Workspace/SDL2mingw/i686-w64-mingw32/include")
    set(SDL2_LIB_DIR "D:/C or C++ Workspace/SDL2mingw/i686-w64-mingw32/lib")


    include_directories(${SDL2_INCLUDE_DIR})
    include_directories(${S_CHIP8_INCLUDE})

    link_directories(${SDL2_LIB_DIR})

    add_executable(schip8
            src/s_chip8.c
            src/s_main.c
            src/s_window.c
            src/s_app.c
            ${S_CHIP8_INCLUDE}/s_window.h
            ${S_CHIP8_INCLUDE}/s_chip8.h
            ${S_CHIP8_INCLUDE}/s_app.h)

    target_link_libraries(schip8 mingw32 SDL2main SDL2)
endif (WIN32)

if (UNIX)
    find_package(SDL2 REQUIRED)

    include_directories(${S_CHIP8_INCLUDE})
    include_directories(${SDL2_INCLUDE_DIRS})

    link_directories(${SDL2_LIB_DIR})

    add_executable(schip8
            src/s_chip8.c
            src/s_main.c
            src/s_window.c
            src/s_app.c
            ${S_CHIP8_INCLUDE}/s_window.h
            ${S_CHIP8_INCLUDE}/s_chip8.h
            ${S_CHIP8_INCLUDE}/s_app.h)

    target_link_libraries(schip8 ${SDL2_LIBRARIES})
endif (UNIX)

