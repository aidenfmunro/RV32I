#include <iostream>
#include <vector>

#include "Handlers.hpp"
#include "Interpreter.hpp"
#include "ElfLoader.hpp"

#include "Status.hpp"
#include "Runner.hpp"

using namespace rv32i;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <program.elf> [args...]\n";

        return 1;
    }

    std::vector<std::string> args(argv + 1, argv + argc);
    Interpreter cpu{};


    auto load = loadElf(cpu, argv[1], args, 0);

    register_all_handlers(cpu);
    // register_F_extension(cpu); // later

    auto result = run_program(cpu);

    if (result.status == ExecutionStatus::ProgramExit)
        return result.exit_code;

    std::cerr << "Program ended with status " << int(result.status) << "\n";

    return -1;
}

