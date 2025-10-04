#pragma once

#include <unordered_map>
#include "InterpreterState.hpp"
#include "Status.hpp"
#include "HandlerFactory.hpp"

namespace rv32i {

class Interpreter 
{
    std::unordered_map<u32, Handler> handlers_;

public:
    void register_handler(u32 key, Handler h) { handlers_[key] = h; }

    ExecutionStatus dispatch(InterpreterState& s, InstrInfo const& info, u32 key) const 
    {
        auto it = handlers_.find(key);
        if (it == handlers_.end()) return ExecutionStatus::TrapIllegal;
        return it->second(s, info);
    }
};

}
