#pragma once

#include "regfile.hh"
#include "memory.hh"

struct CPU
{
    Memory mem;
    RegisterFile regs;
    uint32_t pc = 0;

  public:

    explicit CPU(std::size_t memsize) : mem(memsize) {}
};
