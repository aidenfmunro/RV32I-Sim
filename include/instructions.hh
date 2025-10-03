#pragma once

#include <cstdint>

#include "cpu.hh"

class IInstr
{
  public:
    virtual void execute(CPU&) = 0;

    virtual ~IInstr() = default;
};


class Add : public IInstr
{
    uint8_t rd, rs1, rs2;

  public:

    Add(uint8_t d, uint8_t s1, uint8_t s2) : rd(d), rs1(s1), rs2(s2) {}

    void execute(CPU& cpu) override
    {
        cpu.regs[rd] = cpu.regs[rs1] + cpu.regs[rs2];
    }

};

class Sub : public IInstr
{
    uint8_t rd, rs1, rs2;

  public:

    Sub(uint8_t d, uint8_t s1, uint8_t s2) : rd(d), rs1(s1), rs2(s2) {}

    void execute(CPU& cpu) override
    {
        cpu.regs[rd] = cpu.regs[rs1] - cpu.regs[rs2];
    }
};
