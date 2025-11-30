#include <gtest/gtest.h>
#include <bit>
#include <cmath>

#include "Interpreter.hpp"
#include "Decoder.hpp"
#include "Encoder.hpp"
#include "Opcodes.hpp"
#include "Handlers.hpp"

using namespace rv32i;

static inline u32 fbits(float f)
{
    return std::bit_cast<u32>(f);
}

static inline float fval(u32 bits)
{
    return std::bit_cast<float>(bits);
}

class Rv32FTest : public ::testing::Test
{
protected:
    Interpreter cpu;

    void SetUp() override
    {
        register_all_handlers(cpu);
        cpu.state.pc = 0x1000;
        for (auto& r : cpu.state.regs) r = 0;
        for (auto& r : cpu.state.fregs) r = 0;
        cpu.state.memory.clear();
    }

    void run(u32 instr)
    {
        auto [info, key] = Decoder::decode(instr, cpu.pc());

        cpu.dispatch(cpu.state, info, key);
    }
};

TEST_F(Rv32FTest, FLW_LoadsFloatWord)
{
    const u32 base = 0x3000;
    cpu.state.memory.StoreU32(base, fbits(3.5f));
    cpu.state.regs[1] = base;

    // flw f2, 0(x1)
    IEncoding e{0, 1, 0x2, 2, Opcode::F_LOAD};
    run(encode(e));

    // expect f2 == 3.5
    EXPECT_EQ(cpu.state.fregs[2], fbits(3.5f));
}

TEST_F(Rv32FTest, FSW_StoresFloatWord)
{
    const u32 base = 0x3000;
    cpu.state.regs[1] = base;
    cpu.state.fregs[2] = fbits(-1.25f);

    // fsw f2, 4(x1)
    SEncoding se{4, 2, 1, 0x2, Opcode::F_STORE};
    run(encode(se));

    EXPECT_EQ(cpu.state.memory.LoadU32(base + 4), fbits(-1.25f));
}

TEST_F(Rv32FTest, FADD_FS_Sub_Mul_Div)
{
    // f1 = 1.25, f2 = 2.0
    cpu.state.fregs[1] = fbits(1.25f);
    cpu.state.fregs[2] = fbits(2.0f);

    auto near = [](float a, float b)
    {
        return std::fabs(a - b) < 1e-6f;
    };

    // fadd.s f3, f1, f2
    REncoding add{0x00, 2, 1, 0x0, 3, Opcode::F_OP};
    run(encode(add));
    EXPECT_TRUE(near(fval(cpu.state.fregs[3]), 3.25f));

    // fsub.s f4, f1, f2
    REncoding sub{0x04, 2, 1, 0x0, 4, Opcode::F_OP};
    run(encode(sub));
    EXPECT_TRUE(near(fval(cpu.state.fregs[4]), -0.75f));

    // fmul.s f5, f1, f2
    REncoding mul{0x08, 2, 1, 0x0, 5, Opcode::F_OP};
    run(encode(mul));
    EXPECT_TRUE(near(fval(cpu.state.fregs[5]), 2.5f));

    // fdiv.s f6, f2, f1
    REncoding div{0x0C, 1, 2, 0x0, 6, Opcode::F_OP};
    run(encode(div));
    EXPECT_TRUE(near(fval(cpu.state.fregs[6]), 1.6f));
}

TEST_F(Rv32FTest, FSQRT_ComputesSquareRoot)
{
    // f1 = 9.0
    cpu.state.fregs[1] = fbits(9.0f);

    // fsqrt.s f3, f1
    REncoding sq{0x2C, 0, 1, 0x0, 3, Opcode::F_OP};
    run(encode(sq));
    EXPECT_NEAR(fval(cpu.state.fregs[3]), 3.0f, 1e-6f);
}

TEST_F(Rv32FTest, FSGNJ_FSGNJN_FSGNJX)
{
    // f1 = -3.25, f2 = +2.0
    cpu.state.fregs[1] = fbits(-3.25f);
    cpu.state.fregs[2] = fbits(+2.0f);

    // fsgnj.s  f3, f1, f2
    REncoding sgnj{0x10, 2, 1, 0x0, 3, Opcode::F_OP};
    run(encode(sgnj));
    EXPECT_EQ(cpu.state.fregs[3], fbits(+3.25f));

    // fsgnjn.s f4, f1, f2
    REncoding sgnjn{0x10, 2, 1, 0x1, 4, Opcode::F_OP};
    run(encode(sgnjn));
    EXPECT_EQ(cpu.state.fregs[4], fbits(-3.25f));

    // fsgnjx.s f5, f1, f2
    REncoding sgnjx{0x10, 2, 1, 0x2, 5, Opcode::F_OP};
    run(encode(sgnjx));
    EXPECT_EQ(cpu.state.fregs[5], fbits(-3.25f));
}

