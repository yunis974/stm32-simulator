#include "memory.hpp"

Memory::Memory() {

}

std::uint8_t Memory::read8(std::uint32_t address)
{
    return memory[address];
}

std::uint16_t Memory::read16(std::uint32_t address)
{
    std::uint8_t byte1 = read8(address);
    std::uint8_t byte2 = read8(address + 1);
    
    std::uint16_t value = byte1;
    value |= (byte2 << 8);

    return value;
}

std::uint32_t Memory::read32(std::uint32_t address)
{
    std::uint8_t byte1 = read8(address);
    std::uint8_t byte2 = read8(address + 1);
    std::uint8_t byte3 = read8(address + 2);
    std::uint8_t byte4 = read8(address + 3);

    std::uint32_t value = byte1 | (byte2 << 8) | (byte3 << 16) | (byte4 << 24);

    return value;
}

bool Memory::write8(std::uint32_t address, std::uint8_t value)
{
    memory[address] = value;
    return true;
}

bool Memory::write16(std::uint32_t address, uint16_t value)
{

    for (int i = 0; i < 2; i++) 
    {
        std::uint8_t byte = (value >> (8 * i)) & 0xFF;
        write8(address + i, byte);
    }

    return true;
}

bool Memory::write32(std::uint32_t address, std::uint32_t value)
{
    for (int i = 0; i < 2; i++) 
    {
        std::uint16_t two_byte = (value >> (i * 16)) & 0xFFFF; // Extract 16 bits
        write16(address + i*2, two_byte);
    }

    return true;
}