#pragma once

#include <fstream>
#include <iomanip>
#include <iostream>
#include "BitHelpers.hpp"
#include "InstrInfo.hpp"
#include "InterpreterState.hpp"

namespace rv32i {

class Debugger
{
    bool enabled;
    std::ofstream logfile;

public:
    explicit Debugger(bool enable = true, const std::string& log_path = "", const std::string& elf_path = "")
    :   enabled(enable)
    {
        if (!log_path.empty())
            logfile.open(log_path);

        out() << "Start trace of" + elf_path;
    }

    void setEnabled(bool e) { enabled = e; }

    std::ostream& out()
    {
        return logfile.is_open() ? logfile : std::cout;
    }

    void dump_pc(const InterpreterState& s)
    {
        if (enabled)
            out() << "PC = 0x" << std::hex << s.pc << std::dec << "\n";
    }

    void dump_registers(const InterpreterState& s) 
    {
        if (enabled)
        {
            out() << "=== Registers ===\n";
            for (int i = 0; i < 32; ++i) 
            {
                out() << "x" << std::setw(2) << std::setfill('0') << i
                      << " = 0x" << std::hex << std::setw(8) << std::setfill('0') << s.regs[i]
                      << std::dec << " (" << static_cast<int32_t>(s.regs[i]) << ")";
                if ((i + 1) % 4 == 0) out() << "\n";
                else out() << "\t";
            }

            out() << "\n";
        }
    }

    void dump_memory(const InterpreterState& s, u32 addr, u32 len = 64) 
    {
        if (enabled)
        {
            out() << "=== Memory [0x" << std::hex << addr << "] ===\n";
            for (u32 i = 0; i < len; i += 4) {
                u32 val = s.memory.LoadU32(addr + i);
                out() << "0x" << std::setw(8) << std::setfill('0') << (addr + i)
                      << ": 0x" << std::setw(8) << val << std::dec << "\n";
            }
        }
    }

    void trace_instruction(const InstrInfo& info, u32 instr_word) 
    {
        if (enabled) 
        {
            out() << std::hex << std::setw(8) << std::setfill('0') << info.pc
                  << ": instr=0x" << std::setw(8) << instr_word
                  << " opcode=" << int(get_opcode(instr_word))
                  << " funct3=" << int(get_funct3(instr_word))
                  << " funct7=" << int(get_funct7(instr_word))
                  << " rd=" << std::dec << int(info.rd)
                  << " rs1=" << int(info.rs1)
                  << " rs2=" << int(info.rs2)
                  << " imm=" << static_cast<s32>(info.imm)
                  << "\n";
        }
    }
    
};

} // namespace rv32i
