#include <gtest/gtest.h>
#include <iostream>

#include "Interpreter.hpp"
#include "Decoder.hpp"
#include "Encoder.hpp"
#include "Opcodes.hpp"
#include "Formats.hpp"
#include "Operations.hpp"
#include "HandlerFactory.hpp"

#include "Handlers.hpp"

using namespace rv32i;

class Rv32iTest : public ::testing::Test 
{
protected:
    Interpreter cpu;

    void SetUp() override 
    {
        register_all_handlers(cpu); 

        cpu.state.pc = 0x1000;

        for (auto& r : cpu.state.regs) r = 0;

        cpu.state.memory.clear();
    }

    void run(u32 instr) 
    {
        auto [info, key] = Decoder::decode(instr, cpu.pc());
        cpu.dispatch(cpu.state, info, key);
    }
};

TEST_F(Rv32iTest, MUL_MultipliesRegisters)
{
    // x1 = 6, x2 = 7
    cpu.state.regs[1] = 6u;
    cpu.state.regs[2] = 7u;

    // mul x3, x1, x2
    // opcode=0x33 (R_TYPE), funct7=0x01, funct3=0x0
    REncoding e{/*funct7=*/0x01,
                /*rs2=*/2,
                /*rs1=*/1,
                /*funct3=*/0x0,
                /*rd=*/3,
                /*opcode=*/Opcode::R_TYPE};
    run(encode(e));

    EXPECT_EQ(cpu.state.regs[3], 42u);
    EXPECT_EQ(cpu.state.pc, 0x1004u);
}

TEST_F(Rv32iTest, MULH_HighSignedSigned)
{
    // a = -2, b = 3
    cpu.state.regs[1] = 0xFFFFFFFEu; // -2 as u32
    cpu.state.regs[2] = 3u;

    // mulh x3, x1, x2
    // funct3 = 0x1
    REncoding e{0x01, 2, 1, 0x1, 3, Opcode::R_TYPE};
    run(encode(e));

    // Expected:
    // (int64_t)(-2) * (int64_t)(3) = -6 = 0xFFFFFFFFFFFFFFFA
    // high 32 bits = 0xFFFFFFFF
    EXPECT_EQ(cpu.state.regs[3], 0xFFFFFFFFu);
}

TEST_F(Rv32iTest, MULHSU_HighSignedUnsigned)
{
    // a = -2 (signed), b = 3 (unsigned)
    cpu.state.regs[1] = 0xFFFFFFFEu; // -2
    cpu.state.regs[2] = 3u;

    // mulhsu x3, x1, x2
    // funct3 = 0x2
    REncoding e{0x01, 2, 1, 0x2, 3, Opcode::R_TYPE};
    run(encode(e));

    // (-2 * 3) = -6 = 0xFFFFFFFFFFFFFFFA
    // high 32 bits = 0xFFFFFFFF
    EXPECT_EQ(cpu.state.regs[3], 0xFFFFFFFFu);
}

TEST_F(Rv32iTest, MULHU_HighUnsignedUnsigned)
{
    // a = 0x80000000, b = 2
    cpu.state.regs[1] = 0x80000000u;
    cpu.state.regs[2] = 2u;

    // Product = 0x80000000 * 2 = 0x1_00000000 (64-bit)
    // high 32 bits = 0x00000001

    // mulhu x3, x1, x2
    // funct3 = 0x3
    REncoding e{0x01, 2, 1, 0x3, 3, Opcode::R_TYPE};
    run(encode(e));

    EXPECT_EQ(cpu.state.regs[3], 0x00000001u);
}

TEST_F(Rv32iTest, DIV_SignedDivision)
{
    // a = 7, b = -2
    cpu.state.regs[1] = 7u;
    cpu.state.regs[2] = 0xFFFFFFFEu; // -2

    // div x3, x1, x2
    // funct3 = 0x4
    REncoding e{0x01, 2, 1, 0x4, 3, Opcode::R_TYPE};
    run(encode(e));

    // 7 / -2 = -3 (truncate toward 0)
    EXPECT_EQ(cpu.state.regs[3], 0xFFFFFFFDu); // -3 as u32
}

TEST_F(Rv32iTest, DIV_DivByZeroYieldsMinusOne)
{
    cpu.state.regs[1] = 123u; // dividend
    cpu.state.regs[2] = 0u;   // divisor = 0

    REncoding e{0x01, 2, 1, 0x4, 3, Opcode::R_TYPE};
    run(encode(e));

    // Spec: result = -1
    EXPECT_EQ(cpu.state.regs[3], 0xFFFFFFFFu);
}

TEST_F(Rv32iTest, DIVU_UnsignedDivision)
{
    cpu.state.regs[1] = 7u;
    cpu.state.regs[2] = 2u;

    // divu x3, x1, x2
    // funct3 = 0x5
    REncoding e{0x01, 2, 1, 0x5, 3, Opcode::R_TYPE};
    run(encode(e));

    // 7 / 2 = 3
    EXPECT_EQ(cpu.state.regs[3], 3u);
}

TEST_F(Rv32iTest, DIVU_DivByZeroYieldsAllOnes)
{
    cpu.state.regs[1] = 123u;
    cpu.state.regs[2] = 0u;

    REncoding e{0x01, 2, 1, 0x5, 3, Opcode::R_TYPE};
    run(encode(e));

    // Spec: result = 0xFFFFFFFF
    EXPECT_EQ(cpu.state.regs[3], 0xFFFFFFFFu);
}

TEST_F(Rv32iTest, REM_SignedRemainder)
{
    // a = 7, b = -2
    cpu.state.regs[1] = 7u;
    cpu.state.regs[2] = 0xFFFFFFFEu; // -2

    // rem x3, x1, x2
    // funct3 = 0x6
    REncoding e{0x01, 2, 1, 0x6, 3, Opcode::R_TYPE};
    run(encode(e));

    // 7 % -2 = 1 (same sign as dividend 7)
    EXPECT_EQ(cpu.state.regs[3], 1u);
}

TEST_F(Rv32iTest, REM_DivByZeroYieldsDividend)
{
    cpu.state.regs[1] = 123u; // dividend
    cpu.state.regs[2] = 0u;   // divisor

    REncoding e{0x01, 2, 1, 0x6, 3, Opcode::R_TYPE};
    run(encode(e));

    // Spec: remainder = dividend
    EXPECT_EQ(cpu.state.regs[3], 123u);
}

TEST_F(Rv32iTest, REMU_UnsignedRemainder)
{
    cpu.state.regs[1] = 7u;
    cpu.state.regs[2] = 2u;

    // remu x3, x1, x2
    // funct3 = 0x7
    REncoding e{0x01, 2, 1, 0x7, 3, Opcode::R_TYPE};
    run(encode(e));

    // 7 % 2 = 1
    EXPECT_EQ(cpu.state.regs[3], 1u);
}

TEST_F(Rv32iTest, REMU_DivByZeroYieldsDividend)
{
    cpu.state.regs[1] = 123u;
    cpu.state.regs[2] = 0u;

    REncoding e{0x01, 2, 1, 0x7, 3, Opcode::R_TYPE};
    run(encode(e));

    // Spec: remainder = dividend (unsigned case)
    EXPECT_EQ(cpu.state.regs[3], 123u);
}



