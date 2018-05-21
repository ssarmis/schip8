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

#include <stdio.h> // remove

#include "s_chip8.h"

uint8_t chip8_fontset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};
/*
 * In memory it should look something like this
 * when using the font data to draw pixels in a wanted
 * shape..
 *
 *  0xF0        b11110000      ____
 *  0x90        b10010000     |    |
 *  0x90   ->   b10010000 ->  |    |
 *  0x90        b10010000     |    |
 *  0xF0        b11110000     |____|
 *
 */

void chip8_core_init(chip8_core_t* core){
    core->I.data = 0;
    core->pc.data = 0x200; // 512 offset
    core->sp.data = 0;

    // test goto 0x1NNN
    core->memory[core->pc.data]     = 0x11;
    core->memory[core->pc.data + 1] = 0x23;

    // test 0xFX29
    core->memory[0x0123]     = 0xF8;
    core->memory[0x0123 + 1] = 0x29;

}

void chip8_step(chip8_core_t* core){
    uint8_t opcode[2] = {
            core->memory[core->pc.data],
            core->memory[core->pc.data + 1]
    };

    opcode_instruction_t   _instruction;
    opcode_0_instruction_t _0_instruction;
    opcode_8_instruction_t _8_instruction;
    opcode_E_instruction_t _E_instruction;
    opcode_F_instruction_t _F_instruction;

    if (!opcode[0]){
        // TODO implement functions for opcodes that start with 0x00
    } else {
        uint8_t code = (opcode[0] & 0xF0) >> 4;
        switch (code){
            case 0x8:
                _8_instruction.param[0] = (opcode[0] & 0x0F);
                _8_instruction.param[1] = (opcode[1] & 0xF0) >> 4;
                chip8_execute8(core, opcode, &_8_instruction);
                break;
            case 0xE:
                _E_instruction.param = (opcode[0] & 0x0F);
                chip8_executeE(core, opcode, &_E_instruction);
                break;
            case 0xF:
                _F_instruction.param = (opcode[0] & 0x0F);
                chip8_executeF(core, opcode, &_F_instruction);
                break;
            default:
                _instruction.param[0] = (opcode[0] & 0x0F);
                _instruction.param[1] = (opcode[1] & 0xFF);
                chip8_execute(core, opcode, &_instruction);
                break;
        }
    }

    // TODO make this in a different way
}

void chip8_execute(chip8_core_t* core, uint8_t code[2], opcode_instruction_t* instruction){
    uint16_t op = code[0] & 0x000F;

    switch(op){
        case _goto :
            printf("jumping to address: %x\n\n", ((instruction->param[0] << 8) | instruction->param[1]));

            core->pc.data = ((instruction->param[0] << 8) | instruction->param[1]);
            break;

        case _call : break;
        case _skip_vx_nn :
            if(core->V[instruction->param[0]].data == instruction->param[1]){
                core->pc.data += 4;
            } else {
                core->pc.data += 2;
            }
            break;

        case _skip_vx_n_nn :
            if(core->V[instruction->param[0]].data != instruction->param[1]){
                core->pc.data += 4;
            } else {
                core->pc.data += 2;
            }
            break;

        case _skip_vx_vy :
            if(core->V[instruction->param[0]].data == core->V[((instruction->param[1] & 0xF0) >> 4)].data) {
                core->pc.data += 4;
            } else {
                core->pc.data += 2;
            }
            break;

        case _set_vx :
            core->V[instruction->param[0]].data = instruction->param[1];

            core->pc.data += 2;
            break;

        case _add_nn_vx :
            core->V[instruction->param[0]].data += instruction->param[1];

            core->pc.data += 2;
            break;

        case _skip_vx_n_vy :
            if(core->V[instruction->param[0]].data != core->V[((instruction->param[1] & 0xF0) >> 4)].data) {
                core->pc.data += 4;
            } else {
                core->pc.data += 2;
            }
            break;

        case _set_I :
            core->I.data = ((instruction->param[0] << 8) | instruction->param[1]);

            core->pc.data += 2;
            break;

        case _jump_nnn_v0 :
            core->pc.data += ((instruction->param[0] << 8) | instruction->param[1]) + core->V[0].data;
            break;

        case _set_vx_rand_nn :
            core->V[instruction->param[0]].data = ((rand() % 255) & instruction->param[1]);
            core->pc.data += 2;
            break;

        case _draw :
            // TODO imeplement this
            break;

        default:
            printf("Found a weird opcode: %x\n\n", op);
            break;
    }
}

