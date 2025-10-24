#include <elfio/elfio.hpp>
#include <algorithm>
#include <stdexcept>

#include "ElfLoader.hpp"
namespace rv32i {

static inline u32 align_up(u32 v, u32 a)   { return (v + (a - 1)) & ~(a - 1); }
static inline u32 align_down(u32 v, u32 a) { return v & ~(a - 1); }

                 
// Build argv layout your api.s expects:
//
//   [sp+0]  : argc (u32)
//   [sp+4]  : argv[0] (u32 pointer)
//   [sp+8]  : argv[1]
//   ...
//   [..]    : 0 (terminator)
//   (strings are placed below, NUL-terminated)
//
// Returns final sp.

static u32 setupStack(Interpreter& cpu,
                       u32 stack_top,
                       const std::vector<std::string>& args)
{
    // Reserve a small scratch area below stack_top for strings
    u32 cursor = align_down(stack_top - 4096u, 16u);

    // Copy argument strings first (downwards) and remember their addresses
    std::vector<u32> argv_ptrs;
    argv_ptrs.reserve(args.size());

    for (const auto& s : args) {
        const u32 len = static_cast<u32>(s.size());
        const u32 start = cursor - (len + 1); // include terminating NUL
        // write bytes
        for (u32 i = 0; i < len; ++i)
            cpu.store<u8>(start + i, static_cast<u8>(s[i]));
        cpu.store<u8>(start + len, 0);
        argv_ptrs.push_back(start);
        // keep 4-byte alignment between strings
        cursor = align_down(start, 4u);
    }

    // Now place argc + argv array at the very top
    const u32 argc_u32 = static_cast<u32>(args.size());
    const u32 argv_bytes = 4u * (argc_u32 + 1u); // + one NULL
    u32 sp = stack_top - (4u /*argc*/ + argv_bytes);

    // argc
    cpu.store<u32>(sp + 0, argc_u32);
    // argv[i]
    for (u32 i = 0; i < argc_u32; ++i)
        cpu.store<u32>(sp + 4u + 4u * i, argv_ptrs[i]);
    // argv terminator
    cpu.store<u32>(sp + 4u /* argc */ + 4u * argc_u32, 0);

    return sp;
}

ElfLoadResult loadElf(
    Interpreter& cpu,
    const std::string& elf_path,
    const std::vector<std::string>& args,
    u32 stack_top_hint)
{
    ELFIO::elfio reader;

    if (!reader.load(elf_path))
        throw std::runtime_error("Failed to load ELF: " + elf_path);

    // Basic sanity for your RV32I interpreter
    if (reader.get_class()    != ELFIO::ELFCLASS32)  
        throw std::runtime_error("ELF is not 32-bit");

    if (reader.get_encoding() != ELFIO::ELFDATA2LSB) 
        throw std::runtime_error("ELF is not little-endian");

    if (reader.get_machine()  != ELFIO::EM_RISCV)    
        throw std::runtime_error("ELF is not RISC-V");

    if (reader.get_type() != ELFIO::ET_EXEC)
        throw std::runtime_error("Unsupported ELF type (need ET_EXEC/ET_DYN)");

    ElfLoadResult res{};
    res.entry = static_cast<u32>(reader.get_entry());

    // Map loadable segments
    for (auto& seg : reader.segments) 
    {
        if (!seg || seg->get_type() != ELFIO::PT_LOAD)
            continue;

        const u32 vaddr  = static_cast<u32>(seg->get_virtual_address());
        const u32 filesz = static_cast<u32>(seg->get_file_size());
        const u32 memsz  = static_cast<u32>(seg->get_memory_size());
        const char* data = seg->get_data();

        if (memsz == 0) 
            continue;

        // Copy file-backed part
        if (filesz)
            cpu.writeBlock(vaddr, reinterpret_cast<const u8*>(data), filesz);

        // Zero BSS tail
        if (memsz > filesz) 
        {
            const u32 zero_base = vaddr + filesz;
            const u32 zero_len  = memsz - filesz;

            for (u32 i = 0; i < zero_len; ++i)
            {
                cpu.store<u8>(zero_base + i, 0);
            }
        }

        res.min_vaddr = std::min(res.min_vaddr, vaddr);
        res.max_vaddr = std::max(res.max_vaddr, vaddr + memsz);
    }

    if (res.max_vaddr == 0)
        throw std::runtime_error("No PT_LOAD segments in ELF");

    // Choose a stack top:
    // - if caller gave a hint, use it (align to 16)
    // - otherwise put it >= max_vaddr + 1MB, and at least 16MB total space

    u32 stack_top = 0;
    if (stack_top_hint != 0) 
    {
        stack_top = align_down(stack_top_hint, 16u);
    } 
    else 
    {
        const u32 min_stack_top = 16u * 1024u * 1024u; // 16MB default ceiling minimum
        const u32 above = align_up(res.max_vaddr + 1u * 1024u * 1024u, 16u);

        stack_top = std::max(min_stack_top, above);
    }

    // Build argv for stack. If caller passed no args, emulate typical argv[0]
    std::vector<std::string> argv_vec = args;
    if (argv_vec.empty())
        argv_vec.push_back(elf_path);

    res.sp = setupStack(cpu, stack_top, argv_vec);

    cpu.state.pc      = res.entry;
    cpu.state.regs[2] = res.sp;      // sp = x2

    return res;
}

} // namespace rv32i

