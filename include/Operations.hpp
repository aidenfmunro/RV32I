#pragma once

#include <cstdint>
#include <cmath>
#include <iostream>

#include "BitHelpers.hpp"
#include "IntTypes.hpp"

namespace rv32i {

struct AddOp
{
    static constexpr const char* name = "add";
    static u32 exec(u32 a, u32 b){ return a + b; }
};

struct SubOp
{
    static constexpr const char* name = "sub";
    static u32 exec(u32 a, u32 b){ return a - b; }
};

struct AndOp
{
    static constexpr const char* name = "and";
    static u32 exec(u32 a, u32 b){ return a & b; }
};

struct OrOp
{
    static constexpr const char* name = "or";
    static u32 exec(u32 a, u32 b){ return a | b; }
};

struct XorOp
{
    static constexpr const char* name = "xor";
    static u32 exec(u32 a, u32 b){ return a ^ b; }
};

struct SllOp
{
    static constexpr const char* name = "sll";
    static u32 exec(u32 a, u32 b){ return a << (b & 31); }
};

struct SrlOp
{
    static constexpr const char* name = "srl";
    static u32 exec(u32 a, u32 b){ return a >> (b & 31); }
};

struct SraOp
{
    static constexpr const char* name = "sra";
    static u32 exec(u32 a, u32 b)
    {
        return static_cast<u32>(static_cast<s32>(a) >> (b & 31));
    }
};

struct SltOp
{
    static constexpr const char* name = "slt";
    static u32 exec(u32 a, u32 b)
    {
        return (static_cast<s32>(a)<static_cast<s32>(b)) ? 1u : 0u;
    }
};

struct SltuOp
{
    static constexpr const char* name = "sltu";
    static u32 exec(u32 a, u32 b)
    {
        return (a < b) ? 1u : 0u;
    }
};

struct AddiOp
{
    static constexpr const char* name = "addi";
    static u32 exec(u32 a, s32 imm)
    {
        return a + imm;
    }
};

struct SltiOp
{
    static constexpr const char* name = "slti";
    static u32 exec(u32 a,s32 imm)
    {
        return (static_cast<s32>(a) < imm) ? 1u : 0u;
    }
};

struct SltiuOp
{
    static constexpr const char* name = "sltiu";
    static u32 exec(u32 a, s32 imm)
    {
        return (a < static_cast<u32>(imm)) ? 1u : 0u;
    }
};

struct AndiOp
{
    static constexpr const char* name = "andi";
    static u32 exec(u32 a, s32 imm)
    {
        return a & static_cast<u32>(imm);
    }
};

struct OriOp
{
    static constexpr const char* name = "ori";
    static u32 exec(u32 a, s32 imm)
    {
        return a | static_cast<u32>(imm);
    }
};

struct XoriOp
{
    static constexpr const char* name = "xori";
    static u32 exec(u32 a, s32 imm)
    {
        return a ^ static_cast<u32>(imm);
    }
};

struct SlliOp
{
    static constexpr const char* name = "slli";
    static u32 exec(u32 a, s32 sh)
    {
        return a << (sh & 31);
    }
};

struct SrliOp
{
    static constexpr const char* name = "srli";
    static u32 exec(u32 a, s32 sh)
    {
        return a >> (sh & 31);
    }
};

struct SraiOp
{
    static constexpr const char* name = "srai";
    static u32 exec(u32 a, s32 sh)
    {
        return static_cast<u32>(static_cast<s32>(a) >> (sh & 31));
    }
};

struct LuiOp
{
    static constexpr const char* name = "lui";
};

struct AuipcOp  { static constexpr const char* name = "auipc"; };

struct LbOp     { static constexpr const char* name = "lb"; };
struct LbuOp    { static constexpr const char* name = "lbu"; };
struct LhOp     { static constexpr const char* name = "lh"; };
struct LhuOp    { static constexpr const char* name = "lhu"; };
struct LwOp     { static constexpr const char* name = "lw"; };

struct SbOp     { static constexpr const char* name = "sb"; };
struct ShOp     { static constexpr const char* name = "sh"; };
struct SwOp     { static constexpr const char* name = "sw"; };

struct BeqOp
{
    static constexpr const char* name = "beq";
    static bool cond(u32 a, u32 b)
    {
        return a == b;
    }
};

struct BneOp
{
    static constexpr const char* name = "bne";
    static bool cond(u32 a, u32 b)
    {
        return a != b;
    }
};

struct BltOp
{
    static constexpr const char* name = "blt";
    static bool cond(u32 a, u32 b)
    {
        return static_cast<s32>(a) < static_cast<s32>(b);
    }
};

struct BgeOp
{
    static constexpr const char* name = "bge";
    static bool cond(u32 a, u32 b)
    {
        return static_cast<s32>(a) >= static_cast<s32>(b);
    }
};

struct BltuOp
{
    static constexpr const char* name = "bltu";
    static bool cond(u32 a, u32 b)
    {
        return a < b;
    }
};

struct BgeuOp
{
    static constexpr const char* name = "bgeu";
    static bool cond(u32 a, u32 b)
    {
        return a >= b;
    } };

struct JalOp  { static constexpr const char* name = "jal"; };
struct JalrOp { static constexpr const char* name = "jalr"; };

// ---- M ----

struct MulOp
{
    static constexpr const char* name = "mul";

