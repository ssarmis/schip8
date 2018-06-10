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

/*
 * Keyboard
 *
 * 1  2  3  C
 * 4  5  6  D
 * 7  8  9  E
 * A  0  B  F
 *
 */

void chip8_core_init(chip8_core_t* core){
    int i;

    for(i = 0; i < 16; ++i) {
        core->keys[i] = 0;
    }

    for(i = 0; i < 16; ++i) {
        core->stack[i] = 0;
    }

    for(i = 0; i < 64 * 32; ++i){
        core->display[i] = 0;
    }

    for(i = 0; i < 16; ++i) {
        core->V[i].data = 0;
    }

    for(i = 0; i < 4096; ++i) {
        core->memory[i] = 0;
    }

    for(i = 0; i < 80; ++i){
        core->memory[i] = chip8_fontset[i];
    }
    core->delay = 0;
    core->sound = 0;
    core->I.data = 0;
    core->pc.data = 0x200; // 512 offset
    core->sp.data = 0;

    //testing
    /*
    core->memory[0x0200] = 0xA2;
    core->memory[0x0200 + 1] = 0x0A;
    core->memory[0x0202] = 0x60;
    core->memory[0x0202 + 1] = 0x0A;
    core->memory[0x0204] = 0x61;
    core->memory[0x0204 + 1] = 0x05;
    core->memory[0x0206] = 0xD0;
    core->memory[0x0206 + 1] = 0x17;
    core->memory[0x0208] = 0x12;
    core->memory[0x0208 + 1] = 0x08;
    core->memory[0x020A] = 0x7C;
    core->memory[0x020B] = 0x40;
    core->memory[0x020C] = 0x40;
    core->memory[0x020D] = 0x7C;
    core->memory[0x020E] = 0x40;
    core->memory[0x020F] = 0x40;
    core->memory[0x0210] = 0x7C;
    */
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

#ifdef DEBUG_OPS
    printf("executing: %x\n", (opcode[0] << 8) | opcode[1]);
#endif

    if (!opcode[0]){
        chip8_execute0(core, opcode, &_0_instruction);
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

    if(core->delay > 0) {
        --core->delay;
    }

    if(core->sound > 0){
        // TODO implement sound?... beeps?
        --core->sound;
    }
}

void chip8_execute0(chip8_core_t* core, uint8_t code[2], opcode_0_instruction_t* instruction){
    uint16_t op = (code[0] << 8) | code[1];

    uint16_t i;

    switch(op){
        case _clear_screen : // 0x00E0
            for(i = 0; i < 64 * 32; ++i){
                core->display[i] = 0;
            }
            core->pc.data += 2;
            break;

        case _return_subroutine : // 0x00EE
            core->sp.data--;
            core->pc.data = core->stack[core->sp.data];
#ifdef DEBUG_OPS
            printf("Returned to %x\n", core->pc.data);
#endif
            core->pc.data += 2;
            break;

        default:
            printf("Unknown opcode: %x\n", (code[0] << 8) | code[1]);
            break;
    }
}

void chip8_execute(chip8_core_t* core, uint8_t code[2], opcode_instruction_t* instruction){
    uint16_t op = code[0] & 0x00F0;
    op >>= 4;

    switch(op){
        case _goto : // 0x1NNN
#ifdef DEBUG_OPS
            printf("Goto %x\n", ((instruction->param[0] << 8) | instruction->param[1]));
#endif
            core->pc.data = ((instruction->param[0] << 8) | instruction->param[1]);
            break;

        case _call : // 0x2NNN
#ifdef DEBUG_OPS
            printf("Call %x and need to return %x\n", ((instruction->param[0] << 8) | instruction->param[1]), core->pc.data);
#endif
            core->stack[core->sp.data] = core->pc.data;
            core->sp.data++;
            core->pc.data = ((instruction->param[0] << 8) | instruction->param[1]);
            break;

        case _skip_vx_nn : // 0x3XNN
            if(core->V[instruction->param[0]].data == instruction->param[1]){
                core->pc.data += 4;
            } else {
                core->pc.data += 2;
            }
            break;

        case _skip_vx_n_nn : // 0x4XNN
            if(core->V[instruction->param[0]].data != instruction->param[1]){
                core->pc.data += 4;
            } else {
                core->pc.data += 2;
            }
            break;

        case _skip_vx_vy : // 0x5XY0
            if(core->V[instruction->param[0]].data == core->V[((instruction->param[1] & 0xF0) >> 4)].data) {
                core->pc.data += 4;
            } else {
                core->pc.data += 2;
            }
            break;

        case _set_vx : // 0x6XNN
#ifdef DEBUG_OPS
            printf("Set V[%x] to %x\n", instruction->param[0], instruction->param[1]);
#endif
            core->V[instruction->param[0]].data = instruction->param[1];
            core->pc.data += 2;
            break;

        case _add_nn_vx : // 0x7XNN
#ifdef DEBUG_OPS
            printf("Add V[%x] to %x\n", instruction->param[0], instruction->param[1]);
#endif
            core->V[instruction->param[0]].data += instruction->param[1];
            core->pc.data += 2;
            break;

        case _skip_vx_n_vy : // 0x9XY0
            if(core->V[instruction->param[0]].data != core->V[((instruction->param[1] & 0xF0) >> 4)].data) {
                core->pc.data += 4;
            } else {
                core->pc.data += 2;
            }
            break;

        case _set_I : // 0xANNN
#ifdef DEBUG_OPS
            printf("Set I to %x\n", ((instruction->param[0] << 8) | instruction->param[1]));
#endif
            core->I.data = ((instruction->param[0] << 8) | instruction->param[1]);
            core->pc.data += 2;
            break;

        case _jump_nnn_v0 : // 0xBNNN
            core->pc.data = ((instruction->param[0] << 8) | instruction->param[1]) + core->V[0].data;
            break;

        case _set_vx_rand_nn :  // 0xCXNN
            core->V[instruction->param[0]].data = ((rand() % 255) & instruction->param[1]);
            core->pc.data += 2;
            break;

        case _draw : // 0xDXYN
            core->V[0xF].data = 0;
#ifdef DEBUG_DRAWING
            // TODO correct the printf parameters
            printf("Drawing something at: [%d, %d]%d, %d\n",
                   instruction->param[0], instruction->param[1],
                    core->V[instruction->param[0]].data, core->V[instruction->param[1]].data);
#endif
            uint8_t y;
            for(y = 0; y < (instruction->param[1] & 0x0F); ++y){
                uint8_t yo = y + ((core->V[(instruction->param[1] & 0xF0) >> 4].data));

                uint8_t px = core->memory[core->I.data + y];
                uint8_t x;
                for(x = 0; x < 8; ++x){
                    uint8_t xo = x + core->V[instruction->param[0]].data;

                    if((px & (0x80 >> x)) != 0){
                       if(core->display[xo + yo * 64] == 1){
                           core->V[0xF].data = 1;
                       }
                       core->display[xo + yo * 64] ^= 0x1;
                    }
                }
            }
            core->pc.data += 2;
            break;

        default:
            printf("Unknown opcode: %x\n", (code[0] << 8) | code[1]);
            break;
    }
}

void chip8_execute8(chip8_core_t* core, uint8_t code[2], opcode_8_instruction_t* instruction){
    uint16_t op = ((code[0] & 0xF0) << 8) | ((code[1] & 0x0F));

    switch(op){
        case _set_vx_vy : // 0x8XY0
            core->V[instruction->param[0]].data = core->V[instruction->param[1]].data;
#ifdef DEBUG_OPS
            printf("V[%d] = V[%d]\n", instruction->param[0], instruction->param[1]);
#endif
            core->pc.data += 2;
            break;

        case _set_vx_vx_or_vy :// 0x8XY1
            core->V[instruction->param[0]].data |= core->V[instruction->param[1]].data;
            core->pc.data += 2;
            break;

        case _set_vx_vx_and_vy :// 0x8XY2
            core->V[instruction->param[0]].data &= core->V[instruction->param[1]].data;
            core->pc.data += 2;
            break;

        case _set_vx_vx_xor_vy :// 0x8XY3
            core->V[instruction->param[0]].data ^= core->V[instruction->param[1]].data;
            core->pc.data += 2;
            break;

        case _add_vy_vx :// 0x8XY4
            if(core->V[instruction->param[1]].data > ((0xFF) - core->V[instruction->param[0]].data)){
                core->V[0xF].data = 1;
            } else {
                core->V[0xF].data = 0;
            }

            core->V[instruction->param[0]].data += core->V[instruction->param[1]].data;

            core->pc.data += 2;
            break;

        case _sub_vy_vx :// 0x8XY5
            if(core->V[instruction->param[1]].data > core->V[instruction->param[0]].data){
                core->V[0xF].data = 0;
            } else {
                core->V[0xF].data = 1;
            }

            core->V[instruction->param[0]].data -= core->V[instruction->param[1]].data;

            core->pc.data += 2;
            break;

        case _shiftr_vy_vx :// 0x8XY6
            core->V[0xF].data = core->V[instruction->param[0]].data & 0x01;
            core->V[instruction->param[0]].data >>= 1;
            core->pc.data += 2;
            break;

        case _set_vx_vy_vx :// 0x8XY7
            if(core->V[instruction->param[0]].data > core->V[instruction->param[1]].data){
                core->V[0xF].data = 0;
            } else {
                core->V[0xF].data = 1;
            }

            core->V[instruction->param[0]].data = core->V[instruction->param[1]].data -
                    core->V[instruction->param[0]].data;

            core->pc.data += 2;
            break;

        case _shiftl_vy_vx :// 0x8XYE
            core->V[0xF].data = core->V[instruction->param[0]].data >> 7;
            core->V[instruction->param[0]].data <<= 1;
            core->pc.data += 2;
            break;

        default:
            printf("Unknown opcode: %x\n", (code[0] << 8) | code[1]);
            break;
    }

}

void chip8_executeE(chip8_core_t* core, uint8_t code[2], opcode_E_instruction_t* instruction){
    uint16_t op = ((code[0] & 0xF0) << 8) | ((code[1] & 0xFF));

    switch(op){
        case _skip_vx_pressed :// 0xEX9E
            if(core->keys[core->V[instruction->param].data]) {
                core->pc.data += 4;
            } else {
                core->pc.data += 2;
            }
            break;

        case _skip_vx_released :// 0xEXA1
            if(!core->keys[core->V[instruction->param].data]) {
                core->pc.data += 4;
            } else {
                core->pc.data += 2;
            }
            break;

        default:
            printf("Unknown opcode: %x\n", (code[0] << 8) | code[1]);
            break;
    }
}

void chip8_executeF(chip8_core_t* core, uint8_t code[2], opcode_F_instruction_t* instruction){
    uint16_t op = ((code[0] & 0xF0) << 8) | ((code[1] & 0xFF));

    uint8_t i;

    switch(op) {
        case _set_vx_time :// 0xFX07
            core->V[instruction->param].data = core->delay;
            core->pc.data += 2;
            break;

        case _key_wait :// 0xFX0A

            for(i = 0; i < 16; ++i){
                if(core->keys[i]){
                    core->V[instruction->param].data = i;
                    core->pc.data += 2;
                    break;
                }
            }

            return;

        case _set_delay_vx :// 0xFX15
#ifdef DEBUG_OPS
            printf("Set delay, pc at %x\n", core->pc.data);
#endif
            core->delay = core->V[instruction->param].data;
            core->pc.data += 2;
            break;

        case _set_time_vx :// 0xFX18
            core->sound = core->V[instruction->param].data;
            core->pc.data += 2;
            break;

        case _add_vx_i :// 0xFX1E
            if(core->I.data + core->V[instruction->param].data > 0xFFF){
                core->V[0xF].data = 1;
            } else {
                core->V[0xF].data = 0;
            }
            core->I.data += core->V[instruction->param].data;
            core->pc.data += 2;
            break;

        case _set_i_sprite_vx :// 0xFX29
            core->I.data = core->V[instruction->param].data * 0x5;
            core->pc.data += 2;
            break;

        case _sore_bcd_vx :// 0xFX33
            core->memory[core->I.data + 0] = (core->V[instruction->param].data / 100);
            core->memory[core->I.data + 1] = (core->V[instruction->param].data / 10) % 10;
            core->memory[core->I.data + 2] = (core->V[instruction->param].data % 100) % 10;
            core->pc.data += 2;
            break;

        case _store_v0_vx :// 0xFX55
            for (i = 0; i <= instruction->param; ++i)
                core->memory[core->I.data + i] = core->V[i].data;

            core->I.data += (instruction->param) + 1;
            core->pc.data += 2;
            break;

        case _fill_v0_vx_i :// 0xFX65
            for (i = 0; i <= instruction->param; ++i) {
                core->V[i].data = core->memory[core->I.data + i];
            }
            core->I.data += (instruction->param) + 1;
            core->pc.data += 2;
            break;

        default:
            printf("Unknown opcode: %x\n", (code[0] << 8) | code[1]);
            break;
    }
}


void chip8_rtomem(chip8_core_t* core, const char* filename){

    FILE* file;
    file = fopen(filename, "rb");

    fseek(file, 0, SEEK_END);

    long size = ftell(file);

    rewind(file);

    if(size % 2){
        printf("The file is not valid, incomplete codes, exiting.");
        chip8_shutdown(core);
        exit(0);
    }

    fread((&core->memory[512]), sizeof(uint8_t), size, file);

    fclose(file);
}

void chip8_shutdown(chip8_core_t* core){
    // I don't really need this, since there will be
    // no data leaks
    int i;

    for(i = 0; i < 16; ++i) {
        core->keys[i] = 0;
    }

    for(i = 0; i < 16; ++i) {
        core->stack[i] = 0;
    }

    for(i = 0; i < 64 * 32; ++i){
        core->display[i] = 0;
    }

    for(i = 0; i < 16; ++i) {
        core->V[i].data = 0;
    }

    for(i = 0; i < 4096; ++i) {
        core->memory[i] = 0;
    }

    core->delay = 0;
    core->sound = 0;
    core->I.data = 0;
    core->pc.data = 0;
    core->sp.data = 0;
}