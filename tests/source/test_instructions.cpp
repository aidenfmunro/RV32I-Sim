#include <gtest/gtest.h>
#include <iostream>

#include "Interpreter.hpp"
#include "Decoder.hpp"
#include "Encoder.hpp"
#include "Opcodes.hpp"
#include "Formats.hpp"
#include "Opearations.hpp"
#include "HandlerFactory.hpp"

#include "Handlers.hpp"

using namespace rv32i;

class Rv32iTest : public ::testing::Test 
{
protected:
    Interpreter cpu;

    void SetUp() override 
    {
        register_all_handlers(cpu); 

        cpu.state.pc = 0x1000;

        for (auto& r : cpu.state.regs) r = 0;

        cpu.state.memory.clear();
    }

    void run(u32 instr) 
    {
        auto [info, key] = Decoder::decode(instr, cpu.pc());
        cpu.dispatch(cpu.state, info, key);
    }
};

TEST_F(Rv32iTest, ADD_AddsTwoRegisters) 
{
    cpu.state.regs[1] = 5;
    cpu.state.regs[2] = 7;

    REncoding e{0x00, 2, 1, 0x0, 3, Opcode::R_TYPE}; 
    u32 instr = encode(e);
    run(instr);

    EXPECT_EQ(cpu.state.regs[3], 12u);
    EXPECT_EQ(cpu.state.pc, 0x1004u);
}

TEST_F(Rv32iTest, SUB_SubtractsRegisters) 
{
    cpu.state.regs[1] = 10;
    cpu.state.regs[2] = 3;

    REncoding e{0x20, 2, 1, 0x0, 3, Opcode::R_TYPE};
    u32 instr = encode(e);
    run(instr);

    EXPECT_EQ(cpu.state.regs[3], 7u);
}

TEST_F(Rv32iTest, ADDI_AddsImmediate) 
{
    cpu.state.regs[1] = 10;

    IEncoding e{5, 1, 0x0, 2, Opcode::I_TYPE}; 
    u32 instr = encode(e);
    run(instr);

    EXPECT_EQ(cpu.state.regs[2], 15u);
}

TEST_F(Rv32iTest, SW_LW_StoreAndLoadWord) 
{
    cpu.state.regs[1] = 0x2000;  // base
    cpu.state.regs[2] = 0xABCD1234;

    // sw x2, 0(x1)
    SEncoding s_e{0, 2, 1, 0x2, Opcode::S_TYPE};
    run(encode(s_e));
    cpu.state.memory.dump(cpu.state.regs[1]);

    u32 val = cpu.state.memory.LoadU32(cpu.state.regs[1]);
    std::cerr << "Val: " << std::hex << val << std::dec << std::endl;
    // lw x3, 0(x1)
    IEncoding i_e{0, 1, 0x2, 0x3, Opcode::LOAD};
    run(encode(i_e));

    EXPECT_EQ(cpu.state.regs[3], 0xABCD1234u);
}

TEST_F(Rv32iTest, BEQ_BranchTaken) 
{
    cpu.state.regs[1] = 42;
    cpu.state.regs[2] = 42;

    BEncoding b_e{8, 2, 1, 0x0, Opcode::B_TYPE}; // imm=8, rs2, rs1, funct3=0
    run(encode(b_e));

    EXPECT_EQ(cpu.state.pc, 0x1008u); // branch taken
}

TEST_F(Rv32iTest, BEQ_BranchNotTaken) 
{
    cpu.state.regs[1] = 1;
    cpu.state.regs[2] = 2;

    BEncoding b_e{8, 2, 1, 0x0, Opcode::B_TYPE};
    run(encode(b_e));

    EXPECT_EQ(cpu.state.pc, 0x1004u); // branch not taken
}

TEST_F(Rv32iTest, LUI_LoadUpperImmediate) 
{
    UEncoding u_e{0x12345000, 1, Opcode::U_LUI};
    run(encode(u_e));

    EXPECT_EQ(cpu.state.regs[1], 0x12345000u);
}

TEST_F(Rv32iTest, AUIPC_AddUpperImmediateToPC) 
{
    UEncoding u_e{0x12345000, 1, Opcode::U_AUIPC};
    run(encode(u_e));

    EXPECT_EQ(cpu.state.regs[1], 0x12346000u);
}

TEST_F(Rv32iTest, JAL_JumpsAndStoresReturnAddress) 
{
    JEncoding j_e{8, 1, Opcode::J_TYPE}; // imm=8
    run(encode(j_e));

    EXPECT_EQ(cpu.state.pc, 0x1008u);
    EXPECT_EQ(cpu.state.regs[1], 0x1004u);
}

TEST_F(Rv32iTest, JALR_JumpsIndirectly) 
{
    cpu.state.regs[1] = 0x2000;
    IEncoding e{4, 1, 0x0, 5, Opcode::I_JALR}; // target = x1 + 4
    run(encode(e));

    EXPECT_EQ(cpu.state.pc, 0x2004u);
    EXPECT_EQ(cpu.state.regs[5], 0x1004u);
}

TEST_F(Rv32iTest, SLL_ShiftsLeft) 
{
    cpu.state.regs[1] = 1;
    cpu.state.regs[2] = 3;
    REncoding e{0x00, 2, 1, 0x1, 3, Opcode::R_TYPE};
    run(encode(e));
    EXPECT_EQ(cpu.state.regs[3], 8u);
}