    static u32 exec(u32 a, u32 b)
    {
        return static_cast<s32>(a) * static_cast<s32>(b);
    }

    // S x S high 32 bits
};

struct MulhOp
{
    static constexpr const char* name = "mulh";

    static u32 exec(u32 a, u32 b)
    {
        s64 result = static_cast<s64>(static_cast<s32>(a))
                     *
                     static_cast<s64>(static_cast<s32>(b));

        return static_cast<u32>(result >> 32);
    }

    // S x S low 32 bits
};

struct MulhsuOp
{
    static constexpr const char* name = "mulhsu";

    static u32 exec(u32 a, u32 b)
    {
        s64 result = static_cast<s64>(static_cast<s32>(a))
                     *
                     static_cast<u64>(b);

        return static_cast<u32>(result >> 32);
    }

    // S x U high 32 bits
};

struct MulhuOp
{
    static constexpr const char* name = "mulhu";

    static u32 exec(u32 a, u32 b)
    {
        s64 result = static_cast<u64>(a)
                     *
                     static_cast<u64>(b);

        return static_cast<u32>(result >> 32);
    }

    // U x U high 32 bits
};

struct DivOp
{
    static constexpr const char* name = "div";

    static u32 exec(u32 a, u32 b)
    {
        s32 sa = static_cast<s32>(a);

        s32 sb = static_cast<s32>(b);

        if (sb == 0)
            return static_cast<u32>(-1);
        else
            return sa / sb;
    }
};

struct DivuOp
{
    static constexpr const char* name = "divu";

    static u32 exec(u32 a, u32 b)
    {
        if (b == 0)
            return 0xFFFFFFFFu;
        else
            return a / b;
    }
};

struct RemOp
{
    static constexpr const char* name = "rem";

    static u32 exec(u32 a, u32 b)
    {
        s32 sa = static_cast<s32>(a);
        s32 sb = static_cast<s32>(b);

        if (sb == 0)
            return a;

        if (sa == INT32_MIN && sb == -1)
            return 0;

        return sa % sb;
    }
};

struct RemuOp
{
    static constexpr const char* name = "remu";

    static u32 exec(u32 a, u32 b)
    {
        if (b == 0)
            return a;

        return a % b;
    }
};

// ---- F ----

struct FlwOp
{
    static constexpr const char* name = "flw";
};

struct FswOp
{
    static constexpr const char* name = "fsw";
};

// Binary FP -> FP
struct FaddSOp
{
    static constexpr const char* name="fadd.s";

