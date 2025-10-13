#include "Syscall.hpp"

#include "Syscall.hpp"
#include <iostream>
#include <cstdlib>

namespace rv32i {

ExecutionStatus handle_syscall(InterpreterState& s) 
{
    const uint32_t syscall_num = s.regs[17]; // a7
    const uint32_t a0 = s.regs[10]; // arg0
    const uint32_t a1 = s.regs[11]; // arg1
    const uint32_t a2 = s.regs[12]; // arg2

    switch (syscall_num) 
    {
        case 63: // read
        {
            if (a0 == 0) 
            { // stdin
                for (uint32_t i = 0; i < a2; ++i) 
                 {
                    char c;
                    if (!std::cin.get(c)) break;
                    s.StoreU8(a1 + i, static_cast<uint8_t>(c));
                }
            }

            s.pc += 4u;

            return ExecutionStatus::Success;
        }

        case 64: // write
        {
            if (a0 == 1) 
                { // stdout
                for (uint32_t i = 0; i < a2; ++i) 
                {
                    char c = static_cast<char>(s.LoadU8(a1 + i));
                    std::cout.put(c);
                }

                std::cout.flush();
            }

            s.pc += 4;

            return ExecutionStatus::Success;
        }

        case 93: // exit
        {
            int exit_code = static_cast<int>(a0);
            std::cout << "\n[Program exited with code " << exit_code << "]\n";
            std::exit(exit_code);
        }

        default:
            std::cerr << "Unsupported syscall: " << syscall_num << "\n";
            return ExecutionStatus::TrapIllegal;
    }
}

} // namespace rv32i

