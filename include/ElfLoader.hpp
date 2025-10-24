#pragma once

#include <string>
#include <vector>
#include "IntTypes.hpp"
#include "Interpreter.hpp"

namespace rv32i {

struct ElfLoadResult
{
    u32 entry     = 0; // inital pc
    u32 min_vaddr = 0xFFFFFFFF; // lowest mapped vaddr
    u32 max_vaddr = 0; // highest mapped segment addr (vaddr + memsz)
    u32 sp        = 0; // initial sp
};

ElfLoadResult loadElf(
    Interpreter& cpu,
    const std::string& elf_path,
    const std::vector<std::string>& args,
    u32 stack_top_hint);

} // namespace rv32i

