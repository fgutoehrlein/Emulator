#include <iostream>
#include <array>
#include "AVR_CPU.h"


ATtiny13A_CPU::ATtiny13A_CPU() {
	memory.resize(65536);
	init();
}

ATtiny13A_CPU::ATtiny13A_CPU(unsigned int memory_size) {
	memory.resize(memory_size);
	init();
}

void ATtiny13A_CPU::init() {
	// Clear all registers
	for (int i = 0; i < 32; i++) {
		registers[i] = 0;
	}

	// Clear memory
	for (int i = 0; i < 1024; i++) {
		memory[i] = 0;
	}

	//Initialize Pointers
	pc = 0;
	sr = 0;
	sp = 0;

	// Initialize flags
	flags.C = 0;
	flags.Z = 0;
	flags.N = 0;
	flags.V = 0;
	flags.S = 0;
	flags.H = 0;
	flags.T = 0;
	flags.I = 0;
}

void ATtiny13A_CPU::execute_instruction(uint8_t* instruction) {
	// Decode and execute instruction
	decode_and_execute(instruction, &flags);
}

uint8_t* ATtiny13A_CPU::load_compiled_file(char* filePath) {
	std::ifstream binaryFile("path/to/binary/file", std::ios::binary);
	if (!binaryFile) {
		std::cout << "file not found" << std::endl;
		return nullptr;
	}

	binaryFile.seekg(0, std::ios::end);
	size_t size = binaryFile.tellg();
	binaryFile.seekg(0, std::ios::beg);

	uint8_t* buffer = new uint8_t[size];
	binaryFile.read((char*)buffer, size);
	binaryFile.close();

	// buffer now contains the contents of the binary file as a uint8_t array
	// size contains the size of the file in bytes

	this->program_size = size;
	return buffer;
}


void ATtiny13A_CPU::load_program(uint8_t* program, int program_size) {
	for (int i = 0; i < program_size; i++) {
		memory[i] = program[i];
	}
}

void ATtiny13A_CPU::run_program() {
	for (int i = 0; i < program_size; i++) {
		execute_instruction(&memory[i]);
	}
}

uint8_t ATtiny13A_CPU::read_register(int index) {
	return registers[index];
}

void ATtiny13A_CPU::write_register(int index, uint8_t value) {
	registers[index] = value;
}

uint8_t ATtiny13A_CPU::read_memory(int address) {
	return memory[address];
}

void ATtiny13A_CPU::write_memory(int address, uint8_t value) {
	memory[address] = value;
}

flags_t ATtiny13A_CPU::get_flags() {
	return this->flags;
}

void ATtiny13A_CPU::decode_and_execute(uint8_t* byte_array, flags_t* flags) {
	uint8_t instruction = byte_array[0];
	uint8_t Rd = byte_array[1] >> 4;
	uint8_t Rr = byte_array[1] & 0x0F;

	switch (instruction) {
	case 0x00: { // NOP
		// No operation
		break;
	}

			 // SBC instruction
	case 0x08: {
		uint8_t reg_a = registers[byte_array[1]];
		uint8_t reg_b = registers[byte_array[2]];
		uint16_t result = reg_a - reg_b - (1 - flags->C);
		flags->C = (result < 0xff);
		flags->Z = (result == 0);
		flags->N = (result >> 7);
		flags->V = (((reg_a ^ reg_b) & (reg_a ^ result)) >> 7);
		registers[byte_array[1]] = result & 0xff;
		break;
	}

			 // SUB instruction
	case 0x18: {
		registers[byte_array[1]] -= registers[byte_array[2]];
		flags->C = ((registers[byte_array[1]] - registers[byte_array[2]]) < 0);
		flags->Z = ((registers[byte_array[1]] - registers[byte_array[2]]) == 0);
		flags->N = (registers[byte_array[1]] >> 7);
		break;
	}

			 // ADC instruction
	case 0x1A: {
		uint8_t Rd = byte_array[1] >> 4;
		uint8_t Rr = byte_array[1] & 0x0F;

		// Execute the ADC instruction
		uint8_t result = registers[Rr] + registers[Rd] + flags->C;
		registers[Rd] = result;
		flags->C = (registers[Rr] > (result - flags->C));
		flags->Z = (result == 0);
		flags->N = (result >> 7);
		flags->V = ((registers[Rr] >> 7 == (registers[Rd] >> 7)) && ((registers[Rd] >> 7) != (result >> 7)));
		flags->H = (((registers[Rr] & 0x0F) + (registers[Rd] & 0x0F) + flags->C) > 0x0F);
		break;
	}

	case 0x0C: { //ADD Instruction
		registers[byte_array[1]] += registers[byte_array[2]];
		flags->C = ((registers[byte_array[1]] + registers[byte_array[2]]) > 0xff);
		flags->Z = ((registers[byte_array[1]] + registers[byte_array[2]]) == 0);
		flags->N = (registers[byte_array[1]] >> 7);
		break;
	}

	default: {
		// Handle unknown opcode
		break;
	}
	}
}