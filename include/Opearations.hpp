#pragma once

#include "IntTypes.hpp"

namespace rv32i {

struct AddOp    
{ 
    static constexpr const char* name = "add";  
    static u32 exec(u32 a, u32 b){ return a + b; } 
};

struct SubOp    
{ 
    static constexpr const char* name = "sub";  
    static u32 exec(u32 a, u32 b){ return a - b; } 
};

struct AndOp    
{ 
    static constexpr const char* name = "and";  
    static u32 exec(u32 a, u32 b){ return a & b; } 
};

struct OrOp     
{
    static constexpr const char* name = "or";   
    static u32 exec(u32 a, u32 b){ return a | b; } 
};

struct XorOp    
{ 
    static constexpr const char* name = "xor";  
    static u32 exec(u32 a, u32 b){ return a ^ b; } 
};

struct SllOp    
{ 
    static constexpr const char* name = "sll";  
    static u32 exec(u32 a, u32 b){ return a << (b & 31); } 
};

struct SrlOp    
{ 
    static constexpr const char* name = "srl"; 
    static u32 exec(u32 a, u32 b){ return a >> (b & 31); } 
};

struct SraOp    
{ 
    static constexpr const char* name = "sra";  
    static u32 exec(u32 a, u32 b)
    { 
        return static_cast< u32>(static_cast<s32>(a) >> (b & 31)); 
    } 
};

struct SltOp    
{ 
    static constexpr const char* name = "slt";  
    static u32 exec(u32 a, u32 b)
    { 
        return (static_cast<s32>(a) < static_cast<s32>(b)) ? 1u : 0u; 
    } 
};

struct SltuOp   
{ 
    static constexpr const char* name = "sltu"; 
    static u32 exec(u32 a, u32 b)
    { 
        return (a < b) ? 1u : 0u; 
    } 
};

struct AddiOp   
{ 
    static constexpr const char* name = "addi"; 
    static u32 exec(u32 a, s32 imm)
    { 
        return a + imm; 
    } 
};

struct SltiOp   
{ 
    static constexpr const char* name = "slti"; 
    static u32 exec(u32 a,s32 imm)
    { 
        return (static_cast<s32>(a) < imm) ? 1u : 0u; 
    } 
};

struct SltiuOp  
{ 
    static constexpr const char* name = "sltiu"; 
    static u32 exec(u32 a, s32 imm)
    {
        return (a < static_cast< u32>(imm)) ? 1u : 0u; 
    } 
};

struct AndiOp   
{ 
    static constexpr const char* name = "andi"; 
    static u32 exec(u32 a, s32 imm)
    { 
        return a & static_cast< u32>(imm); 
    } 
};

struct OriOp    
{ 
    static constexpr const char* name = "ori";  
    static u32 exec(u32 a, s32 imm)
    { 
        return a | static_cast< u32>(imm); 
    } 
};

struct XoriOp   
{ 
    static constexpr const char* name = "xori"; 
    static u32 exec(u32 a, s32 imm)
    { 
        return a ^ static_cast< u32>(imm); 
    } 
};

struct SlliOp   
{ 
    static constexpr const char* name = "slli"; 
    static u32 exec(u32 a, s32 sh)
    { 
        return a << (sh & 31); 
    } 
};

struct SrliOp   
{ 
    static constexpr const char* name = "srli"; 
    static u32 exec(u32 a, s32 sh)
    { 
        return a >> (sh & 31); 
    } 
};

struct SraiOp   
{ 
    static constexpr const char* name = "srai"; 
    static u32 exec(u32 a, s32 sh)
    { 
        return static_cast<u32>(static_cast<s32>(a) >> (sh & 31)); 
    } 
};

struct LuiOp    
{ 
    static constexpr const char* name = "lui"; 
};

struct AuipcOp  { static constexpr const char* name = "auipc"; };

struct LbOp     { static constexpr const char* name = "lb"; };
struct LbuOp    { static constexpr const char* name = "lbu"; };
struct LhOp     { static constexpr const char* name = "lh"; };
struct LhuOp    { static constexpr const char* name = "lhu"; };
struct LwOp     { static constexpr const char* name = "lw"; };

struct SbOp     { static constexpr const char* name = "sb"; };
struct ShOp     { static constexpr const char* name = "sh"; };
struct SwOp     { static constexpr const char* name = "sw"; };

struct BeqOp  
{ 
    static constexpr const char* name = "beq";  
    static bool cond(u32 a, u32 b)
    { 
        return a == b; 
    } 
};

struct BneOp  
{ 
    static constexpr const char* name = "bne";  
    static bool cond(u32 a, u32 b)
    { 
        return a != b; 
    } 
};

struct BltOp  
{ 
    static constexpr const char* name = "blt"; 
    static bool cond(u32 a, u32 b)
    { 
        return static_cast<s32>(a) < static_cast<s32>(b); 
    } 
};

struct BgeOp  
{ 
    static constexpr const char* name = "bge";  
    static bool cond(u32 a, u32 b)
    { 
        return static_cast<s32>(a) >= static_cast<s32>(b); 
    } 
};

struct BltuOp 
{ 
    static constexpr const char* name = "bltu"; 
    static bool cond(u32 a,u32 b)
    { 
        return a < b; 
    } 
};

struct BgeuOp 
{ 
    static constexpr const char* name = "bgeu"; 
    static bool cond(u32 a,u32 b)
    { 
        return a >= b; 
    } };

struct JalOp  { static constexpr const char* name = "jal"; };
struct JalrOp { static constexpr const char* name = "jalr"; };

} // namespace rv32i
