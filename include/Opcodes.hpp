#pragma once

#include "IntTypes.hpp"

namespace rv32i {

enum Opcode : u8
{
    R_TYPE    = 0x33,
    I_TYPE    = 0x13,
    LOAD      = 0x03,
    S_TYPE    = 0x23,
    B_TYPE    = 0x63,
    U_LUI     = 0x37,
    U_AUIPC   = 0x17,
    J_TYPE    = 0x6F,
    I_JALR    = 0x67,
    FENCE     = 0x0F,
    SYSTEM    = 0x73,
    F_LOAD    = 0x07, // FLW
    F_STORE   = 0x27, // FSW
    F_MADD    = 0x43, // FMADD.S
    F_MSUB    = 0x47, // FMSUB.S
    F_NMSUB   = 0x4B, // FNMSUB.S
    F_NMADD   = 0x4F, // FNMADD.S
    F_OP      = 0x53  // OP-FP: FADD.S, FSUB.S, ...
};

} // namespace rv32i
