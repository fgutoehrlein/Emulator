#include "pch.h"

class ATtiny13A_CPU_Test : public ::testing::Test {
protected:
    ATtiny13A_CPU* cpu;
    void SetUp() override {
        cpu = new ATtiny13A_CPU();
    }
    void TearDown() override {
        delete cpu;
    }
};

TEST_F(ATtiny13A_CPU_Test, TestDecodeAndExecuteAdd) {
    uint8_t instruction[] = { 0x0C, 0x01, 0x02 }; // ADD instruction
    cpu->write_register(1, 10);
    cpu->write_register(2, 20);
    flags_t flags = cpu->get_flags();
    cpu->decode_and_execute(instruction, &flags);
    EXPECT_EQ(cpu->read_register(1), 30);
    EXPECT_EQ(cpu->read_flag(flags.C), false);
    EXPECT_EQ(cpu->read_flag(flags.Z), false);
    EXPECT_EQ(cpu->read_flag(flags.N), false);
}

TEST_F(ATtiny13A_CPU_Test, TestDecodeAndExecuteAdd_Carry) {
    uint8_t instruction[] = { 0x0C, 0x01, 0x02 }; // ADD instruction
    cpu->write_register(1, 250);
    cpu->write_register(2, 20);
    flags_t flags = cpu->get_flags();
    cpu->decode_and_execute(instruction, &flags);
    EXPECT_EQ(cpu->read_register(1), (250 + 20));
    EXPECT_EQ(flags.C, true);
    EXPECT_EQ(flags.Z, false);
    EXPECT_EQ(flags.N, false);
}

TEST_F(ATtiny13A_CPU_Test, TestDecodeAndExecuteADC) {
    uint8_t instruction[] = { 0x03, 0x01, 0x02 }; // ADC instruction
    cpu->write_register(1, 10);
    cpu->write_register(2, 20);
    flags_t flags;
    flags.C = 1;
    cpu->decode_and_execute(instruction, &flags);
    EXPECT_EQ(cpu->read_register(1), (10 + 20 + 1));
    EXPECT_EQ(flags.C, false);
    EXPECT_EQ(flags.Z, false);
    EXPECT_EQ(flags.N, false);
    EXPECT_EQ(flags.V, false);

    EXPECT_EQ(cpu->read_flag(6), false);
    EXPECT_EQ(cpu->read_flag(5), false);
    EXPECT_EQ(cpu->read_flag(4), false);
    EXPECT_EQ(cpu->read_flag(3), false);
}

TEST_F(ATtiny13A_CPU_Test, TestDecodeAndExecuteADC_Carry) {
    uint8_t instruction[] = { 0x03, 0x01, 0x02 }; // ADC instruction
    cpu->write_register(1, 250);
    cpu->write_register(2, 20);
    flags_t flags;
    flags.C = 1;
    cpu->decode_and_execute(instruction, &flags);
    EXPECT_EQ(cpu->read_register(1), (250 + 20 + 1));
    EXPECT_EQ(flags.C, true);
    EXPECT_EQ(flags.Z, false);
    EXPECT_EQ(flags.N, false);
    EXPECT_EQ(flags.V, false);
}

TEST_F(ATtiny13A_CPU_Test, TestDecodeAndExecuteSUB) {
    uint8_t instruction[] = { 0x02, 0x01, 0x02 }; // SUB instruction
    cpu->write_register(1, 20);
    cpu->write_register(2, 10);
    flags_t flags;
    cpu->decode_and_execute(instruction, &flags);
    EXPECT_EQ(cpu->read_register(1), (20 - 10));
    EXPECT_EQ(flags.C, true);
    EXPECT_EQ(flags.Z, false);
    EXPECT_EQ(flags.N, false);
    EXPECT_EQ(flags.V, false);
}

TEST_F(ATtiny13A_CPU_Test, TestDecodeAndExecuteSUB_Carry) {
    uint8_t instruction[] = { 0x02, 0x01, 0x02 }; // SUB instruction
    cpu->write_register(1, 10);
    cpu->write_register(2, 20);
    flags_t flags;
    flags.C = 0;
    cpu->decode_and_execute(instruction, &flags);
    EXPECT_EQ(cpu->read_register(1), (10 - 20));
    EXPECT_EQ(flags.C, false);
    EXPECT_EQ(flags.Z, false);
    EXPECT_EQ(flags.N, true);
    EXPECT_EQ(flags.V, false);
}

//And so on for all other instructions