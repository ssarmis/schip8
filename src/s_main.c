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


#include <stdio.h>
#include <SDL2/SDL.h>

#include "s_app.h"
#include "s_chip8.h"

int main(int argc, char* argv[]) {

    chip8_core_t core;
    chip8_core_init(&core);

    chip8_rtomem(&core, "../BLITZ");

    app_t app;
    app_init(&app);

    app_run(&app, &core);

    app_shutdown(&app);
    chip8_shutdown(&core);

    return 0;
}