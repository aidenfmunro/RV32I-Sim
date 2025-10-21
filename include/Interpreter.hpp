#pragma once

#include <iostream>
#include <unordered_map>
#include "InterpreterState.hpp"
#include "Status.hpp"
#include "HandlerFactory.hpp"

namespace rv32i {

class Interpreter 
{
    std::unordered_map<u32, Handler> handlers_;

public:

    Interpreter() = default;

    InterpreterState state;

    void register_handler(u32 key, Handler h) { handlers_[key] = h; }

    ExecutionStatus dispatch(InterpreterState& s, InstrInfo const& info, u32 key) const 
    {
        std::cerr << "[DISPATCH] key=0x" << std::hex << key << std::dec << std::endl;

        auto it = handlers_.find(key);

        if (it == handlers_.end()) 
        {
            std::cerr << "No handler found for " << key << std::endl;
            return ExecutionStatus::TrapIllegal;
        }

        std::cerr << "Handler found for " << key << std::endl;

        return it->second(s, info);
    }

    template<typename T>
    T load(u32 addr) const 
    {
        if constexpr (std::is_same_v<T, u8>)
            return state.memory.LoadU8(addr);
        else if constexpr (std::is_same_v<T, u16>)
            return state.memory.LoadU16(addr);
        else if constexpr (std::is_same_v<T, u32>)
            return state.memory.LoadU32(addr);
        else
            static_assert(!sizeof(T*), "Unsupported Load<T> type");
    }

    template<typename T>
    void store(u32 addr, T value) 
    {
        if constexpr (std::is_same_v<T, u8>)
            state.memory.StoreU8(addr, value);
        else if constexpr (std::is_same_v<T, u16>)
            state.memory.StoreU16(addr, value);
        else if constexpr (std::is_same_v<T, u32>)
            state.memory.StoreU32(addr, value);
        else
            static_assert(!sizeof(T*), "Unsupported Store<T> type");
    }

    void writeBlock(u32 addr, const u8* src, u32 len)
    {
        state.memory.WriteBlock(addr, src, len);
    }

    u32& pc() { return state.pc; }
    const u32& pc() const { return state.pc; }

    u32& reg(int i) { return state.regs[i]; }
    const u32& reg(int i) const { return state.regs[i]; }
};

} // namespace rv32i