    static u32 exec(u32 a, u32 b)
    {
        f32 fa = f32_from_bits(a);
        f32 fb = f32_from_bits(b);

        return bits_from_f32(fa + fb);
    }
};

struct FsubSOp
{
    static constexpr const char* name="fsub.s";

    static u32 exec(u32 a, u32 b)
    {
        f32 fa = f32_from_bits(a);
        f32 fb = f32_from_bits(b);

        return bits_from_f32(fa - fb);
    }
};

struct FmulSOp
{
    static constexpr const char* name = "fmul.s";

    static u32 exec(u32 a, u32 b)
    {

        f32 fa = f32_from_bits(a);
        f32 fb = f32_from_bits(b);

        return bits_from_f32(fa * fb);
    }
};

struct FdivSOp
{
    static constexpr const char* name = "fdiv.s";

    static u32 exec(u32 a, u32 b)
    {

        f32 fa = f32_from_bits(a);
        f32 fb = f32_from_bits(b);

        return bits_from_f32(fa / fb); // FIXME: What if fb is zero?
    }
};

// Unary FP -> FP
struct FsqrtSOp
{
    static constexpr const char* name = "fsqrt.s";

    static u32 exec(u32 a, [[maybe_unused]] u32 b)
    {
        f32 fa = f32_from_bits(a);

        return bits_from_f32(std::sqrt(fa));
    }
};

// Sign injection family (bitwise)

struct FsgnjSOp
{
    static constexpr const char* name = "fsgnj.s";

    static u32 exec(u32 a, u32 b)
    {
        return (a & 0x7FFF'FFFFu) | (b & 0x8000'0000u);
    }
};

struct FsgnjnSOp
{
    static constexpr const char* name = "fsgnjn.s";

    static u32 exec(u32 a, u32 b)
    {
        return (a & 0x7FFF'FFFFu) | ((~b) & 0x8000'0000u);
    }
};

struct FsgnjxSOp
{
    static constexpr const char* name = "fsgnjx.s";

    static u32 exec(u32 a, u32 b)
    {
        return (a & 0x7FFF'FFFFu) | (((a^b) & 0x8000'0000u));
    }
};

// Min/Max (naive: ignore NaN rules)
struct FminSOp
{
    static constexpr const char* name = "fmin.s";
    static u32 exec(u32 a, u32 b)
    {
        float x = f32_from_bits(a);
        float y = f32_from_bits(b);

        return bits_from_f32((x < y) ? x : y);
    }
};

struct FmaxSOp
{
    static constexpr const char* name = "fmax.s";

    static u32 exec(u32 a, u32 b)
    {
        float x = f32_from_bits(a);
        float y = f32_from_bits(b);

        return bits_from_f32((x > y) ? x : y);
    }
};

// Comparisons -> integer 0/1 (rd in GPRs)
struct FeqSOp
{
    static constexpr const char* name = "feq.s";

    static u32 exec(u32 a, u32 b)
    {
        return f32_from_bits(a)==f32_from_bits(b); // FIXME: Is this correct?
    }
};

struct FltSOp
{
    static constexpr const char* name="flt.s";

    static u32 exec(u32 a, u32 b)
    {
        return f32_from_bits(a) < f32_from_bits(b);
    }
};

struct FleSOp
{
    static constexpr const char* name = "fle.s";
    static u32 exec(u32 a, u32 b)
    {
        return f32_from_bits(a)<=f32_from_bits(b);
    }
};

// Convert FP->int

struct FcvtWSOp
{
    static constexpr const char* name="fcvt.w.s";
    static u32 exec(u32 a, [[maybe_unused]] u32 b)
    {
        f32 fa = f32_from_bits(a);

        return static_cast<u32>(static_cast<s32>(std::trunc(fa)));
    }
};

struct FcvtWUSOp
{
    static constexpr const char* name="fcvt.wu.s";
    static u32 exec(u32 a, [[maybe_unused]] u32 b)
    {
        f32 fa = f32_from_bits(a);

        u32 num = static_cast<u32>(std::trunc(fa));

        std::cerr << num ;
        return static_cast<u32>(std::trunc(fa));
    }
};

