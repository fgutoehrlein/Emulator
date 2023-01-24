// Emulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "AVR_CPU.h"

int main() {
    ATtiny13A_CPU cpu;
    cpu.init();
    cpu.run_program();

    return 0;
}
