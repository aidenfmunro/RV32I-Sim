#include "memory.hh"
#include <cassert>

Memory::Memory(size_t size) : data_(size) {}

uint8_t Memory::read8(uint32_t addr) const
{
    assert(addr < data_.size());

    return data_[addr];
}

uint16_t Memory::read16(uint32_t addr) const
{
    return (static_cast<uint16_t>(read8(addr + 0)) << 0)
        |  (static_cast<uint16_t>(read8(addr + 1)) << 8);
}

uint32_t Memory::read32(uint32_t addr) const
{
    return (static_cast<uint32_t>(read8(addr + 0)) << 0)
        |  (static_cast<uint32_t>(read8(addr + 1)) << 8)
        |  (static_cast<uint32_t>(read8(addr + 2)) << 16)
        |  (static_cast<uint32_t>(read8(addr + 3)) << 24);
}

void Memory::write8(uint32_t addr, uint8_t val)
{
    assert(addr < data_.size());

    data_[addr] = val;
}

void Memory::write16(uint32_t addr, uint16_t val)
{
    write8(addr    , static_cast<uint8_t>((val >> 0) & 0xFF));
    write8(addr + 1, static_cast<uint8_t>((val >> 8) & 0xFF));
}

void Memory::write32(uint32_t addr, uint32_t val)
{
    write8(addr    , static_cast<uint8_t>((val >> 0)  & 0xFF));
    write8(addr + 1, static_cast<uint8_t>((val >> 8)  & 0xFF));
    write8(addr + 2, static_cast<uint8_t>((val >> 16) & 0xFF));
    write8(addr + 3, static_cast<uint8_t>((val >> 24) & 0xFF));
}