// Convert int->FP
struct FcvtSWOp
{
    static constexpr const char* name="fcvt.s.w";
    static u32 exec(u32 a, [[maybe_unused]] u32 b)
    {
        return bits_from_f32(static_cast<f32>(static_cast<s32>(a)));
    }
};

struct FcvtSWUOp
{
    static constexpr const char* name="fcvt.s.wu";

    static u32 exec(u32 a, [[maybe_unused]] u32 b)
    {
        return bits_from_f32(static_cast<f32>(a));
    }
};

// Moves & classify
struct FmvXWOp
{
    static constexpr const char* name = "fmv.x.w";
    static u32 exec(u32 a, [[maybe_unused]] u32 b)
    {
        return a;
    }
};

struct FmvWXOp
{
    static constexpr const char* name = "fmv.w.x";
    static u32 exec(u32 a, [[maybe_unused]] u32 b)
    {
        return a;
    }
};

// Simple classifier (bit-level: sign/exp/fract)
struct FclassSOp{ static constexpr const char* name="fclass.s";
  static u32 exec(u32 a){
    const u32 s=(a>>31)&1, e=(a>>23)&0xFF, f=a&0x7FFFFF;
    // Very rough: set one bit per class (not full spec)
    if(e==0 && f==0) return s? 0x0000'0001u /* -0 */ : 0x0000'0002u /* +0 */;
    if(e==0)         return s? 0x0000'0004u /* -sub */: 0x0000'0008u /* +sub */;
    if(e==0xFF && f) return 0x0000'0200u;    /* NaN */
    if(e==0xFF)      return s? 0x0000'0010u /* -inf */: 0x0000'0020u /* +inf */;
    return s? 0x0000'0040u /* -norm */: 0x0000'0080u /* +norm */;
  }
};

// R4 fused (not actually fused; rm ignored)
struct FmaddSOp
{
    static constexpr const char* name="fmadd.s";
    static u32 exec(u32 a, u32 b, u32 c)
    {
        f32 fa = f32_from_bits(a);
        f32 fb = f32_from_bits(b);
        f32 fc = f32_from_bits(c);

        return bits_from_f32(fa * fb + fc);
    }
};

struct FmsubSOp
{
    static constexpr const char* name="fmsub.s";
    static u32 exec(u32 a, u32 b, u32 c)
    {
        f32 fa = f32_from_bits(a);
        f32 fb = f32_from_bits(b);
        f32 fc = f32_from_bits(c);

        return bits_from_f32(fa * fb - fc);
    }
};

struct FnmaddSOp
{
    static constexpr const char* name="fnmadd.s";

    static u32 exec(u32 a, u32 b, u32 c)
    {
        f32 fa = f32_from_bits(a);
        f32 fb = f32_from_bits(b);
        f32 fc = f32_from_bits(c);

        return bits_from_f32(-fa * fb + fc);
    }
};

struct FnmsubSOp
{
    static constexpr const char* name = "fnmsub.s";

    static u32 exec(u32 a,u32 b,u32 c)
    {
        f32 fa = f32_from_bits(a);
        f32 fb = f32_from_bits(b);
        f32 fc = f32_from_bits(c);

        return bits_from_f32(-fa * fb - fc);
    }
};

// ---- Zbb  ----

struct AndnOp
{
    static constexpr const char* name = "andn";
    static u32 exec(u32 a, u32 b)
    {
        return a & ~b;
    }
};

struct OrnOp
{
    static constexpr const char* name = "orn";
    static u32 exec(u32 a, u32 b)
    {
        return a | ~b;
    }
};

struct XnorOp
{
    static constexpr const char* name = "xnor";
    static u32 exec(u32 a, u32 b)
    {
        return ~(a ^ b);
    }
};

