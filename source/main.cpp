#include <cstdint>
#include <iostream>
#include <iomanip>
#include <ostream>
#include <vector>

#include "Interpreter.hpp"
#include "ElfLoader.hpp"
#include "Formats.hpp"
#include "Opearations.hpp"
#include "Encoder.hpp"
#include "Decoder.hpp"
#include "Status.hpp"
#include "Syscall.hpp"
#include "Debugger.hpp"

using namespace rv32i;

void register_all_handlers(Interpreter& cpu)
{
    auto key = [](u8 opcode, u8 funct3, u8 funct7)
        { return u32(opcode) | (u32(funct3)<<8) | (u32(funct7)<<16); };

    // --------------------
    // R-type (Opcode 0x33)
    // --------------------
    cpu.register_handler(key(Opcode::R_TYPE, 0x0, 0x00), make_handler<FormatR, AddOp>());   // ADD
    cpu.register_handler(key(Opcode::R_TYPE, 0x0, 0x20), make_handler<FormatR, SubOp>());   // SUB
    cpu.register_handler(key(Opcode::R_TYPE, 0x1, 0x00), make_handler<FormatR, SllOp>());   // SLL
    cpu.register_handler(key(Opcode::R_TYPE, 0x2, 0x00), make_handler<FormatR, SltOp>());   // SLT
    cpu.register_handler(key(Opcode::R_TYPE, 0x3, 0x00), make_handler<FormatR, SltuOp>());  // SLTU
    cpu.register_handler(key(Opcode::R_TYPE, 0x4, 0x00), make_handler<FormatR, XorOp>());   // XOR
    cpu.register_handler(key(Opcode::R_TYPE, 0x5, 0x00), make_handler<FormatR, SrlOp>());   // SRL
    cpu.register_handler(key(Opcode::R_TYPE, 0x5, 0x20), make_handler<FormatR, SraOp>());   // SRA
    cpu.register_handler(key(Opcode::R_TYPE, 0x6, 0x00), make_handler<FormatR, OrOp>());    // OR
    cpu.register_handler(key(Opcode::R_TYPE, 0x7, 0x00), make_handler<FormatR, AndOp>());   // AND

    // --------------------
    // I-type arithmetic (Opcode 0x13)
    // --------------------
    cpu.register_handler(key(Opcode::I_TYPE, 0x0, 0x00), make_handler<FormatI, AddiOp>());  // ADDI
    cpu.register_handler(key(Opcode::I_TYPE, 0x2, 0x00), make_handler<FormatI, SltiOp>());  // SLTI
    cpu.register_handler(key(Opcode::I_TYPE, 0x3, 0x00), make_handler<FormatI, SltiuOp>()); // SLTIU
    cpu.register_handler(key(Opcode::I_TYPE, 0x4, 0x00), make_handler<FormatI, XoriOp>());  // XORI
    cpu.register_handler(key(Opcode::I_TYPE, 0x6, 0x00), make_handler<FormatI, OriOp>());   // ORI
    cpu.register_handler(key(Opcode::I_TYPE, 0x7, 0x00), make_handler<FormatI, AndiOp>());  // ANDI
    cpu.register_handler(key(Opcode::I_TYPE, 0x1, 0x00), make_handler<FormatI, SlliOp>());  // SLLI
    cpu.register_handler(key(Opcode::I_TYPE, 0x5, 0x00), make_handler<FormatI, SrliOp>());  // SRLI
    cpu.register_handler(key(Opcode::I_TYPE, 0x5, 0x20), make_handler<FormatI, SraiOp>());  // SRAI

    // --------------------
    // Load (Opcode 0x03)
    // --------------------
    cpu.register_handler(key(Opcode::LOAD, 0x0, 0x00), make_handler<FormatLoad, LbOp>());  // LB
    cpu.register_handler(key(Opcode::LOAD, 0x1, 0x00), make_handler<FormatLoad, LhOp>());  // LH
    cpu.register_handler(key(Opcode::LOAD, 0x2, 0x00), make_handler<FormatLoad, LwOp>());  // LW
    cpu.register_handler(key(Opcode::LOAD, 0x4, 0x00), make_handler<FormatLoad, LbuOp>()); // LBU
    cpu.register_handler(key(Opcode::LOAD, 0x5, 0x00), make_handler<FormatLoad, LhuOp>()); // LHU

    // --------------------
    // Store (Opcode 0x23)
    // --------------------
    cpu.register_handler(key(Opcode::S_TYPE, 0x0, 0x00), make_handler<FormatS, SbOp>()); // SB
    cpu.register_handler(key(Opcode::S_TYPE, 0x1, 0x00), make_handler<FormatS, ShOp>()); // SH
    cpu.register_handler(key(Opcode::S_TYPE, 0x2, 0x00), make_handler<FormatS, SwOp>()); // SW

    // --------------------
    // Branch (Opcode 0x63)
    // --------------------
    cpu.register_handler(key(Opcode::B_TYPE, 0x0, 0x00), make_handler<FormatB, BeqOp>());  // BEQ
    cpu.register_handler(key(Opcode::B_TYPE, 0x1, 0x00), make_handler<FormatB, BneOp>());  // BNE
    cpu.register_handler(key(Opcode::B_TYPE, 0x4, 0x00), make_handler<FormatB, BltOp>());  // BLT
    cpu.register_handler(key(Opcode::B_TYPE, 0x5, 0x00), make_handler<FormatB, BgeOp>());  // BGE
    cpu.register_handler(key(Opcode::B_TYPE, 0x6, 0x00), make_handler<FormatB, BltuOp>()); // BLTU
    cpu.register_handler(key(Opcode::B_TYPE, 0x7, 0x00), make_handler<FormatB, BgeuOp>()); // BGEU

    // --------------------
    // Upper immediates (Opcode 0x37, 0x17)
    // --------------------
    cpu.register_handler(key(Opcode::U_LUI, 0x0, 0x00), make_handler<FormatU, LuiOp>());   // LUI
    cpu.register_handler(key(Opcode::U_AUIPC, 0x0, 0x00), make_handler<FormatU, AuipcOp>()); // AUIPC

    // --------------------
    // Jump (Opcode 0x6F, 0x67)
    // --------------------
    cpu.register_handler(key(Opcode::J_TYPE, 0x0, 0x00), make_handler<FormatJ, JalOp>());   // JAL
    cpu.register_handler(key(Opcode::I_JALR, 0x0, 0x00), make_handler<FormatJalr, JalrOp>()); // JALR

    // --------------------
    // System (Opcode 0x73)
    // --------------------
    cpu.register_handler(key(Opcode::SYSTEM, 0x0, 0x00),
         [](InterpreterState& s, InstrInfo const&) -> ExecutionStatus { return handle_syscall(s); }
    ); // ECALL
}


int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <program.elf> [args...]\n";
        return 1;
    }

    Debugger dbg(true /* enable */, "");

    try {
        // 1. Load ELF and setup stack
        std::vector<std::string> args(argv + 1, argv + argc);
        Interpreter cpu;
        auto load = loadElfAndSetupStack(cpu, argv[1], args, 0);

        // 2. Register all instruction handlers
        register_all_handlers(cpu);

        // 3. Run interpreter loop
        const int CYCLE_LIMIT = 1000000;
        for (int cycles = 0; cycles < CYCLE_LIMIT; ++cycles)
        {
            u32 instr = cpu.load<u32>(cpu.pc());
            auto [info, key] = Decoder::decode(instr, cpu.pc());

            dbg.dump_pc(cpu.state);
            dbg.trace_instruction(info, instr);

            ExecutionStatus st = cpu.dispatch(cpu.state, info, key);
            
            dbg.dump_registers(cpu.state);

            if (st != ExecutionStatus::Success)
                break;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

