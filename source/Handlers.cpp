#include "Handlers.hpp"
#include "Opcodes.hpp"
#include "Formats.hpp"
#include "Operations.hpp"
#include "Syscall.hpp"
#include "IntTypes.hpp"
#include "HandlerFactory.hpp"

namespace rv32i {

void register_all_handlers(Interpreter& cpu)
{
    auto key = [](u8 opcode, u8 funct3, u8 funct7)
        { return u32(opcode) | (u32(funct3) << 8) | (u32(funct7) << 16); };

    cpu.register_handler(key(Opcode::R_TYPE, 0x0, 0x00), make_handler<FormatR, AddOp>());  
    cpu.register_handler(key(Opcode::R_TYPE, 0x0, 0x20), make_handler<FormatR, SubOp>());  
    cpu.register_handler(key(Opcode::R_TYPE, 0x1, 0x00), make_handler<FormatR, SllOp>());  
    cpu.register_handler(key(Opcode::R_TYPE, 0x2, 0x00), make_handler<FormatR, SltOp>()); 
    cpu.register_handler(key(Opcode::R_TYPE, 0x3, 0x00), make_handler<FormatR, SltuOp>()); 
    cpu.register_handler(key(Opcode::R_TYPE, 0x4, 0x00), make_handler<FormatR, XorOp>());  
    cpu.register_handler(key(Opcode::R_TYPE, 0x5, 0x00), make_handler<FormatR, SrlOp>());  
    cpu.register_handler(key(Opcode::R_TYPE, 0x5, 0x20), make_handler<FormatR, SraOp>());  
    cpu.register_handler(key(Opcode::R_TYPE, 0x6, 0x00), make_handler<FormatR, OrOp>());   
    cpu.register_handler(key(Opcode::R_TYPE, 0x7, 0x00), make_handler<FormatR, AndOp>());  

    cpu.register_handler(key(Opcode::I_TYPE, 0x0, 0x00), make_handler<FormatI, AddiOp>()); 
    cpu.register_handler(key(Opcode::I_TYPE, 0x2, 0x00), make_handler<FormatI, SltiOp>()); 
    cpu.register_handler(key(Opcode::I_TYPE, 0x3, 0x00), make_handler<FormatI, SltiuOp>());
    cpu.register_handler(key(Opcode::I_TYPE, 0x4, 0x00), make_handler<FormatI, XoriOp>()); 
    cpu.register_handler(key(Opcode::I_TYPE, 0x6, 0x00), make_handler<FormatI, OriOp>());  
    cpu.register_handler(key(Opcode::I_TYPE, 0x7, 0x00), make_handler<FormatI, AndiOp>()); 
    cpu.register_handler(key(Opcode::I_TYPE, 0x1, 0x00), make_handler<FormatI, SlliOp>()); 
    cpu.register_handler(key(Opcode::I_TYPE, 0x5, 0x00), make_handler<FormatI, SrliOp>()); 
    cpu.register_handler(key(Opcode::I_TYPE, 0x5, 0x20), make_handler<FormatI, SraiOp>()); 

    cpu.register_handler(key(Opcode::LOAD, 0x0, 0x00), make_handler<FormatLoad, LbOp>());  
    cpu.register_handler(key(Opcode::LOAD, 0x1, 0x00), make_handler<FormatLoad, LhOp>());  
    cpu.register_handler(key(Opcode::LOAD, 0x2, 0x00), make_handler<FormatLoad, LwOp>());  
    cpu.register_handler(key(Opcode::LOAD, 0x4, 0x00), make_handler<FormatLoad, LbuOp>()); 
    cpu.register_handler(key(Opcode::LOAD, 0x5, 0x00), make_handler<FormatLoad, LhuOp>()); 

    cpu.register_handler(key(Opcode::S_TYPE, 0x0, 0x00), make_handler<FormatS, SbOp>()); 
    cpu.register_handler(key(Opcode::S_TYPE, 0x1, 0x00), make_handler<FormatS, ShOp>()); 
    cpu.register_handler(key(Opcode::S_TYPE, 0x2, 0x00), make_handler<FormatS, SwOp>()); 

    cpu.register_handler(key(Opcode::B_TYPE, 0x0, 0x00), make_handler<FormatB, BeqOp>());  
    cpu.register_handler(key(Opcode::B_TYPE, 0x1, 0x00), make_handler<FormatB, BneOp>());  
    cpu.register_handler(key(Opcode::B_TYPE, 0x4, 0x00), make_handler<FormatB, BltOp>());  
    cpu.register_handler(key(Opcode::B_TYPE, 0x5, 0x00), make_handler<FormatB, BgeOp>());  
    cpu.register_handler(key(Opcode::B_TYPE, 0x6, 0x00), make_handler<FormatB, BltuOp>()); 
    cpu.register_handler(key(Opcode::B_TYPE, 0x7, 0x00), make_handler<FormatB, BgeuOp>()); 

    cpu.register_handler(key(Opcode::U_LUI, 0x0, 0x00), make_handler<FormatU, LuiOp>());   
    cpu.register_handler(key(Opcode::U_AUIPC, 0x0, 0x00), make_handler<FormatU, AuipcOp>());

    cpu.register_handler(key(Opcode::J_TYPE, 0x0, 0x00), make_handler<FormatJ, JalOp>());  
    cpu.register_handler(key(Opcode::I_JALR, 0x0, 0x00), make_handler<FormatJalr, JalrOp>()); 

    // M

    cpu.register_handler(key(Opcode::R_TYPE, 0x0, 0x01), make_handler<FormatR, MulOp>());  
    cpu.register_handler(key(Opcode::R_TYPE, 0x1, 0x01), make_handler<FormatR, MulhOp>());  
    cpu.register_handler(key(Opcode::R_TYPE, 0x2, 0x01), make_handler<FormatR, MulhsuOp>());  
    cpu.register_handler(key(Opcode::R_TYPE, 0x3, 0x01), make_handler<FormatR, MulhuOp>());  
    cpu.register_handler(key(Opcode::R_TYPE, 0x4, 0x01), make_handler<FormatR, DivOp>());  
    cpu.register_handler(key(Opcode::R_TYPE, 0x5, 0x01), make_handler<FormatR, DivuOp>());  
    cpu.register_handler(key(Opcode::R_TYPE, 0x6, 0x01), make_handler<FormatR, RemOp>());  
    cpu.register_handler(key(Opcode::R_TYPE, 0x7, 0x01), make_handler<FormatR, RemuOp>());  

    cpu.register_handler(key(Opcode::SYSTEM, 0x0, 0x00),
         [](InterpreterState& s, InstrInfo const&) -> ExecutionStatus { return handle_syscall(s); }
    ); 
}

} // namespace rv32i
