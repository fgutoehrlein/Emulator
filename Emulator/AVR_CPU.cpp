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
		registries[Rd] = registries[Rr];
		break;

	case 0x02: // MULS
		// Multiply signed
		int16_t result = (int8_t)registries[Rd] * (int8_t)registries[Rr];

		//break apart result into lower and higher
		registries[0] = (result & 0xFF); // low byte = 0x34
		registries[1] = ((result >> 8) & 0xFF); // high byte = 0x12
		break;

	case 0x03: // FMUL
		// Fractional Multiply Unsigned
		uint16_t result = (uint8_t)registries[Rd] * (uint8_t)registries[Rr];
		registries[0] = (result & 0xFF);
		registries[1] = ((result >> 8) & 0xFF);
		registries[Rd] = (result & 0x00FF);
		registries[Rd + 1] = ((result >> 8) & 0x00FF);
		break;

	case 0x04: // FMULS
		// Fractional Multiply signed
		int16_t result = (int8_t)registries[Rd] * (int8_t)registries[Rr];
		registries[0] = (result & 0xFF);
		registries[1] = ((result >> 8) & 0xFF);
		registries[Rd] = (result & 0x00FF);
		registries[Rd + 1] = ((result >> 8) & 0x00FF);
		break;

	case 0x05: // FMULSU
		// Fractional Multiply signed with unsigned
		int16_t result = (int8_t)registries[Rd] * (uint8_t)registries[Rr];
		registries[0] = (result & 0xFF);
		registries[1] = ((result >> 8) & 0xFF);
		registries[Rd] = (result & 0x00FF);
		registries[Rd + 1] = ((result >> 8) & 0x00FF);
		break;

	case 0x06: // LDI
		// Load immediate
		registries[Rd] = Rr;
		break;

	case 0x07: // LDS
		// Load direct from data space
		registries[Rd] = mem[Rr];
		break;

	case 0x08: // LDS
		// Store direct to data space
		mem[Rr] = registries[Rd];
		break;

	case 0x09: // LPM
		// Load program memory
		registries[0] = mem[registries[Rd]];
		break;

	case 0x0A: // SPM
		// Store Program Memory
		uint16_t address = registries[Rd];
		uint8_t data = registries[Rr];
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
		registries[i] = 0;
	}
	// Clear memory
	for (int i = 0; i < 65536; i++) {
		mem[i] = 0;
	}
	pc = 0;
	sr = 0;
	sp = 0;
}