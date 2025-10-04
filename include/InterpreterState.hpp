#pragma once

#include <array>
#include <vector>
#include "IntTypes.hpp"

namespace rv32i {

struct InterpreterState {
    std::array<u32, 32> regs{};

    u32 pc = 0;

    std::vector<u8> memory;

    InterpreterState(size_t mem_bytes = 4096) 
    : memory(mem_bytes, 0) 
    {}

    u8  LoadU8(u32 addr)  
    { 
        return memory.at(addr); 
    }

    u16 LoadU16(u32 addr) 
    { 
        uint16_t lo = memory.at(addr); 
        uint16_t hi = memory.at(addr+1); 

        return uint16_t(lo | (hi<<8));
    }

    u32 LoadU32(u32 addr) 
    { 
        uint32_t b0 = memory.at(addr); 
        uint32_t b1 = memory.at(addr+1); 
        uint32_t b2 = memory.at(addr+2); 
        uint32_t b3 = memory.at(addr+3); 

        return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24); 
    }

    void StoreU8(u32 addr, u8 v)  
    { 
        memory.at(addr) = v; 
    }

    void StoreU16(u32 addr, u16 v) 
    { 
        memory.at(addr)   = u8(v & 0xFF); 
        memory.at(addr+1) = u8((v >> 8) & 0xFF); 
    }
    
    void StoreU32(u32 addr, u32 v) 
    { 
        memory.at(addr)     = u8(v & 0xFF); 
        memory.at(addr + 1) = u8((v >> 8)  & 0xFF); 
        memory.at(addr + 2) = u8((v >> 16) & 0xFF); 
        memory.at(addr + 3) = u8((v >> 24) & 0xFF); 
    }
};

}
