#pragma once

#include "InterpreterState.hpp"
#include "Status.hpp"

namespace rv32i {

    ExecutionStatus handle_syscall(InterpreterState& s);

} // namespace rv32i

