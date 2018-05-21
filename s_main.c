
#include <stdio.h>
#include "s_chip8.h"

int main() {

    chip8_core_t core;

    chip8_core_init(&core);

    chip8_step(&core);

    return 0;
}