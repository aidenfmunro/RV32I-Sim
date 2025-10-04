#include <cstdint>
#include <iostream>
#include <iomanip>
#include <ostream>

#include "Interpreter.hpp"
#include "Formats.hpp"
#include "Opearations.hpp"
#include "Encoder.hpp"
#include "Decoder.hpp"

using namespace rv32i;

int main(){
    Interpreter interp;
    auto key = [](u8 opcode, uint8_t funct3, uint8_t funct7){ return u32(opcode) | (u32(funct3)<<8) | (uint32_t(funct7)<<16); };

    // examples
    interp.register_handler(key(0x33, 0x0, 0x00),  make_handler<FormatR,    AddOp>()); // ADD
    interp.register_handler(key(0x33, 0x0, 0x20),  make_handler<FormatR,    SubOp>()); // SUB
    interp.register_handler(key(0x33, 0x7, 0x00),  make_handler<FormatR,    AndOp>()); // AND
    interp.register_handler(key(0x33, 0x6, 0x00),  make_handler<FormatR,    OrOp>());  // OR
    interp.register_handler(key(0x13, 0x0, 0x00),  make_handler<FormatI,    AddiOp>()); // ADDI
    interp.register_handler(key(0x13, 0x2, 0x00),  make_handler<FormatI,    SltiOp>()); // SLTI
    interp.register_handler(key(0x03, 0x2, 0x00),  make_handler<FormatLoad, LwOp>()); // LW
    interp.register_handler(key(0x23, 0x2, 0x00),  make_handler<FormatS,    SwOp>());    // SW
    interp.register_handler(key(0x63, 0x0, 0x00),  make_handler<FormatB,    BeqOp>());   // BEQ
    interp.register_handler(key(0x63, 0x1, 0x00),  make_handler<FormatB,    BneOp>());   // BNE
    interp.register_handler(key(0x6F, 0x0, 0x00),  make_handler<FormatJ,    JalOp>());   // JAL
    interp.register_handler(key(0x67, 0x0, 0x00),  make_handler<FormatJalr, JalrOp>());// JALR
    interp.register_handler(key(0x37, 0x0, 0x00),  make_handler<FormatU,    LuiOp>());   // LUI
    interp.register_handler(key(0x17, 0x0, 0x00),  make_handler<FormatU,    AuipcOp>()); // AUIPC

    InterpreterState state(1024);
    state.pc = 0;
    state.regs.fill(0);

    // Build program
    std::vector<u32> prog;
 
    prog.push_back(encode(IEncoding{10, 0, 0x0, 1, 0x13}));   // addi x1, x0, 10
    prog.push_back(encode(IEncoding{20, 0, 0x0, 2, 0x13}));   // addi x2, x0, 20
    prog.push_back(encode(REncoding{0x00, 2, 1, 0x0, 3, 0x33})); // add x3, x1, x2
    prog.push_back(encode(SEncoding{0, 3, 0, 0x2, 0x23}));    // sw x3, 0(x0)
    prog.push_back(encode(IEncoding{0, 0, 0x2, 4, 0x03}));    // lw x4, 0(x0)
    prog.push_back(encode(BEncoding{8, 4, 3, 0x0, 0x63}));    // beq x4, x3, +8
    prog.push_back(encode(IEncoding{1, 0, 0x0, 5, 0x13}));    // addi x5, x0, 1 (skipped)
    prog.push_back(encode(JEncoding{8, 0, 0x6F}));            // jal x0, +8
    prog.push_back(encode(IEncoding{7, 0, 0x0, 6, 0x13}));    // addi x6, x0, 7
    prog.push_back(encode(UEncoding{0x12345000, 7, 0x37}));   // lui x7, 0x12345
    prog.push_back(encode(IEncoding{4, 0, 0x0, 8, 0x67}));    // jalr x8, x0, 4
    prog.push_back(encode(IEncoding{0, 0, 0x0, 0, 0x13}));    // nop
    
    for (size_t i = 0; i < prog.size(); ++i) state.StoreU32(u32(i * 4), prog[i]);

    const int CYCLE_LIMIT = 200;
    int cycles = 0;
    while (cycles < CYCLE_LIMIT) 
    {
        u32 instr_word = state.LoadU32(state.pc);

        auto [info, keyval] = Decoder::decode(instr_word, state.pc);

        ExecutionStatus st = interp.dispatch(state, info, keyval);

        std::cout << state.regs[1] << std::endl;

        if (st != ExecutionStatus::Success) 
        { 
            std::cerr << "Trap at pc=0x"<< std::hex << info.pc << ""; 
            break; 
        }
            cycles++;

        if (state.pc > prog.size() * 4) 
            break;
    }

    return 0;
}

