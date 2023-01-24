#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <string>

// Flags
typedef struct flags_t {
    bool T;   // T Flag
    bool H;   // Half Carry Flag
    bool S;   // Sign Flag
    bool V;   // Two's complement overflow indicator
    bool N;   // Negative Flag
    bool Z;   // Zero Flag
    bool C;   // Carry Flag
    bool I;   // Global Interrupt Enable/Disable Flag
};

class ATtiny13A_CPU {
    int program_size = 0;

    // 6502 registers
    std::array<uint8_t, 32> registers;

    uint16_t* X = (uint16_t*)(registers[26]);
    uint16_t* Y = (uint16_t*)(registers[28]);
    uint16_t* Z = (uint16_t*)(registers[30]);

    // 6502 memory
    std::vector<uint8_t> memory;

    // 6502 program counter
    uint16_t pc = 0;

    // 6502 status register
    uint8_t sr = 0;

    // 6502 stack pointer
    uint8_t sp = 0;

    flags_t flags;

public:
    // AVR instruction decode
    void decode_and_execute(uint8_t* byte_array, flags_t* flags);

    ATtiny13A_CPU();
    // Constructor for custom memory size (0x10000 == 64Kbytes of memory)
    ATtiny13A_CPU(unsigned int memory_size);

    // Initialize the CPU
    void init();

    // Function to execute a single instruction
    void execute_instruction(uint8_t* instruction);

    // Function to load the compile binary file
    uint8_t* load_compiled_file(char* filePath);

    // Function to load a program into memory
    void load_program(uint8_t* program, int program_size);

    // Function to run the program loaded into memory
    void run_program();

    // Function to read a register
    uint8_t read_register(int index);

    // Function to write to a register
    void write_register(int index, uint8_t value);

    // Function to read a flag
    bool read_flag(int index);

    // Function to get the Flags
    flags_t get_flags();

    // Function to read memory
    uint8_t read_memory(int address);

    // Function to write to memory
    void write_memory(int address, uint8_t value);
};