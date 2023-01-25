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
    EXPECT_EQ(cpu->read_flag(flags.C), false); // Carry flag
    EXPECT_EQ(cpu->read_flag(flags.Z), false); // Zero flag
    EXPECT_EQ(cpu->read_flag(flags.N), false); // Negative flag
}

TEST_F(ATtiny13A_CPU_Test, TestDecodeAndExecuteAdd_Carry) {
    uint8_t instruction[] = { 0x0C, 0x01, 0x02 }; // ADD instruction
    cpu->write_register(1, 250);
    cpu->write_register(2, 20);
    flags_t flags = cpu->get_flags();
    cpu->decode_and_execute(instruction, &flags);
    EXPECT_EQ(cpu->read_register(1), (250 + 20));
    EXPECT_EQ(flags.C, true);  // Carry flag
    EXPECT_EQ(flags.Z, false); // Zero flag
    EXPECT_EQ(flags.N, false); // Negative flag
}

TEST_F(ATtiny13A_CPU_Test, TestDecodeAndExecuteADC) {
    uint8_t instruction[] = { 0x03, 0x01, 0x02 }; // ADC instruction
    cpu->write_register(1, 10);
    cpu->write_register(2, 20);
    flags_t flags = cpu->get_flags();
    flags.C = 1;
    cpu->decode_and_execute(instruction, &flags);
    EXPECT_EQ(cpu->read_register(1), (10 + 20 + 1));
    EXPECT_EQ(flags.C, false); // Carry flag
    EXPECT_EQ(flags.Z, false); // Zero flag
    EXPECT_EQ(flags.N, false); // Negative flag
    EXPECT_EQ(flags.V, false); // Two's complement overflow indicator

    EXPECT_EQ(cpu->read_flag(6), false); // Carry flag
    EXPECT_EQ(cpu->read_flag(5), false); // Zero flag
    EXPECT_EQ(cpu->read_flag(4), false); // Negative flag
    EXPECT_EQ(cpu->read_flag(3), false); // Two's complement overflow indicator
}

TEST_F(ATtiny13A_CPU_Test, TestDecodeAndExecuteADC_Carry) {
    uint8_t instruction[] = { 0x03, 0x01, 0x02 }; // ADC instruction
    cpu->write_register(1, 250);
    cpu->write_register(2, 20);
    flags_t flags = cpu->get_flags();
    flags.C = 1;
    cpu->decode_and_execute(instruction, &flags);
    EXPECT_EQ(cpu->read_register(1), (250 + 20 + 1));
    EXPECT_EQ(flags.C, true);  // Carry flag
    EXPECT_EQ(flags.Z, false); // Zero flag
    EXPECT_EQ(flags.N, false); // Negative flag
    EXPECT_EQ(flags.V, false); // Two's complement overflow indicator
}

TEST_F(ATtiny13A_CPU_Test, TestDecodeAndExecuteSUB) {
    uint8_t instruction[] = { 0x02, 0x01, 0x02 }; // SUB instruction
    cpu->write_register(1, 20);
    cpu->write_register(2, 10);
    flags_t flags = cpu->get_flags();
    cpu->decode_and_execute(instruction, &flags);
    EXPECT_EQ(cpu->read_register(1), (20 - 10));
    EXPECT_EQ(flags.C, true);  // Carry flag
    EXPECT_EQ(flags.Z, false); // Zero flag
    EXPECT_EQ(flags.N, false); // Negative flag
    EXPECT_EQ(flags.V, false); // Two's complement overflow indicator
}

TEST_F(ATtiny13A_CPU_Test, TestDecodeAndExecuteSUB_Carry) {
    uint8_t instruction[] = { 0x02, 0x01, 0x02 }; // SUB instruction
    cpu->write_register(1, 10);
    cpu->write_register(2, 20);
    flags_t flags = cpu->get_flags();
    flags.C = 0;
    cpu->decode_and_execute(instruction, &flags);
    EXPECT_EQ(cpu->read_register(1), (10 - 20));
    EXPECT_EQ(flags.C, false); // Carry flag
    EXPECT_EQ(flags.Z, false); // Zero flag
    EXPECT_EQ(flags.N, true);  // Negative flag
    EXPECT_EQ(flags.V, false); // Two's complement overflow indicator
}

TEST_F(ATtiny13A_CPU_Test, TestDecodeAndExecuteADIW_X) {
    uint8_t instruction[2] = { 0x96, 0x05 };
    flags_t flags = cpu->get_flags();
    cpu->write_register(26, 0x10);
    cpu->decode_and_execute(instruction, &flags);
    EXPECT_EQ(cpu->read_register(26), 0x15);
    EXPECT_FALSE(cpu->read_flag(6)); //Carry flag
    EXPECT_FALSE(cpu->read_flag(5)); // Zero flag
    EXPECT_FALSE(cpu->read_flag(4)); // Negative flag
    EXPECT_FALSE(cpu->read_flag(3)); // Two's complement overflow indicator
}

TEST_F(ATtiny13A_CPU_Test, TestDecodeAndExecuteADIW_Y) {
    uint8_t instruction[2] = { 0x97, 0x05 };
    flags_t flags = cpu->get_flags();
    cpu->write_register(28, 0x10);
    cpu->decode_and_execute(instruction, &flags);
    EXPECT_EQ(cpu->read_register(28), 0x15);
    EXPECT_FALSE(cpu->read_flag(6)); // Carry flag
    EXPECT_FALSE(cpu->read_flag(5)); // Zero flag
    EXPECT_FALSE(cpu->read_flag(4)); // Negative flag
    EXPECT_FALSE(cpu->read_flag(3)); // Two's complement overflow indicator
}

TEST_F(ATtiny13A_CPU_Test, TestDecodeAndExecuteADIW_Z) {
    uint8_t instruction[2] = { 0x98, 0x05 };
    flags_t flags = cpu->get_flags();
    cpu->write_register(30, 0x10);
    cpu->decode_and_execute(instruction, &flags);
    EXPECT_EQ(cpu->read_register(30), 0x15);
    EXPECT_FALSE(cpu->read_flag(6)); //Carry flag
    EXPECT_FALSE(cpu->read_flag(5)); // Zero flag
    EXPECT_FALSE(cpu->read_flag(4)); // Negative flag
    EXPECT_FALSE(cpu->read_flag(3)); // Two's complement overflow indicator
}

TEST_F(ATtiny13A_CPU_Test, TestDecodeAndExecuteADIW_Carry) {
    uint8_t instruction[2] = { 0x96, 0xf0 };
    flags_t flags = cpu->get_flags();
    cpu->write_register(26, 0xff);
    cpu->decode_and_execute(instruction, &flags);
    EXPECT_EQ(cpu->read_register(26), 0xef);
    EXPECT_TRUE(cpu->read_flag(6)); //Carry flag
    EXPECT_FALSE(cpu->read_flag(5)); // Zero flag
    EXPECT_FALSE(cpu->read_flag(4)); // Negative flag
    EXPECT_FALSE(cpu->read_flag(3)); // Two's complement overflow indicator
}


//And so on for all other instructions