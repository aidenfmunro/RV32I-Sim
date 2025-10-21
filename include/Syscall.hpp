#pragma once

#include "InterpreterState.hpp"
#include "Status.hpp"

namespace rv32i {

enum Syscall : u32 
{
    READ   = 63,
    WRITE  = 64,
    EXIT   = 93
};

ExecutionStatus handle_syscall(InterpreterState& s);

} // namespace rv32i

