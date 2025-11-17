#pragma once

namespace rv32i {

enum class ExecutionStatus
{
    Success,
    TrapIllegal,
    TrapLoadFault,
    TrapStoreFault,
    ProgramExit
};

} // namespace rv32i

