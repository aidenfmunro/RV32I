#include "Handlers.hpp"
#include "Opcodes.hpp"
#include "Formats.hpp"
#include "Operations.hpp"
#include "Syscall.hpp"
#include "HandlerFactory.hpp"

#define REG(KEYEXPR, FORMAT, INSTR) \
  cpu.register_handler((KEYEXPR), make_handler<FORMAT, INSTR>())

#define RM8(OPC, F7, FORMAT, INSTR)                                          \
  cpu.register_handler(key(OPC, 0x0, F7), make_handler<FORMAT, INSTR>());    \
  cpu.register_handler(key(OPC, 0x1, F7), make_handler<FORMAT, INSTR>());    \
  cpu.register_handler(key(OPC, 0x2, F7), make_handler<FORMAT, INSTR>());    \
  cpu.register_handler(key(OPC, 0x3, F7), make_handler<FORMAT, INSTR>());    \
  cpu.register_handler(key(OPC, 0x4, F7), make_handler<FORMAT, INSTR>());    \
  cpu.register_handler(key(OPC, 0x5, F7), make_handler<FORMAT, INSTR>());    \
  cpu.register_handler(key(OPC, 0x6, F7), make_handler<FORMAT, INSTR>());    \
  cpu.register_handler(key(OPC, 0x7, F7), make_handler<FORMAT, INSTR>())

