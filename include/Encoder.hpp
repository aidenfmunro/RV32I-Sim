#pragma once

#include "IntTypes.hpp"
#include <iostream>

namespace rv32i {

struct REncoding 
{ 
    u8 funct7, 
       rs2, 
       rs1, 
       funct3, 
       rd, 
       opcode; 
};

struct IEncoding 
{ 
    s32 imm; 
    u8  rs1, 
        funct3, 
        rd, 
        opcode; 
};

struct SEncoding 
{ 
    s32 imm; 
    u8  rs2, 
        rs1, 
        funct3, 
        opcode; 
};

struct BEncoding 
{
    s32 imm; 
    u8 rs2, 
       rs1, 
       funct3, 
       opcode; 
};

struct UEncoding 
{ 
    s32 imm; 
    u8  rd, 
        opcode; 
};

struct JEncoding 
{ 
    s32 imm; 
    u8  rd, 
        opcode; 
};

//> Traits with encode() for each format

template<typename T>
struct EncodingTraits; 

template<>
struct EncodingTraits<REncoding> 
{
    static u32 encode(const REncoding& e) 
    {
        return (u32(e.funct7) << 25)
             | (u32(e.rs2)    << 20)
             | (u32(e.rs1)    << 15)
             | (u32(e.funct3) << 12)
             | (u32(e.rd)     << 7)
             | u32(e.opcode);
    }
};

template<>
struct EncodingTraits<IEncoding> 
{
    static u32 encode(const IEncoding& e) 
    {
        return ((u32(e.imm) & 0xFFF) << 20)
             | (u32(e.rs1)    << 15)
             | (u32(e.funct3) << 12)
             | (u32(e.rd)     << 7)
             | u32(e.opcode);
    }
};

template<>
struct EncodingTraits<SEncoding> 
{
    static u32 encode(const SEncoding& e) 
    {
        u32 imm11_5 = (u32(e.imm) >> 5) & 0x7F;
        u32 imm4_0  =  u32(e.imm) & 0x1F;
        return (imm11_5       << 25)
             | (u32(e.rs2)    << 20)
             | (u32(e.rs1)    << 15)
             | (u32(e.funct3) << 12)
             | (imm4_0        << 7)
             | u32(e.opcode);
    }
};

template<>
struct EncodingTraits<BEncoding> 
{
    static u32 encode(const BEncoding& e) 
    {
        u32 imm12   = (u32(e.imm) >> 12) & 1;
        u32 imm10_5 = (u32(e.imm) >> 5)  & 0x3F;
        u32 imm4_1  = (u32(e.imm) >> 1)  & 0xF;
        u32 imm11   = (u32(e.imm) >> 11) & 1;
        return (imm12         << 31)
             | (imm10_5       << 25)
             | (u32(e.rs2)    << 20)
             | (u32(e.rs1)    << 15)
             | (u32(e.funct3) << 12)
             | (imm4_1        << 8)
             | (imm11         << 7)
             | u32(e.opcode);
    }
};

template<>
struct EncodingTraits<UEncoding> 
{
    static u32 encode(const UEncoding& e) 
    {
        std::cerr << e.opcode << std::endl;
        return (u32(e.imm) & 0xFFFFF000)
             | (u32(e.rd) << 7)
             | u32(e.opcode);

    }
};

template<>
struct EncodingTraits<JEncoding> 
{
    static u32 encode(const JEncoding& e)
    {
        u32 imm20    = (u32(e.imm) >> 20) & 1;
        u32 imm10_1  = (u32(e.imm) >> 1)  & 0x3FF;
        u32 imm11    = (u32(e.imm) >> 11) & 1;
        u32 imm19_12 = (u32(e.imm) >> 12) & 0xFF;
        return (imm20     << 31)
             | (imm19_12  << 12)
             | (imm11     << 20)
             | (imm10_1   << 21)
             | (u32(e.rd) << 7)
             | u32(e.opcode);
    }
};

//> Generic interface 

template<typename T>
u32 encode(const T& e) 
{
    return EncodingTraits<T>::encode(e);
}

} // namespace rv32i
