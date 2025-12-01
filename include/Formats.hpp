#pragma once

//> Unified Format abstractions
//> Each Format implements a template `execute<Oper>(state, info)` that performs the
//> shared operand extraction and calls Oper's semantics

#include "IntTypes.hpp"
#include "InterpreterState.hpp"
#include "Status.hpp"
#include "InstrInfo.hpp"
#include "Operations.hpp"

namespace rv32i {

struct FormatR
{
    template<typename Oper>
    static ExecutionStatus execute(InterpreterState& s, InstrInfo const& info)
    {

        u32 a = s.regs[info.rs1];
        u32 b = s.regs[info.rs2];

        // std::cerr << Oper::name << '\n';

        u32 r = Oper::exec(a, b);

        if (info.rd != 0)
            s.regs[info.rd] = r;

        s.pc = info.pc + 4u;

        return ExecutionStatus::Success;
    }
};

struct FormatFR
{
    template<typename Oper>
    static ExecutionStatus execute(InterpreterState& s, InstrInfo const& info)
    {

        u32 a = s.fregs[info.rs1];
        u32 b = s.fregs[info.rs2];

        // std::cerr << Oper::name << '\n';

        u32 r = Oper::exec(a, b);

        s.fregs[info.rd] = r;

        s.pc = info.pc + 4u;

        return ExecutionStatus::Success;
    }
};

struct FormatFR4
{
    template<class Oper>
    static ExecutionStatus execute(InterpreterState& s, InstrInfo const& info)
    {
        u32 a = s.fregs[info.rs1];
        u32 b = s.fregs[info.rs2];
        u32 c = s.fregs[info.rs3];

        s.fregs[info.rd] = Oper::exec(a, b, c);

        s.pc = info.pc + 4;

        return ExecutionStatus::Success;
    }
};

struct FormatF2I
{
    template<class Oper>
    static ExecutionStatus execute(InterpreterState& s, InstrInfo const& info)
    {
        const u32 a = s.fregs[info.rs1];
        const u32 b = s.fregs[info.rs2];

        if (info.rd != 0)
            s.regs[info.rd] = Oper::exec(a, b);

        s.pc = info.pc + 4;

        return ExecutionStatus::Success;
    }
};

struct FormatI2F
{
    template<class Oper>
    static ExecutionStatus execute(InterpreterState& s, InstrInfo const& info)
    {
        const u32 a = s.regs[info.rs1];
        const u32 b = s.fregs[info.rs2];

        s.fregs[info.rd] = Oper::exec(a, b);

        s.pc = info.pc + 4;

        return ExecutionStatus::Success;
    }
};

struct FormatFCmp
{
    template<class Oper>
    static ExecutionStatus execute(InterpreterState& s, InstrInfo const& info)
    {
        const u32 a = s.fregs[info.rs1];
        const u32 b = s.fregs[info.rs2];

        if (info.rd != 0)
            s.regs[info.rd] = Oper::exec(a, b);

        s.pc = info.pc + 4;

        return ExecutionStatus::Success;
    }
};


struct FormatFlw
{
    template<class Oper>
    static ExecutionStatus execute(InterpreterState& s, InstrInfo const& info)
    {
        const u32 addr = s.regs[info.rs1] + info.imm; // FIXME: signed?

        if (info.rd != 0)
            s.fregs[info.rd] = s.memory.LoadU32(addr);

        s.pc = info.pc + 4;

        return ExecutionStatus::Success;
    }
};

// FSW: store f[rs2] to addr = x[rs1] + imm
struct FormatFsw
{
    template<class Oper>
    static ExecutionStatus execute(InterpreterState& s, InstrInfo const& info)
    {
        const u32 addr = s.regs[info.rs1] + (s32)info.imm;

        s.memory.StoreU32(addr, s.fregs[info.rs2]);

        s.pc = info.pc + 4;

        return ExecutionStatus::Success;
    }
};

struct FormatI
{
    template<typename Oper>
    static ExecutionStatus execute(InterpreterState& s, InstrInfo const& info)
    {
        u32 a = s.regs[info.rs1];
        s32 imm = static_cast<s32>(info.imm);

        u32 r = Oper::exec(a, imm);

        if (info.rd != 0)
            s.regs[info.rd] = r;

        s.pc = info.pc + 4u;

        return ExecutionStatus::Success;
    }
};

struct FormatLoad  //> load variants handled by Oper type selection
{
    template<typename Oper>
    static ExecutionStatus execute(InterpreterState& s, InstrInfo const& info)
    {
        u32 addr = s.regs[info.rs1] + static_cast<s32>(info.imm);

        if constexpr (std::is_same_v<Oper, LbOp>)
        {
            s.regs[info.rd] = static_cast<s32>(static_cast<int8_t>(s.memory.LoadU8(addr)));
        }
        else if constexpr (std::is_same_v<Oper, LbuOp>)
        {
            s.regs[info.rd] = s.memory.LoadU8(addr);
        }
        else if constexpr (std::is_same_v<Oper, LhOp>)
        {
            s.regs[info.rd] = static_cast<s32>(static_cast<s16>(s.memory.LoadU16(addr)));
        }
        else if constexpr (std::is_same_v<Oper, LhuOp>)
        {
            s.regs[info.rd] = s.memory.LoadU16(addr);
        }
        else
        {
            s.regs[info.rd] = s.memory.LoadU32(addr);
        }

        s.pc = info.pc + 4u;

        return ExecutionStatus::Success;
    }
};

struct FormatS
{
    template<typename Oper>
    static ExecutionStatus execute(InterpreterState& s, InstrInfo const& info)
    {
        u32 addr = s.regs[info.rs1] + static_cast<s32>(info.imm);

        u32 val = s.regs[info.rs2];

        if constexpr (std::is_same_v<Oper, SbOp>)
        {
            s.memory.StoreU8(addr, static_cast<u8>(val & 0xFF));
        }
        else if constexpr (std::is_same_v<Oper, ShOp>)
        {
            s.memory.StoreU16(addr, static_cast<uint16_t>(val & 0xFFFF));
        }
        else
        {
            s.memory.StoreU32(addr, val);
        }

        s.pc = info.pc + 4u;

        return ExecutionStatus::Success;
    }
};

struct FormatB
{
    template<typename Oper>
    static ExecutionStatus execute(InterpreterState& s, InstrInfo const& info)
    {
        u32 a = s.regs[info.rs1];
        u32 b = s.regs[info.rs2];

        if (Oper::cond(a,b))
        {
            s.pc = info.pc + static_cast<s32>(info.imm);
        }
        else
        {
            s.pc = info.pc + 4u;
        }

        return ExecutionStatus::Success;
    }
};

struct FormatU
{
    template<typename Oper>
    static ExecutionStatus execute(InterpreterState& s, InstrInfo const& info)
    {

        if constexpr (std::is_same_v<Oper, LuiOp>)
        {
            if (info.rd != 0) s.regs[info.rd] = info.imm;
        }
        else if constexpr (std::is_same_v<Oper, AuipcOp>)
        {
            if (info.rd != 0) s.regs[info.rd] = info.pc + info.imm;
        }

        s.pc = info.pc + 4u;

        return ExecutionStatus::Success;
    }
};

struct FormatJ
{
    template<typename Oper>
    static ExecutionStatus execute(InterpreterState& s, InstrInfo const& info)
    {
        u32 link = info.pc + 4u;

        if (info.rd != 0)
            s.regs[info.rd] = link;

        s.pc = info.pc + static_cast<s32>(info.imm);

        return ExecutionStatus::Success;
    }
};

struct FormatJalr
{
    template<typename Oper>
    static ExecutionStatus execute(InterpreterState& s, InstrInfo const& info)
    {
        u32 link = info.pc + 4u;
        u32 target = (s.regs[info.rs1] + static_cast<s32>(info.imm)) & ~u32(1);

        if (info.rd != 0)
            s.regs[info.rd] = link;

        s.pc = target;

        return ExecutionStatus::Success;
    }
};


} // namespace rv32i
