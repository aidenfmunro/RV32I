#pragma once

#include "Interpreter.hpp"
#include "Status.hpp"

namespace rv32i {

struct ExecutionResult
{
    ExecutionStatus status;
    size_t pc;
    int cycles;
    int exit_code;    
};

ExecutionResult run_program(Interpreter& cpu, int cycle_limit = 1'000'000);

} // namespace rv32i
