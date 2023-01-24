// Emulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "AVR_CPU.h"

int main() {
    ATtiny13A_CPU cpu;
    cpu.init();
    //cpu.run_program();
    uint8_t byte_array[2] = { 0xff, 0xff };

    uint8_t var1 = 0xff;
    uint8_t var2 = 0xff;

    uint16_t var3 = byte_array[0] + byte_array[1];
    bool c = (var3 > 0xff);

    return 0;
}
