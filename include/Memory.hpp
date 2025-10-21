#pragma once
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <array>
#include <memory>
#include "IntTypes.hpp"

namespace rv32i {

class SparseMemory 
{
    static constexpr u32 PAGE_SIZE = 4096;

    struct Page 
    { 
        std::array<u8, PAGE_SIZE> data{}; 
    };

    std::unordered_map<u32, std::unique_ptr<Page>> pages_;

    Page* getPage(u32 page_index) 
    {
        auto& p = pages_[page_index];

        if (!p) 
            p = std::make_unique<Page>();

        return p.get();
    }

public:

    u8  LoadU8(u32 addr) const {
        u32 page = addr / PAGE_SIZE, off = addr % PAGE_SIZE;

        auto it = pages_.find(page);

        return (it == pages_.end()) ? 0 : it->second->data[off];
    }

    u16 LoadU16(u32 addr) const 
    { 
        return u16(LoadU8(addr) | (LoadU8(addr+1) << 8)); 
    }

    u32 LoadU32(u32 addr) const 
    {
        return (u32(LoadU8(addr + 0)) << 0)
             | (u32(LoadU8(addr + 1)) << 8)
             | (u32(LoadU8(addr + 2)) << 16)
             | (u32(LoadU8(addr + 3)) << 24);
    }

    void StoreU8(u32 addr, u8 val) 
    {
        u32 page   = addr / PAGE_SIZE; 
        u32 offset = addr % PAGE_SIZE;

        getPage(page)->data[offset] = val;
    }

    void StoreU16(u32 addr, u16 val)
    {
        StoreU8(addr + 0, (val >> 0) & 0xFF);
        StoreU8(addr + 1, (val >> 8) & 0xFF);
    }

    void StoreU32(u32 addr, u32 val) 
    {
        StoreU8(addr + 0, (val >> 0) & 0xFF);
        StoreU8(addr + 1, (val >> 8) & 0xFF);
        StoreU8(addr + 2, (val >> 16) & 0xFF);
        StoreU8(addr + 3, (val >> 24) & 0xFF);
    }

    void WriteBlock(u32 addr, const u8* src, u32 len) 
    {
        for (u32 i = 0; i < len; ++i)
            StoreU8(addr + i, src[i]);
    }

    size_t numPages() const { return pages_.size(); }

    void clear() 
    {
        pages_.clear();
    }

    void dump(u32 addr, u32 len = 16) const
    {
        std::cerr<< "Memory dump at 0x" << std::hex << addr << ":\n";

        for (u32 i = 0; i < len; ++i)
        {
            u8 byte = LoadU8(addr + i);
            if (i % 16 == 0)
                std::cerr << "\n0x" << std::setw(8) << std::setfill('0') << (addr + i) << ": ";
            std::cerr << std::setw(2) << std::setfill('0') << (int)byte << " ";
        }

        std::cerr << std::dec << "\n\n";
    }

};

} // namespace rv32i

