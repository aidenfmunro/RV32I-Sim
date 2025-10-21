#pragma once

#include <utility>
#include "InstrInfo.hpp"

namespace rv32i {

struct Decoder 
{
    static std::pair<InstrInfo, u32> decode(u32 instr_word, u32 pc); 
};

} // namespace rv32i

