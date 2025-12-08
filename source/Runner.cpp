#include "Runner.hpp"
#include "Decoder.hpp"
#include "Status.hpp"
namespace rv32i {

ExecutionResult run_program(Interpreter& cpu, size_t cycle_limit)
{
    ExecutionResult res{ExecutionStatus::Success, 0, 0, 0};

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
            std::cerr << "I'm here!" << "\n";
            res.status = st;
            return res;
        }
    }

    std::cerr << "I'm here!" << "\n";

    res.status = ExecutionStatus::TrapIllegal; // timeout / exceeded cycles
    return res;
}

} // namespace rv32i
