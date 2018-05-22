#include <SDL2/SDL.h>

#include <stdio.h>
#include "s_chip8.h"

int main(int argc, char* argv[]) {

    chip8_core_t core;

    chip8_core_init(&core);

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;

    int w = 64, h = 32, s = 8;

    window = SDL_CreateWindow("SDL_CreateTexture",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              w * s, h * s,
                              SDL_WINDOW_RESIZABLE);

    renderer = SDL_CreateRenderer(window, -1, 0);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

    SDL_Event event;

    for(;;){
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT) {
            break;
        }
    }

    //chip8_step(&core);
    //chip8_step(&core);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}