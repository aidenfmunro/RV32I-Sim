#pragma once
#include "IntTypes.hpp"

namespace rv32i {

struct InstrInfo
{
    u8  rd       = 0;
    u8  rs1      = 0;
    u8  rs2      = 0;
    u32 imm      = 0;   // sign-extended where relevant
    u8  shamt    = 0;
    u32 pc       = 0;   
};

} // namespace rv32i
