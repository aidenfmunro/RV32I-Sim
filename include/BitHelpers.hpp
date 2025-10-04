#pragma once

#include "IntTypes.hpp"
namespace rv32i {

u8 get_opcode(u32 w); 

u8 get_rd(u32 w);     

u8 get_funct3(u32 w); 

u8 get_rs1(u32 w); 

u8 get_rs2(u32 w);    

u8 get_funct7(u32 w); 

s32 get_imm_i(u32 w); 

s32 get_imm_s(u32 w); 

s32 get_imm_b(u32 w); 

s32 get_imm_u(u32 w); 

s32 get_imm_j(u32 w); 

} // namespace rv32i
