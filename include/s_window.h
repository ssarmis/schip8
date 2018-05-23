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


#pragma once

#include <assert.h>
#include <malloc.h>
#include <stdint.h>
#include <SDL2/SDL.h>

typedef struct window_details {
    uint8_t scale;
} window_details_t;

typedef struct window_screen {
    SDL_Renderer* renderer;
    SDL_Texture* screen;
    uint8_t pixels[64 * 32 * 4];
} window_screen_t;

typedef struct window {
    window_details_t window_details;
    window_screen_t window_screen;
    SDL_Window* window;
} window_t;

// TODO add config reading function

void window_init(window_t* window, uint8_t scale);

void window_shutdown(window_t* window);
