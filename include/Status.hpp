#pragma once

namespace rv32i {

enum class ExecutionStatus 
{ 
    Success, 
    TrapIllegal, 
    TrapLoadFault,
    TrapStoreFault 
};

} // namespace rv32i

