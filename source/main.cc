#include "InstructionTypes.hh"

int main()
{
    IType i{};

    i.rs1 = 4;

    InstrPrinter{}(i);

    return 0;
}
