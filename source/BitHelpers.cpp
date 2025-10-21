#include "BitHelpers.hpp"

namespace rv32i {

u32 bits(u32 w, int hi, int lo) 
{
    return (w >> lo) & ((1u << (hi - lo + 1)) - 1);
}

u8 get_opcode(u32 w) 
{
    return static_cast<u8>(bits(w, 6, 0)); 
}

u8 get_rd(u32 w)     
{ 
    return static_cast<u8>(bits(w, 11, 7)); 
}

u8 get_funct3(u32 w) 
{ 
    return static_cast<u8>(bits(w, 14, 12)); 
}

u8 get_rs1(u32 w)    
{ 
    return static_cast<u8>(bits(w, 19, 15)); 
}

u8 get_rs2(u32 w)    
{ 
    return static_cast<u8>(bits(w, 24, 20)); 
}

u8 get_funct7(u32 w) 
{ 
    return static_cast<u8>(bits(w, 31, 25)); 
}

s32 get_imm_i(u32 w) 
{
    u32 imm = (w >> 20) & 0xFFF;
    // sign-extend 12-bit immediate
    if (imm & 0x800)
        imm |= 0xFFFFF000;
    return static_cast<s32>(imm);
}

s32 get_imm_s(u32 w) 
{
    s32 imm = ((w >> 25) << 5) | ((w >> 7) & 0x1F);
    return (imm << 20) >> 20; // now imm is signed, sign extends correctly
}

s32 get_imm_b(u32 w) 
{ 
    uint32_t imm = (bits(w, 31, 31) << 12) 
        | (bits(w,  7,  7) << 11) 
        | (bits(w, 30, 25) << 5) 
        | (bits(w, 11,  8) << 1); 

    return static_cast<int32_t>(imm << 19) >> 19; 
}

s32 get_imm_u(u32 w)
{
    return static_cast<int32_t>(w) >> 12 << 12;
}

s32 get_imm_j(u32 w) 
{ 
    uint32_t imm = (bits(w,31,31) << 20) 
        | (bits(w, 19, 12) << 12) 
        | (bits(w, 20, 20) << 11) 
        | (bits(w, 30, 21) << 1); 

    return static_cast<int32_t>(imm << 11) >> 11; 
}

} // namespace rv32i