TEST_F(Rv32FTest, FMIN_FMAX)
{
    // f1 = -1.0, f2 = +2.0
    cpu.state.fregs[1] = fbits(-1.0f);
    cpu.state.fregs[2] = fbits(+2.0f);

    // fmin.s f3,f1,f2   (funct7=0x14, rm=000)
    REncoding fmin{0x14, 2, 1, 0x0, 3, Opcode::F_OP};
    run(encode(fmin));
    EXPECT_EQ(cpu.state.fregs[3], fbits(-1.0f));

    // fmax.s f4,f1,f2   (funct7=0x14, rm=001)
    REncoding fmax{0x14, 2, 1, 0x1, 4, Opcode::F_OP};
    run(encode(fmax));
    EXPECT_EQ(cpu.state.fregs[4], fbits(+2.0f));
}

TEST_F(Rv32FTest, FEQ_FLT_FLE)
{
    // f1 = 1.5, f2 = 1.5, f3 = 2.0
    cpu.state.fregs[1] = fbits(1.5f);
    cpu.state.fregs[2] = fbits(1.5f);
    cpu.state.fregs[3] = fbits(2.0f);

    // feq.s x5, f1, f2
    REncoding feq{0x50, 2, 1, 0x2, /*rd(x)*/5, Opcode::F_OP};
    run(encode(feq));
    EXPECT_EQ(cpu.state.regs[5], 1u);

    // flt.s x6, f1, f3
    REncoding flt{0x50, 3, 1, 0x1, 6, Opcode::F_OP};
    run(encode(flt));
    EXPECT_EQ(cpu.state.regs[6], 1u);

    // fle.s x7, f3, f1
    REncoding fle{0x50, 1, 3, 0x0, 7, Opcode::F_OP};
    run(encode(fle));
    EXPECT_EQ(cpu.state.regs[7], 0u);
}

TEST_F(Rv32FTest, FCVT_W_and_WU_from_S)
{
    cpu.state.fregs[1] = fbits(42.0f);
    cpu.state.fregs[2] = fbits(123.0f);

    // fcvt.w.s x5, f1
    REncoding cw{0x60, 0, 1, 0x0, 5, Opcode::F_OP};
    run(encode(cw));
    EXPECT_EQ(cpu.state.regs[5], 42u);

    // fcvt.wu.s x6,f2
    REncoding cwu{0x60, 1, 2, 0x0, 6, Opcode::F_OP};
    run(encode(cwu));
    EXPECT_EQ(cpu.state.regs[6], 123u);
}

TEST_F(Rv32FTest, FCVT_S_from_W_and_WU)
{
    cpu.state.regs[1] = -7;     // x1
    cpu.state.regs[2] = 100;    // x2

    // fcvt.s.w  f5, x1
    REncoding sw{0x68, 0, /*rs1(x)*/1, 0x0, /*fd*/5, Opcode::F_OP};
    run(encode(sw));
    EXPECT_EQ(cpu.state.fregs[5], fbits(-7.0f));

    // fcvt.s.wu f6, x2
    REncoding swu{0x68, 1, 2, 0x0, 6, Opcode::F_OP};
    run(encode(swu));
    EXPECT_EQ(cpu.state.fregs[6], fbits(100.0f));
}

TEST_F(Rv32FTest, FMV_X_W_and_W_X_MoveRawBits)
{
    cpu.state.fregs[1] = fbits(-0.0f); // interesting sign-bit case

    // fmv.x.w x5, f1
    REncoding mxw{0x70, 0, 1, 0x0, 5, Opcode::F_OP};
    run(encode(mxw));
    EXPECT_EQ(cpu.state.regs[5], fbits(-0.0f));

    // fmv.w.x f6, x5
    REncoding mwx{0x78, 0, 5, 0x0, 6, Opcode::F_OP};
    run(encode(mwx));
    EXPECT_EQ(cpu.state.fregs[6], fbits(-0.0f));
}
