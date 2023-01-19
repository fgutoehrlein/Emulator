#include <iostream>
#include <array>
#include "AVR_CPU.h"


// Run the CPU
void AVR_CPU::decode(uint16_t opcode) {
	uint8_t instruction = opcode & 0xFF;
	uint8_t Rd = (opcode >> 4) & 0x1F;
	uint8_t Rr = (opcode >> 0) & 0x1F;
	switch (instruction) {
	case 0x00: // NOP
		// No operation
		break;

	case 0x01: // MOVW
		// Move word
		regs[Rd] = regs[Rr];
		break;

	case 0x02: // MULS
		// Multiply signed
		int16_t result = (int8_t)regs[Rd] * (int8_t)regs[Rr];

		//break apart result into lower and higher
		regs[0] = (result & 0xFF); // low byte = 0x34
		regs[1] = ((result >> 8) & 0xFF); // high byte = 0x12
		break;

	case 0x03: // FMUL
		// Fractional Multiply Unsigned
		uint16_t result = (uint8_t)regs[Rd] * (uint8_t)regs[Rr];
		regs[0] = (result & 0xFF);
		regs[1] = ((result >> 8) & 0xFF);
		regs[Rd] = (result & 0x00FF);
		regs[Rd + 1] = ((result >> 8) & 0x00FF);
		break;

	case 0x04: // FMULS
		// Fractional Multiply signed
		int16_t result = (int8_t)regs[Rd] * (int8_t)regs[Rr];
		regs[0] = (result & 0xFF);
		regs[1] = ((result >> 8) & 0xFF);
		regs[Rd] = (result & 0x00FF);
		regs[Rd + 1] = ((result >> 8) & 0x00FF);
		break;

	case 0x05: // FMULSU
		// Fractional Multiply signed with unsigned
		int16_t result = (int8_t)regs[Rd] * (uint8_t)regs[Rr];
		regs[0] = (result & 0xFF);
		regs[1] = ((result >> 8) & 0xFF);
		regs[Rd] = (result & 0x00FF);
		regs[Rd + 1] = ((result >> 8) & 0x00FF);
		break;

	case 0x06: // LDI
		// Load immediate
		regs[Rd] = Rr;
		break;

	case 0x07: // LDS
		// Load direct from data space
		regs[Rd] = mem[Rr];
		break;

	case 0x08: // LDS
		// Store direct to data space
		mem[Rr] = regs[Rd];
		break;

	case 0x09: // LPM
		// Load program memory
		regs[0] = mem[regs[Rd]];
		break;

	case 0x0A: // SPM
		// Store Program Memory
		uint16_t address = regs[Rd];
		uint8_t data = regs[Rr];
		mem[address] = data;
		break;

	}
	pc++;
}
AVR_CPU::AVR_CPU() {
	mem.resize(65536);
	init();
}

AVR_CPU::AVR_CPU(unsigned int memory_size) {
	mem.resize(memory_size);
	init();
}


void AVR_CPU::init() {
	// Clear all registers
	for (int i = 0; i < 32; i++) {
		regs[i] = 0;
	}
	// Clear memory
	for (int i = 0; i < 65536; i++) {
		mem[i] = 0;
	}
	pc = 0;
	sr = 0;
	sp = 0;
}