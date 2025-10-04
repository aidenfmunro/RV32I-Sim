#pragma once

#include "IntTypes.hpp"

namespace rv32i {

enum Opcode : u8 {
    R_TYPE    = 0x33,
    I_TYPE    = 0x13, // immediate arithmetic
    LOAD      = 0x03,
    S_TYPE    = 0x23,
    B_TYPE    = 0x63,
    U_LUI     = 0x37,
    U_AUIPC   = 0x17,
    J_TYPE    = 0x6F,
    I_JALR    = 0x67,
    FENCE     = 0x0F,
    SYSTEM    = 0x73
};

}