struct MinOp
{
    static constexpr const char* name = "min";
    static u32 exec(u32 a, u32 b)
    {
        s32 sa = static_cast<s32>(a);
        s32 sb = static_cast<s32>(b);
        return static_cast<u32>(sa < sb ? sa : sb);
    }
};

struct MaxOp
{
    static constexpr const char* name = "max";
    static u32 exec(u32 a, u32 b)
    {
        s32 sa = static_cast<s32>(a);
        s32 sb = static_cast<s32>(b);
        return static_cast<u32>(sa > sb ? sa : sb);
    }
};

struct MinuOp
{
    static constexpr const char* name = "minu";
    static u32 exec(u32 a, u32 b)
    {
        return (a < b) ? a : b;
    }
};

struct MaxuOp
{
    static constexpr const char* name = "maxu";
    static u32 exec(u32 a, u32 b)
    {
        return (a > b) ? a : b;
    }
};

struct ClzOp
{
    static constexpr const char* name = "clz";
    static u32 exec(u32 a, [[maybe_unused]] s32 imm)
    {
        if (a == 0)
            return 32u;
        return static_cast<u32>(std::countl_zero(a));
    }
};

struct CtzOp
{
    static constexpr const char* name = "ctz";
    static u32 exec(u32 a, [[maybe_unused]] u32 imm)
    {
        if (a == 0)
            return 32u;
        return static_cast<u32>(std::countr_zero(a));
    }
};

struct CpopOp
{
    static constexpr const char* name = "cpop";
    static u32 exec(u32 a, [[maybe_unused]] u32 imm)
    {
        return static_cast<u32>(std::popcount(a));
    }
};

struct SextBOp
{
    static constexpr const char* name = "sext.b";
    static u32 exec(u32 a, [[maybe_unused]] u32 imm)
    {
        return static_cast<u32>(static_cast<s8>(a));
    }
};

struct SextHOp
{
    static constexpr const char* name = "sext.h";
    static u32 exec(u32 a, [[maybe_unused]] u32 imm)
    {
        return static_cast<u32>(static_cast<s16>(a));
    }
};

struct ZextHOp
{
    static constexpr const char* name = "zext.h";
    static u32 exec(u32 a, [[maybe_unused]] u32 b)
    {
        return a & 0xFFFFu;
    }
};

struct RolOp
{
    static constexpr const char* name = "rol";
    static u32 exec(u32 a, u32 b)
    {
        u32 sh = b & 31u;
        if (sh == 0) return a;
        return (a << sh) | (a >> ((32u - sh) & 31u));
    }
};

struct RorOp
{
    static constexpr const char* name = "ror";
    static u32 exec(u32 a, u32 b)
    {
        u32 sh = b & 31u;
        if (sh == 0) return a;
        return (a >> sh) | (a << ((32u - sh) & 31u));
    }
};

struct RoriOp
{
    static constexpr const char* name = "rori";
    static u32 exec(u32 a, s32 sh)
    {
        u32 s = static_cast<u32>(sh) & 31u;
        if (s == 0) return a;
        return (a >> s) | (a << ((32u - s) & 31u));
    }
};

struct OrcbOp
{
    static constexpr const char* name = "orc.b";
    static u32 exec(u32 a, [[maybe_unused]] u32 b)
    {
        u32 res = 0;
        for (int byte = 0; byte < 4; ++byte)
        {
            u32 chunk = (a >> (byte * 8)) & 0xFFu;
            u32 out_byte = (chunk == 0) ? 0x00u : 0xFFu;
            res |= (out_byte << (byte * 8));
        }
        return res;
    }
};

struct Rev8Op
{
    static constexpr const char* name = "rev8";
    static u32 exec(u32 a, [[maybe_unused]] u32 b)
    {
        return ((a & 0x000000FFu) << 24)
             | ((a & 0x0000FF00u) << 8)
             | ((a & 0x00FF0000u) >> 8)
             | ((a & 0xFF000000u) >> 24);
    }
};



} // namespace rv32i
