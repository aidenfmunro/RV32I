#pragma once

#include "IntTypes.hpp"

namespace rv32i {

constexpr u32 ZBB_KEY_CLZ   = 0x80000100u;
constexpr u32 ZBB_KEY_CTZ   = 0x80000101u;
constexpr u32 ZBB_KEY_CPOP  = 0x80000102u;
constexpr u32 ZBB_KEY_SEXTB = 0x80000103u;
constexpr u32 ZBB_KEY_SEXTH = 0x80000104u;
constexpr u32 ZBB_KEY_RORI  = 0x80000105u;
constexpr u32 ZBB_KEY_ORCB  = 0x80000106u;
constexpr u32 ZBB_KEY_REV8  = 0x80000107u;

} // namespace rv32i
