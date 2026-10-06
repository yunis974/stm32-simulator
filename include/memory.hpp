#pragma once

#include <iostream>
#include <cstdint>

class Memory
{
public:
    Memory();
    std::uint8_t read8(std::uint32_t address);
    std::uint16_t read16(std::uint32_t address);
    std::uint32_t read32(std::uint32_t address);

    bool write8(std::uint32_t address, std::uint8_t value);
    bool write16(std::uint32_t address, std::uint16_t value);
    bool write32(std::uint32_t address, std::uint32_t value);
private:
    std::uint8_t memory[65536] = {0}; // 64KB of memory filled with zeros
};