void chip8_execute8(chip8_core_t* core, uint8_t code[2], opcode_8_instruction_t* instruction){
    uint16_t op = ((code[0] & 0xF0) << 8) | ((code[1] & 0x0F));

    switch(op){
        case _set_vx_vy :
            core->V[instruction->param[0]].data = core->V[instruction->param[1]].data;
            core->pc.data += 2;
            break;

        case _set_vx_vx_or_vy :
            core->V[instruction->param[0]].data |= core->V[instruction->param[1]].data;
            core->pc.data += 2;
            break;

        case _set_vx_vx_and_vy :
            core->V[instruction->param[0]].data &= core->V[instruction->param[1]].data;
            core->pc.data += 2;
            break;

        case _set_vx_vx_xor_vy :
            core->V[instruction->param[0]].data ^= core->V[instruction->param[1]].data;
            core->pc.data += 2;
            break;

        case _add_vy_vx :
            if(core->V[instruction->param[0]].data + core->V[instruction->param[1]].data > (~0x00)){
                core->V[0xF].data = 1;
            } else {
                core->V[0xF].data = 0;
            }

            core->V[instruction->param[0]].data += core->V[instruction->param[1]].data;

            core->pc.data += 2;
            break;

        case _sub_vy_vx :
            if(core->V[instruction->param[0]].data - core->V[instruction->param[1]].data > (~0x00)){
                core->V[0xF].data = 0;
            } else {
                core->V[0xF].data = 1;
            }

            core->V[instruction->param[0]].data -= core->V[instruction->param[1]].data;

            core->pc.data += 2;
            break;

        case _shiftr_vy_vx :
            core->V[instruction->param[0]].data = core->V[instruction->param[1]].data >> 1;
            core->V[0xF].data = core->V[instruction->param[1]].data & 0x01;
            core->pc.data += 2;
            break;

        case _set_vx_vy_vx :
            if(core->V[instruction->param[1]].data - core->V[instruction->param[0]].data > (~0x00)){
                core->V[0xF].data = 0;
            } else {
                core->V[0xF].data = 1;
            }

            core->V[instruction->param[0]].data = core->V[instruction->param[1]].data -
                    core->V[instruction->param[0]].data;

            core->pc.data += 2;
            break;

        case _shiftl_vy_vx :
            core->V[instruction->param[0]].data = core->V[instruction->param[1]].data << 1;
            core->V[0xF].data = core->V[instruction->param[1]].data & 0x80;
            core->pc.data += 2;
            break;
    }

}

void chip8_execute0(chip8_core_t* core, uint8_t code[2], opcode_0_instruction_t* instruction){
}

void chip8_executeE(chip8_core_t* core, uint8_t code[2], opcode_E_instruction_t* instruction){
    uint16_t op = ((code[0] & 0xF0) << 8) | ((code[1] & 0xFF));

    switch(op){
        case _skip_vx_pressed : break;
        case _skip_vx_released : break;
    }
}

void chip8_executeF(chip8_core_t* core, uint8_t code[2], opcode_F_instruction_t* instruction){
    uint16_t op = ((code[0] & 0xF0) << 8) | ((code[1] & 0xFF));

    switch(op) {
        case _set_vx_time : break;
        case _key_wait : break;
        case _set_delay_vx : break;
        case _set_time_vx : break;
        case _add_vx_i :
            core->I.data += core->V[instruction->param].data;
            core->pc.data += 2;
            break;

        case _set_i_sprite_vx :
            printf("got it!%d\n\n", instruction->param);
            core->pc.data += 2;
            break;

        case _sore_bcd_vx :
            core->memory[core->I.data + 0] = (core->V[instruction->param].data / 100) % 10;
            core->memory[core->I.data + 1] = (core->V[instruction->param].data / 10) % 10;
            core->memory[core->I.data + 2] = (core->V[instruction->param].data) % 10;
            core->pc.data += 2;
            break;

        case _store_v0_vx : break;
        case _fill_v0_vx_i : break;
    }
}


void chip8_rtomem(chip8_core_t* core, const char* filename){
    // TODO read file to memory
}