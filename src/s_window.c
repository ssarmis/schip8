//	schip8 is a chip-8 emulator
//
//	Copyright (C) 2018 Streanga Sarmis-Stefan<streangasarmis@gmail.com>
//
//	This program is free software : you can redistribute it and / or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program, if not, see <http://www.gnu.org/licenses/>.


#include "s_window.h"

void window_init(window_t* window, uint8_t scale){
    assert(window != NULL);
    window->window_details.scale = scale;

    window->window = SDL_CreateWindow("schip8",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              64 * scale, 32 * scale,
                              SDL_WINDOW_RESIZABLE);

    assert(window->window != NULL);
    window->window_screen.renderer = SDL_CreateRenderer(window->window, -1, 0);

    assert(window->window_screen.renderer != NULL);
    window->window_screen.screen = SDL_CreateTexture(window->window_screen.renderer,
            SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
}

void window_shutdown(window_t* window){
    SDL_DestroyRenderer(window->window_screen.renderer);
    SDL_DestroyTexture(window->window_screen.screen);
    SDL_DestroyWindow(window->window);
}
