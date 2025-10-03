#pragma once

#include "IntTypes.hh"

struct Instruction 
{
    u32 raw;

    explicit Instruction(u32 r)
    :   raw(r)
    {}

    u32 opcode() const { return (raw >> 0)  & 0x7f; }
    u32 rd()     const { return (raw >> 7)  & 0x1f; }
    u32 funct3() const { return (raw >> 12) & 0x7;  }
    u32 rs1()    const { return (raw >> 15) & 0x1f; }
    u32 rs2()    const { return (raw >> 20) & 0x1f; }
    u32 funct7() const { return (raw >> 25) & 0x7f; }

    s32 imm_i() const 
    {
        return static_cast<s32>(raw) >> 20;
    }

    s32 imm_s() const
    {
        s32 imm = (((raw >> 31)         << 12)
                | (((raw >> 7 ) & 0x1)  << 11)
                | (((raw >> 25) & 0x3f) << 5 )
                | (((raw >> 8 ) & 0xf)  << 1 ));

        return imm;
    }
};





