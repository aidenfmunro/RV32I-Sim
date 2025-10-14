#pragma once

//> Unified Format abstractions 
//> Each Format implements a template `execute<Oper>(state, info)` that performs the
//> shared operand extraction and calls Oper's semantics
#include <iostream>
#include "IntTypes.hpp"
#include "InterpreterState.hpp"
#include "Status.hpp"
#include "InstrInfo.hpp"

namespace rv32i {

struct FormatR 
{
    template<typename Oper>
    static ExecutionStatus execute(InterpreterState& s, InstrInfo const& info) 
    {
        u32 a = s.regs[info.rs1];
        u32 b = s.regs[info.rs2];

        u32 r = Oper::exec(a,b);

        if (info.rd != 0) 
            s.regs[info.rd] = r;

        s.pc = info.pc + 4u;

        return ExecutionStatus::Success;
    }
};

struct FormatI 
{
    template<typename Oper>
    static ExecutionStatus execute(InterpreterState& s, InstrInfo const& info) 
    {
        u32 a = s.regs[info.rs1];
        s32 imm = static_cast<s32>(info.imm);

        u32 r = Oper::exec(a, imm);

        if (info.rd != 0) 
            s.regs[info.rd] = r;

        s.pc = info.pc + 4u;

        return ExecutionStatus::Success;
    }
};

struct FormatLoad  //> load variants handled by Oper type selection
{
    template<typename Oper>
    static ExecutionStatus execute(InterpreterState& s, InstrInfo const& info) 
    {
        u32 addr = s.regs[info.rs1] + static_cast<s32>(info.imm);

        if constexpr (std::is_same_v<Oper, struct LbOp>) 
        {
            s.regs[info.rd] = static_cast<s32>(static_cast<int8_t>(s.memory.LoadU8(addr)));
        } 
        else if constexpr (std::is_same_v<Oper, struct LbuOp>) 
        {
            s.regs[info.rd] = s.memory.LoadU8(addr);
        } 
        else if constexpr (std::is_same_v<Oper, struct LhOp>) 
        {
            s.regs[info.rd] = static_cast<s32>(static_cast<s16>(s.memory.LoadU16(addr)));
        } 
        else if constexpr (std::is_same_v<Oper, struct LhuOp>) 
        {
            s.regs[info.rd] = s.memory.LoadU16(addr);
        } 
        else 
        {
            s.regs[info.rd] = s.memory.LoadU32(addr);
        }

        s.pc = info.pc + 4u;

        return ExecutionStatus::Success;
    }
};

struct FormatS 
{
    template<typename Oper>
    static ExecutionStatus execute(InterpreterState& s, InstrInfo const& info) 
    {
        u32 addr = s.regs[info.rs1] + static_cast<s32>(info.imm);

        u32 val = s.regs[info.rs2];

        if constexpr (std::is_same_v<Oper, struct SbOp>) 
        {
            s.memory.StoreU8(addr, static_cast<u8>(val & 0xFF));
        } 
        else if constexpr (std::is_same_v<Oper, struct ShOp>) 
        {
            s.memory.StoreU16(addr, static_cast<uint16_t>(val & 0xFFFF));
        }
        else 
        {
            s.memory.StoreU32(addr, val);
        }

        s.pc = info.pc + 4u;

        return ExecutionStatus::Success;
    }
};

struct FormatB 
{
    template<typename Oper>
    static ExecutionStatus execute(InterpreterState& s, InstrInfo const& info) 
    {
        u32 a = s.regs[info.rs1];
        u32 b = s.regs[info.rs2];

        if (Oper::cond(a,b)) 
        {
            s.pc = info.pc + static_cast<s32>(info.imm);
        } 
        else 
        {
            s.pc = info.pc + 4u;
        }

        return ExecutionStatus::Success;
    }
};

struct FormatU 
{
    template<typename Oper>
    static ExecutionStatus execute(InterpreterState& s, InstrInfo const& info) 
    {
        if constexpr (std::is_same_v<Oper, struct LuiOp>) 
        {
            if (info.rd != 0) s.regs[info.rd] = info.imm;
        } 
        else if constexpr (std::is_same_v<Oper, struct AuipcOp>) 
        {
            if (info.rd != 0) s.regs[info.rd] = info.pc + info.imm;
        }

        s.pc = info.pc + 4u;

        return ExecutionStatus::Success;
    }
};

struct FormatJ 
{
    template<typename Oper>
    static ExecutionStatus execute(InterpreterState& s, InstrInfo const& info) 
    {
        u32 link = info.pc + 4u;

        if (info.rd != 0) 
            s.regs[info.rd] = link;

        s.pc = info.pc + static_cast<s32>(info.imm);

        return ExecutionStatus::Success;
    }
};

struct FormatJalr 
{
    template<typename Oper>
    static ExecutionStatus execute(InterpreterState& s, InstrInfo const& info) 
    {
        u32 link = info.pc + 4u;
        u32 target = (s.regs[info.rs1] + static_cast<s32>(info.imm)) & ~uint32_t(1);

        if (info.rd != 0) 
            s.regs[info.rd] = link;

        s.pc = target;

        return ExecutionStatus::Success;
    }
};

} // namespace rv32i
