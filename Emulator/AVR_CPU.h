#include <iostream>
#include <array>
#include <vector>

class AVR_CPU {
    // 6502 registers
    std::array<uint8_t, 32> regs;

    // 6502 memory
    std::vector<uint8_t> mem;

    // 6502 program counter
    uint16_t pc = 0;

    // 6502 status register
    uint8_t sr = 0;

    // 6502 stack pointer
    uint8_t sp = 0;

    // AVR instruction decode
    void decode(uint16_t opcode);


public:
    AVR_CPU();
    // Constructor for custom memory size (0x10000 == 64Kbytes of memory)
    AVR_CPU(unsigned int memory_size);

    // Initialize the CPU
    void init();

    void run();

    uint8_t getReg(int reg);

    void setReg(int reg, uint8_t value);
};