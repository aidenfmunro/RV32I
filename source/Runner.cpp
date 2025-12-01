#include "Runner.hpp"
#include "Decoder.hpp"
namespace rv32i {

ExecutionResult run_program(Interpreter& cpu, int cycle_limit)
{
    ExecutionResult res{ExecutionStatus::Success, 0, 0};

    for (int cycles = 0; cycles < cycle_limit; ++cycles)
    {
        u32 instr = cpu.load<u32>(cpu.pc());

        auto [info, key] = Decoder::decode(instr, cpu.pc());

        res.pc = info.pc;

        ExecutionStatus st = cpu.dispatch(cpu.state, info, key);

        res.cycles = cycles + 1;

        if (st == ExecutionStatus::ProgramExit)
        {
            res.status    = st;
            res.exit_code = static_cast<int>(cpu.state.regs[10]); // a0
            return res;
        }

        if (st != ExecutionStatus::Success)
        {
            res.status = st;
            return res;
        }
    }

    res.status = ExecutionStatus::TrapIllegal; // timeout / exceeded cycles
    return res;
}

} // namespace rv32i
