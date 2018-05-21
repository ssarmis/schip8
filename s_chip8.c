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

void chip8_core_init(chip8_core_t* core){
    core->I.data = 0;
    core->pc.data = 0x200; // 512 offset
    core->sp.data = 0;

    // test 0xF029
    core->memory[core->pc.data]     = 0XF0;
    core->memory[core->pc.data + 1] = 0x29;
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
                _instruction.param[1] = (opcode[1] & 0xF0);
                _instruction.param[2] = (opcode[1] & 0x0F);
                chip8_execute(core, opcode, &_instruction);
                break;
        }
    }

    // TODO make this in a different way
}

void chip8_execute(chip8_core_t* core, uint8_t code[2], opcode_instruction_t* instruction){
    uint16_t op = code[0] & 0x000F;

    uint8_t a = code[0] & 0x0F;
    uint8_t b = code[1] & 0xFF;

    switch(op){
        case _goto :
            printf("jumping to address: %x\n\n", ((a << 8) | b));

            core->pc.data = ((a << 8) | b);
            break;

        case _call : break;
        case _skip_vx_nn :
            if(core->V[a].data == b){
                core->pc.data += 4;
            } else {
                core->pc.data += 2;
            }
            break;

        case _skip_vx_n_nn :
            if(core->V[a].data != b){
                core->pc.data += 4;
            } else {
                core->pc.data += 2;
            }
            break;

        case _skip_vx_vy :
            if(core->V[a].data == core->V[((b & 0xF0) >> 4)].data) {
                core->pc.data += 4;
            } else {
                core->pc.data += 2;
            }
            break;

        case _set_vx :
            core->V[a].data = b;

            core->pc.data += 2;
            break;

        case _add_nn_vx :
            core->V[a].data += b;

            core->pc.data += 2;
            break;

        case _skip_vx_n_vy :
            if(core->V[a].data != core->V[((b & 0xF0) >> 4)].data) {
                core->pc.data += 4;
            } else {
                core->pc.data += 2;
            }
            break;

        case _set_I :
            core->I.data = ((a << 8) | b);

            core->pc.data += 2;
            break;

        case _jump_nnn_v0 :
            core->pc.data += ((a << 8) | b) + core->V[0].data;
            break;

        case _set_vx_rand_nn :
            core->V[a].data = ((rand() % 255) & b);
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

    uint8_t a = code[0] & 0x0F;
    uint8_t b = code[1] & 0xF0;

    switch(op){
        case _set_vx_vy :
            core->V[a].data = core->V[b].data;
            core->pc.data += 2;
            break;

        case _set_vx_vx_or_vy :
            core->V[a].data |= core->V[b].data;
            core->pc.data += 2;
            break;

        case _set_vx_vx_and_vy :
            core->V[a].data &= core->V[b].data;
            core->pc.data += 2;
            break;

        case _set_vx_vx_xor_vy :
            core->V[a].data ^= core->V[b].data;
            core->pc.data += 2;
            break;

        case _add_vy_vx :
            if(core->V[a].data + core->V[b].data > (~0x00)){
                core->V[0xF].data = 1;
            } else {
                core->V[0xF].data = 0;
            }

            core->V[a].data += core->V[b].data;

            core->pc.data += 2;
            break;

        case _sub_vy_vx :
            if(core->V[a].data - core->V[b].data > (~0x00)){
                core->V[0xF].data = 0;
            } else {
                core->V[0xF].data = 1;
            }

            core->V[a].data -= core->V[b].data;

            core->pc.data += 2;
            break;

        case _shiftr_vy_vx :
            core->V[a].data = core->V[b].data >> 1;
            core->V[0xF].data = core->V[b].data & 0x01;
            core->pc.data += 2;
            break;

        case _set_vx_vy_vx :
            if(core->V[b].data - core->V[a].data > (~0x00)){
                core->V[0xF].data = 0;
            } else {
                core->V[0xF].data = 1;
            }

            core->V[a].data = core->V[b].data - core->V[a].data;

            core->pc.data += 2;
            break;

        case _shiftl_vy_vx :
            core->V[a].data = core->V[b].data << 1;
            core->V[0xF].data = core->V[b].data & 0x80;
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

    uint8_t a = (code[0] & 0x0F);

    switch(op) {
        case _set_vx_time : break;
        case _key_wait : break;
        case _set_delay_vx : break;
        case _set_time_vx : break;
        case _add_vx_i :
            core->I.data += core->V[a].data;
            core->pc.data += 2;
            break;

        case _set_i_sprite_vx :
            printf("got it!\n\n");
            core->pc.data += 2;
            break;

        case _sore_bcd_vx :
            core->memory[core->I.data + 0] = (core->V[a].data / 100) % 10;
            core->memory[core->I.data + 1] = (core->V[a].data / 10) % 10;
            core->memory[core->I.data + 2] = (core->V[a].data) % 10;
            core->pc.data += 2;
            break;

        case _store_v0_vx : break;
        case _fill_v0_vx_i : break;
    }
}


void chip8_rtomem(chip8_core_t* core, const char* filename){
    // TODO read file to memory
}