namespace rv32i {

void register_all_handlers(Interpreter& cpu)
{
    auto key = [](u8 opcode, u8 funct3, u8 funct7)
        { return u32(opcode) | (u32(funct3) << 8) | (u32(funct7) << 16); };

    // ---- RV32I ----
    REG(key(Opcode::R_TYPE, 0x0, 0x00), FormatR, AddOp);
    REG(key(Opcode::R_TYPE, 0x0, 0x20), FormatR, SubOp);
    REG(key(Opcode::R_TYPE, 0x1, 0x00), FormatR, SllOp);
    REG(key(Opcode::R_TYPE, 0x2, 0x00), FormatR, SltOp);
    REG(key(Opcode::R_TYPE, 0x3, 0x00), FormatR, SltuOp);
    REG(key(Opcode::R_TYPE, 0x4, 0x00), FormatR, XorOp);
    REG(key(Opcode::R_TYPE, 0x5, 0x00), FormatR, SrlOp);
    REG(key(Opcode::R_TYPE, 0x5, 0x20), FormatR, SraOp);
    REG(key(Opcode::R_TYPE, 0x6, 0x00), FormatR, OrOp);
    REG(key(Opcode::R_TYPE, 0x7, 0x00), FormatR, AndOp);

    REG(key(Opcode::I_TYPE, 0x0, 0x00), FormatI, AddiOp);
    REG(key(Opcode::I_TYPE, 0x2, 0x00), FormatI, SltiOp);
    REG(key(Opcode::I_TYPE, 0x3, 0x00), FormatI, SltiuOp);
    REG(key(Opcode::I_TYPE, 0x4, 0x00), FormatI, XoriOp);
    REG(key(Opcode::I_TYPE, 0x6, 0x00), FormatI, OriOp);
    REG(key(Opcode::I_TYPE, 0x7, 0x00), FormatI, AndiOp);
    REG(key(Opcode::I_TYPE, 0x1, 0x00), FormatI, SlliOp);
    REG(key(Opcode::I_TYPE, 0x5, 0x00), FormatI, SrliOp);
    REG(key(Opcode::I_TYPE, 0x5, 0x20), FormatI, SraiOp);

    REG(key(Opcode::LOAD, 0x0, 0x00), FormatLoad, LbOp);
    REG(key(Opcode::LOAD, 0x1, 0x00), FormatLoad, LhOp);
    REG(key(Opcode::LOAD, 0x2, 0x00), FormatLoad, LwOp);
    REG(key(Opcode::LOAD, 0x4, 0x00), FormatLoad, LbuOp);
    REG(key(Opcode::LOAD, 0x5, 0x00), FormatLoad, LhuOp);

    REG(key(Opcode::S_TYPE, 0x0, 0x00), FormatS, SbOp);
    REG(key(Opcode::S_TYPE, 0x1, 0x00), FormatS, ShOp);
    REG(key(Opcode::S_TYPE, 0x2, 0x00), FormatS, SwOp);

    REG(key(Opcode::B_TYPE, 0x0, 0x00), FormatB, BeqOp);
    REG(key(Opcode::B_TYPE, 0x1, 0x00), FormatB, BneOp);
    REG(key(Opcode::B_TYPE, 0x4, 0x00), FormatB, BltOp);
    REG(key(Opcode::B_TYPE, 0x5, 0x00), FormatB, BgeOp);
    REG(key(Opcode::B_TYPE, 0x6, 0x00), FormatB, BltuOp);
    REG(key(Opcode::B_TYPE, 0x7, 0x00), FormatB, BgeuOp);

    REG(key(Opcode::U_LUI,   0x0, 0x00), FormatU, LuiOp);
    REG(key(Opcode::U_AUIPC, 0x0, 0x00), FormatU, AuipcOp);

    REG(key(Opcode::J_TYPE,  0x0, 0x00), FormatJ,    JalOp);
    REG(key(Opcode::I_JALR,  0x0, 0x00), FormatJalr, JalrOp);

    // ---- RV32M ----
    REG(key(Opcode::R_TYPE, 0x0, 0x01), FormatR, MulOp);
    REG(key(Opcode::R_TYPE, 0x1, 0x01), FormatR, MulhOp);
    REG(key(Opcode::R_TYPE, 0x2, 0x01), FormatR, MulhsuOp);
    REG(key(Opcode::R_TYPE, 0x3, 0x01), FormatR, MulhuOp);
    REG(key(Opcode::R_TYPE, 0x4, 0x01), FormatR, DivOp);
    REG(key(Opcode::R_TYPE, 0x5, 0x01), FormatR, DivuOp);
    REG(key(Opcode::R_TYPE, 0x6, 0x01), FormatR, RemOp);
    REG(key(Opcode::R_TYPE, 0x7, 0x01), FormatR, RemuOp);

    // ---- RV32F ----

    // FLW / FSW (funct3=010)
    REG(key(Opcode::F_LOAD,  0x2, 0x00), FormatFlw, void);
    REG(key(Opcode::F_STORE, 0x2, 0x00), FormatFsw, void);

    RM8(Opcode::F_OP, 0x00, FormatFR, FaddSOp);
    RM8(Opcode::F_OP, 0x04, FormatFR, FsubSOp);
    RM8(Opcode::F_OP, 0x08, FormatFR, FmulSOp);
    RM8(Opcode::F_OP, 0x0C, FormatFR, FdivSOp);

    RM8(Opcode::F_OP, 0x2C, FormatFR, FsqrtSOp);

    REG(key(Opcode::F_OP, 0x0, 0x10), FormatFR, FsgnjSOp);
    REG(key(Opcode::F_OP, 0x1, 0x10), FormatFR, FsgnjnSOp);
    REG(key(Opcode::F_OP, 0x2, 0x10), FormatFR, FsgnjxSOp);

    REG(key(Opcode::F_OP, 0x0, 0x14), FormatFR, FminSOp);
    REG(key(Opcode::F_OP, 0x1, 0x14), FormatFR, FmaxSOp);

    REG(key(Opcode::F_OP, 0x2, 0x50), FormatFCmp, FeqSOp);
    REG(key(Opcode::F_OP, 0x1, 0x50), FormatFCmp, FltSOp);
    REG(key(Opcode::F_OP, 0x0, 0x50), FormatFCmp, FleSOp);

    RM8(Opcode::F_OP, 0x60, FormatF2I, FcvtWSOp);
    RM8(Opcode::F_OP, 0x61, FormatF2I, FcvtWUSOp);

    RM8(Opcode::F_OP, 0x68, FormatI2F, FcvtSWOp);
    RM8(Opcode::F_OP, 0x69, FormatI2F, FcvtSWUOp);

    REG(key(Opcode::F_OP, 0x0, 0x70), FormatF2I, FmvXWOp);
    // REG(key(Opcode::F_OP, 0x1, 0x70), FormatF2I, FclassSOp);

    REG(key(Opcode::F_OP, 0x0, 0x78), FormatI2F, FmvWXOp);

    REG(key(Opcode::F_MADD,  0x0, 0x0), FormatFR4, FmaddSOp);
    REG(key(Opcode::F_MSUB,  0x0, 0x0), FormatFR4, FmsubSOp);
    REG(key(Opcode::F_NMSUB, 0x0, 0x0), FormatFR4, FnmsubSOp);
    REG(key(Opcode::F_NMADD, 0x0, 0x0), FormatFR4, FnmaddSOp);

    // ---- Syscall ----
    cpu.register_handler(
        key(Opcode::SYSTEM, 0x0, 0x00),
        [](InterpreterState& s, InstrInfo const&) -> ExecutionStatus {
            return handle_syscall(s);
        }
    );
}

} // namespace rv32i
