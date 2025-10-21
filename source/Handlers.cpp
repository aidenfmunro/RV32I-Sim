#include "Handlers.hpp"
#include "Opcodes.hpp"
#include "Formats.hpp"
#include "Opearations.hpp"
#include "Syscall.hpp"
#include "IntTypes.hpp"
#include "HandlerFactory.hpp"

namespace rv32i {

void register_all_handlers(Interpreter& cpu)
{
    auto key = [](u8 opcode, u8 funct3, u8 funct7)
        { return u32(opcode) | (u32(funct3)<<8) | (u32(funct7)<<16); };

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

    cpu.register_handler(key(Opcode::I_TYPE, 0x0, 0x00), make_handler<FormatI, AddiOp>());  // ADDI
    cpu.register_handler(key(Opcode::I_TYPE, 0x2, 0x00), make_handler<FormatI, SltiOp>());  // SLTI
    cpu.register_handler(key(Opcode::I_TYPE, 0x3, 0x00), make_handler<FormatI, SltiuOp>()); // SLTIU
    cpu.register_handler(key(Opcode::I_TYPE, 0x4, 0x00), make_handler<FormatI, XoriOp>());  // XORI
    cpu.register_handler(key(Opcode::I_TYPE, 0x6, 0x00), make_handler<FormatI, OriOp>());   // ORI
    cpu.register_handler(key(Opcode::I_TYPE, 0x7, 0x00), make_handler<FormatI, AndiOp>());  // ANDI
    cpu.register_handler(key(Opcode::I_TYPE, 0x1, 0x00), make_handler<FormatI, SlliOp>());  // SLLI
    cpu.register_handler(key(Opcode::I_TYPE, 0x5, 0x00), make_handler<FormatI, SrliOp>());  // SRLI
    cpu.register_handler(key(Opcode::I_TYPE, 0x5, 0x20), make_handler<FormatI, SraiOp>());  // SRAI

    cpu.register_handler(key(Opcode::LOAD, 0x0, 0x00), make_handler<FormatLoad, LbOp>());  // LB
    cpu.register_handler(key(Opcode::LOAD, 0x1, 0x00), make_handler<FormatLoad, LhOp>());  // LH
    cpu.register_handler(key(Opcode::LOAD, 0x2, 0x00), make_handler<FormatLoad, LwOp>());  // LW
    cpu.register_handler(key(Opcode::LOAD, 0x4, 0x00), make_handler<FormatLoad, LbuOp>()); // LBU
    cpu.register_handler(key(Opcode::LOAD, 0x5, 0x00), make_handler<FormatLoad, LhuOp>()); // LHU

    cpu.register_handler(key(Opcode::S_TYPE, 0x0, 0x00), make_handler<FormatS, SbOp>()); // SB
    cpu.register_handler(key(Opcode::S_TYPE, 0x1, 0x00), make_handler<FormatS, ShOp>()); // SH
    cpu.register_handler(key(Opcode::S_TYPE, 0x2, 0x00), make_handler<FormatS, SwOp>()); // SW

    cpu.register_handler(key(Opcode::B_TYPE, 0x0, 0x00), make_handler<FormatB, BeqOp>());  // BEQ
    cpu.register_handler(key(Opcode::B_TYPE, 0x1, 0x00), make_handler<FormatB, BneOp>());  // BNE
    cpu.register_handler(key(Opcode::B_TYPE, 0x4, 0x00), make_handler<FormatB, BltOp>());  // BLT
    cpu.register_handler(key(Opcode::B_TYPE, 0x5, 0x00), make_handler<FormatB, BgeOp>());  // BGE
    cpu.register_handler(key(Opcode::B_TYPE, 0x6, 0x00), make_handler<FormatB, BltuOp>()); // BLTU
    cpu.register_handler(key(Opcode::B_TYPE, 0x7, 0x00), make_handler<FormatB, BgeuOp>()); // BGEU

    cpu.register_handler(key(Opcode::U_LUI, 0x0, 0x00), make_handler<FormatU, LuiOp>());   // LUI
    cpu.register_handler(key(Opcode::U_AUIPC, 0x0, 0x00), make_handler<FormatU, AuipcOp>()); // AUIPC

    cpu.register_handler(key(Opcode::J_TYPE, 0x0, 0x00), make_handler<FormatJ, JalOp>());   // JAL
    cpu.register_handler(key(Opcode::I_JALR, 0x0, 0x00), make_handler<FormatJalr, JalrOp>()); // JALR

    cpu.register_handler(key(Opcode::SYSTEM, 0x0, 0x00),
         [](InterpreterState& s, InstrInfo const&) -> ExecutionStatus { return handle_syscall(s); }
    ); // ECALL
}

} // namespace rv32i
