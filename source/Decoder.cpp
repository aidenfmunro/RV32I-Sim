#include "Decoder.hpp"
#include "BitHelpers.hpp"
#include "Opcodes.hpp"

namespace rv32i {

std::pair<InstrInfo, u32> Decoder::decode(u32 instr_word, u32 pc)
{
    InstrInfo info{};

    info.pc    = pc;
    info.rd    = get_rd(instr_word);
    info.rs1   = get_rs1(instr_word);
    info.rs2   = get_rs2(instr_word);
    info.shamt = info.rs2;
    u8 opcode  = get_opcode(instr_word);
    u8 funct3  = get_funct3(instr_word);
    u8 funct7  = get_funct7(instr_word);

    switch (static_cast<int>(opcode))
    {
        case Opcode::R_TYPE:
            info.imm = 0;
            break; // R
        case Opcode::I_TYPE:
        case Opcode::LOAD:
        case Opcode::I_JALR:
            info.imm = static_cast<u32>(get_imm_i(instr_word));
            break;
        case Opcode::S_TYPE:
            info.imm = static_cast<u32>(get_imm_s(instr_word));
            break;
        case Opcode::B_TYPE:
            info.imm = static_cast<u32>(get_imm_b(instr_word));
            break;
        case Opcode::U_LUI:
        case Opcode::U_AUIPC:
            info.imm = static_cast<u32>(get_imm_u(instr_word)); break;
        case Opcode::J_TYPE:
            info.imm = static_cast<u32>(get_imm_j(instr_word)); break;
        case Opcode::SYSTEM:
            
        default:
            info.imm = 0;
            break; // FIXME: give default type!
    }

    u32 key = u32(opcode) | (u32(funct3) << 8) | (u32(funct7) << 16);

    return {info, key};
}

}
