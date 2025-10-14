#pragma once

#include <array>
#include "IntTypes.hpp"
#include "Memory.hpp"

namespace rv32i {

struct InterpreterState 
{
    std::array<u32, 32> regs{};

    u32 pc = 0;

    SparseMemory memory;

    InterpreterState() = default; 
};

}
