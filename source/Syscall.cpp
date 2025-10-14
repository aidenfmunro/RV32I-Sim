#include <iostream>
#include <cstdlib>

#include <iostream>
#include "Syscall.hpp"
#include "IntTypes.hpp"
#include "InterpreterState.hpp"

namespace rv32i {

ExecutionStatus handle_syscall(InterpreterState& s) 
{
    const u32 syscall_num = s.regs[17]; // a7
    const u32 a0 = s.regs[10]; // arg0
    const u32 a1 = s.regs[11]; // arg1
    const u32 a2 = s.regs[12]; // arg2

    switch (syscall_num) 
    {
        case 63: // read
        {
            u32 bytesRead = 0;

            if (a0 == 0) 
            { // stdin
                for (u32 i = 0; i < a2; ++i) 
                 {
                    char c;
                    if (!std::cin.get(c)) break;
                    
                    std::cerr << c;
                    s.memory.StoreU8(a1 + i, static_cast<u8>(c));

                    bytesRead++;
                }
            }

            s.regs[10] = bytesRead;

            s.pc += 4u;

            return ExecutionStatus::Success;
        }

        case 64: // write
        {
            if (a0 == 1) 
                { // stdout
                for (u32 i = 0; i < a2; ++i) 
                {
                    char c = static_cast<char>(s.memory.LoadU8(a1 + i));
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

