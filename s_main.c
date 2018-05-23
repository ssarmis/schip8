
#include <SDL2/SDL.h>

#include <stdio.h>
#include "s_chip8.h"

int main(int argc, char* argv[]) {

    chip8_core_t core;

    chip8_core_init(&core);
    chip8_rtomem(&core, "../UFO");

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* screen;

    int w = 64, h = 32, s = 15;

    window = SDL_CreateWindow("schip8",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              w * s, h * s,
                              SDL_WINDOW_RESIZABLE);

    renderer = SDL_CreateRenderer(window, -1, 0);

    screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, w, h);
    uint8_t pixels[w * h * 4];

    SDL_Event event;

    for(;;){
        SDL_Delay(1000 / 800);
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT) {
            break;
        }
        chip8_step(&core);
        // TODO move this into a module or function
        int y;
        int x;
        int offset = 0;
        for (y = 0; y < h; ++y) {
            for (x = 0; x < w; ++x) {
                if (core.display[x + y * w]) {
                    pixels[0 + offset] = 0xFF;//a
                    pixels[1 + offset] = 0xFF;//b
                    pixels[2 + offset] = 0xFF;//g
                    pixels[3 + offset] = 0xFF;//r
                } else {
                    pixels[0 + offset] = 0x00;//a
                    pixels[1 + offset] = 0x00;//b
                    pixels[2 + offset] = 0x00;//g
                    pixels[3 + offset] = 0x00;//r
                }
                offset += 4;
            }
        }

        SDL_UpdateTexture(
                screen,
                NULL,
                &pixels[0],
                w * 4);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, screen, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}