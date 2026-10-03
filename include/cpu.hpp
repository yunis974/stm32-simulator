#pragma once

#include <iostream>
#include <cstdint>
#include <vector>
#include "memory.hpp"

class CPU;

struct InstructionEncoding
{
    std::uint16_t mask;
    std::uint16_t value;
    void (CPU::*handler)(std::uint16_t instruction);
};



class CPU
{
public:
    CPU(Memory* mem);
    std::uint32_t getPC();
    void setPC(std::uint32_t value);
    std::uint16_t fetch();

    void decodeInstruction(std::uint16_t instruction);
    bool is32BitInstruction(std::uint16_t instruction);

    void decodeInstruction16(std::uint16_t instruction);
    void decodeInstruction32(std::uint32_t instruction);
    
    std::uint32_t getRegisterValue(std::uint8_t index);
    // instruction functions
    void executeMovs(std::uint16_t instruction);
    void executeAdd(std::uint16_t instruction);
private:
    std::uint32_t registers[16]; // General-purpose registers
    Memory* memory; // Pointer to the memory object
    std::vector<InstructionEncoding> instructions; // Vector to hold instruction encodings
};

