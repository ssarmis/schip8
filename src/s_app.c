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

// TODO duplicate code, make just one function V
void app_assign_pressed_key(SDL_Keycode c, chip8_core_t* core){
    switch(c){
        case SDLK_0:
            core->keys[ZERO_KEY] = 1;
            break;
        case SDLK_1:
            core->keys[ONE_KEY] = 1;
            break;
        case SDLK_2:
            core->keys[TWO_KEY] = 1;
            break;
        case SDLK_3:
            core->keys[THREE_KEY] = 1;
            break;
        case SDLK_4:
            core->keys[FOUR_KEY] = 1;
            break;
        case SDLK_5:
            core->keys[FIVE_KEY] = 1;
            break;
        case SDLK_6:
            core->keys[SIX_KEY] = 1;
            break;
        case SDLK_7:
            core->keys[SEVEN_KEY] = 1;
            break;
        case SDLK_8:
            core->keys[EIGHT_KEY] = 1;
            break;
        case SDLK_9:
            core->keys[NINE_KEY] = 1;
            break;
        case SDLK_a:
            core->keys[A_KEY] = 1;
            break;
        case SDLK_b:
            core->keys[B_KEY] = 1;
            break;
        case SDLK_c:
            core->keys[C_KEY] = 1;
            break;
        case SDLK_d:
            core->keys[D_KEY] = 1;
            break;
        case SDLK_e:
            core->keys[E_KEY] = 1;
            break;
        case SDLK_f:
            core->keys[F_KEY] = 1;
            break;
    }
}

void app_assign_released_key(SDL_Keycode c, chip8_core_t* core){
    switch(c){
        case SDLK_0:
            core->keys[ZERO_KEY] = 0;
            break;
        case SDLK_1:
            core->keys[ONE_KEY] = 0;
            break;
        case SDLK_2:
            core->keys[TWO_KEY] = 0;
            break;
        case SDLK_3:
            core->keys[THREE_KEY] = 0;
            break;
        case SDLK_4:
            core->keys[FOUR_KEY] = 0;
            break;
        case SDLK_5:
            core->keys[FIVE_KEY] = 0;
            break;
        case SDLK_6:
            core->keys[SIX_KEY] = 0;
            break;
        case SDLK_7:
            core->keys[SEVEN_KEY] = 0;
            break;
        case SDLK_8:
            core->keys[EIGHT_KEY] = 0;
            break;
        case SDLK_9:
            core->keys[NINE_KEY] = 0;
            break;
        case SDLK_a:
            core->keys[A_KEY] = 0;
            break;
        case SDLK_b:
            core->keys[B_KEY] = 0;
            break;
        case SDLK_c:
            core->keys[C_KEY] = 0;
            break;
        case SDLK_d:
            core->keys[D_KEY] = 0;
            break;
        case SDLK_e:
            core->keys[E_KEY] = 0;
            break;
        case SDLK_f:
            core->keys[F_KEY] = 0;
            break;
    }
}

void app_init(app_t* app){
    assert(app != NULL);
    SDL_Init(SDL_INIT_EVERYTHING);

    window_init(&app->window, 12);
}

void app_run(app_t* app, chip8_core_t* core){
    assert(app->window.window != NULL);

    for(;;){
        SDL_Delay(1000 / 800);
        SDL_PollEvent(&app->event);
        if(app->event.type == SDL_QUIT) {
            break;
        } else if(app->event.type == SDL_KEYDOWN){
            app_assign_pressed_key(app->event.key.keysym.sym, core);
        } else if(app->event.type == SDL_KEYUP){
            app_assign_released_key(app->event.key.keysym.sym, core);
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
        //SDL_RenderClear(app->window.window_screen.renderer);
        SDL_RenderCopy(app->window.window_screen.renderer, app->window.window_screen.screen, NULL, NULL);
        SDL_RenderPresent(app->window.window_screen.renderer);
    }
}

void app_shutdown(app_t* app){
    window_shutdown(&app->window);
    SDL_Quit();
}
