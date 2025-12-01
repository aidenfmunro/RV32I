#include <iostream>
#include <vector>

#include "Handlers.hpp"
#include "Interpreter.hpp"
#include "ElfLoader.hpp"
#include "Status.hpp"
#include "Runner.hpp"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <program.elf> [args...]\n";

        return 1;
    }

    std::vector<std::string> args(argv + 1, argv + argc);
    rv32i::Interpreter cpu{};


    auto load = rv32i::loadElf(cpu, argv[1], args, 0);

    rv32i::register_all_handlers(cpu);
    // register_F_extension(cpu); // later

    auto result = rv32i::run_program(cpu);

    if (result.status == rv32i::ExecutionStatus::ProgramExit)
        return result.exit_code;

    if (result.status == rv32i::ExecutionStatus::TrapIllegal)
        std::cerr << "Trap! on instruction: " << result.pc  <<  "\n";

    std::cerr << "Program ended with status " << int(result.status) << "\n";

    return -1;
}

