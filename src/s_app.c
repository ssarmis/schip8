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

#include "s_app.h"

void app_init(app_t* app){
    assert(app != NULL);
    SDL_Init(SDL_INIT_EVERYTHING);

    window_init(&app->window, 12);
}

void app_run(app_t* app, chip8_core_t* core){
    assert(app->window.window_screen.pixels != NULL);

    for(;;){
        SDL_Delay(1000 / 800);
        SDL_PollEvent(&app->event);
        if(app->event.type == SDL_QUIT) {
            break;
        }

        chip8_step(core);

        int y;
        int x;
        int offset = 0;
        for (y = 0; y < 32; ++y) {
            for (x = 0; x < 64; ++x) {
                if (core->display[x + y * 64]) {
                    app->window.window_screen.pixels[0 + offset] = 0xFF;//a
                    app->window.window_screen.pixels[1 + offset] = 0xFF;//b
                    app->window.window_screen.pixels[2 + offset] = 0xFF;//g
                    app->window.window_screen.pixels[3 + offset] = 0xFF;//r
                } else {
                    app->window.window_screen.pixels[0 + offset] = 0x00;
                    app->window.window_screen.pixels[1 + offset] = 0x00;
                    app->window.window_screen.pixels[2 + offset] = 0x00;
                    app->window.window_screen.pixels[3 + offset] = 0x00;
                }
                offset += 4;
            }
        }

        SDL_UpdateTexture(
                app->window.window_screen.screen,
                NULL,
                app->window.window_screen.pixels,
                64 * 4);
        SDL_RenderClear(app->window.window_screen.renderer);
        SDL_RenderCopy(app->window.window_screen.renderer, app->window.window_screen.screen, NULL, NULL);
        SDL_RenderPresent(app->window.window_screen.renderer);
    }
}

void app_shutdown(app_t* app){
    window_shutdown(&app->window);
    SDL_Quit();
}