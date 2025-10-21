#pragma once

#include "Status.hpp"
#include "InstrInfo.hpp"
#include "InterpreterState.hpp"

namespace rv32i {

using Handler = ExecutionStatus(*)(InterpreterState&, InstrInfo const&);

template<typename Format, typename Oper>
constexpr Handler make_handler() 
{
    return [](InterpreterState& s, InstrInfo const& info)->ExecutionStatus 
    {
        return Format::template execute<Oper>(s, info);
    };
}

} // namespace rv32i
