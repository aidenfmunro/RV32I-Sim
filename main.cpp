#include <iostream>
#include <vector>

#include "Interpreter.hpp"
#include "ElfLoader.hpp"
#include "Decoder.hpp"
#include "Status.hpp"
#include "Debugger.hpp"

#include "Handlers.hpp"

using namespace rv32i;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <program.elf> [args...]\n";
        return 1;
    }

    Debugger dbg(true /* enable */, "trace.log", argv[1]);

    try {
        std::vector<std::string> args(argv + 1, argv + argc);
        Interpreter cpu;
        auto load = loadElf(cpu, argv[1], args, 0);

        register_all_handlers(cpu);

        const int CYCLE_LIMIT = 1000000;
        for (int cycles = 0; cycles < CYCLE_LIMIT; ++cycles)
        {
            u32 instr = cpu.load<u32>(cpu.pc());
            auto [info, key] = Decoder::decode(instr, cpu.pc());

            dbg.dump_pc(cpu.state);
            dbg.trace_instruction(info, instr);

            ExecutionStatus st = cpu.dispatch(cpu.state, info, key);

            dbg.dump_registers(cpu.state);

            if (st != ExecutionStatus::Success)
                break;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

