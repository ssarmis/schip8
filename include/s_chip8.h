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

//#define DEBUG_DRAWING
//#define DEBUG_OPS

#include <mem.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

enum _0_opcodes {
    _clear_screen       = 0x00E0,
    _return_subroutine  = 0x00EE
};

enum _8_opcodes {
    _set_vx_vy          = 0x8000,
    _set_vx_vx_or_vy    = 0x8001,
    _set_vx_vx_and_vy   = 0x8002,
    _set_vx_vx_xor_vy   = 0x8003,
    _add_vy_vx          = 0x8004,
    _sub_vy_vx          = 0x8005,
    _shiftr_vy_vx       = 0x8006,
    _set_vx_vy_vx       = 0x8007,
    _shiftl_vy_vx       = 0x800E
};

enum _E_opcodes {
    _skip_vx_pressed    = 0xE09E,
    _skip_vx_released   = 0xE0A1
};

enum _F_opcodes {
    _set_vx_time        = 0xF007,
    _key_wait           = 0XF00A,
    _set_delay_vx       = 0XF015,
    _set_time_vx        = 0XF018,
    _add_vx_i           = 0XF01E,
    _set_i_sprite_vx    = 0XF029,
    _sore_bcd_vx        = 0XF033,
    _store_v0_vx        = 0XF055,
    _fill_v0_vx_i       = 0XF065
};

enum opcodes{
    _goto           = 0x1,
    _call           = 0x2,
    _skip_vx_nn     = 0x3,
    _skip_vx_n_nn   = 0x4,
    _skip_vx_vy     = 0x5,
    _set_vx         = 0x6,
    _add_nn_vx      = 0x7,
    _skip_vx_n_vy   = 0x9,
    _set_I          = 0xA,
    _jump_nnn_v0    = 0xB,
    _set_vx_rand_nn = 0xC,
    _draw = 0xD
};

typedef struct chip8_8bit_register {
    uint8_t data;
} chip8_8bit_register_t;

typedef struct chip8_16bit_register {
    uint16_t data;
} chip8_16bit_register_t;

typedef struct chip8_core{
    uint8_t delay;
    uint8_t sound;
    uint8_t keys[16];
    uint8_t display[64 * 32]; // resolution of the screen was 64 x 32
    uint8_t memory[4096]; // 4k memory
    uint16_t stack[16]; // a stack to return from subroutines
    chip8_8bit_register_t V[16]; // 16 V registers
    chip8_16bit_register_t sp; // stack pointer
    chip8_16bit_register_t pc; // program counter
    chip8_16bit_register_t I; // I register
} chip8_core_t;

typedef struct opcode_instruction {
    uint8_t param[2];
} opcode_instruction_t;

typedef struct opcode_0_instruction {
} opcode_0_instruction_t;

typedef struct opcode_8_instruction {
    uint8_t param[2];
} opcode_8_instruction_t;

typedef struct opcode_E_instruction {
    uint8_t param;
} opcode_E_instruction_t;

typedef struct opcode_F_instruction {
    uint8_t param;
} opcode_F_instruction_t;

void chip8_core_init(chip8_core_t* core);

void chip8_step(chip8_core_t* core);

void chip8_execute(chip8_core_t* core, uint8_t code[2], opcode_instruction_t* instruction);

void chip8_execute0(chip8_core_t* core, uint8_t code[2], opcode_0_instruction_t* instruction);

void chip8_execute8(chip8_core_t* core, uint8_t code[2], opcode_8_instruction_t* instruction);

void chip8_executeE(chip8_core_t* core, uint8_t code[2], opcode_E_instruction_t* instruction);

void chip8_executeF(chip8_core_t* core, uint8_t code[2], opcode_F_instruction_t* instruction);

void chip8_rtomem(chip8_core_t* core, const char* filename);

void chip8_shutdown(chip8_core_t* core);