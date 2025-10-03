#pragma once

#include <array>
#include <cstdint>
#include <cstddef>
#include <cassert>

class RegisterFile
{
    std::array<uint32_t, 32> regs_;

  public:

    uint32_t& operator[](size_t idx)
    {
        assert(idx < regs_.size());

        if (idx == 0)
        {
            static uint32_t zero = 0;
            zero = 0;

            return zero;
        }

        return regs_[idx];
    }
};
