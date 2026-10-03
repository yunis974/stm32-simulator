#include "cpu.hpp"

CPU::CPU(Memory* mem)
{
    memory = mem;
    instructions = {
        {0xF800, 0x2000, &CPU::executeMovs}, // MOVS instruction encoding
        {0xFE00, 0x1800, &CPU::executeAdd},   // ADD instruction encoding
    };

}

std::uint32_t CPU::getPC()
{
    return registers[15]; //register 15 is the program counter (PC)
}

void CPU::setPC(std::uint32_t value)
{
    registers[15] = value; //register 15 is the program counter (PC)
}

std::uint16_t CPU::fetch()
{
    uint16_t instruction = memory->read16(getPC()); // Fetch 16 bits from memory at the current PC
    setPC(getPC() + 2); // Increment PC by 2 to point to the next instruction
    return instruction;
}



bool CPU::is32BitInstruction(std::uint16_t instruction)
{
    uint8_t prefix = (instruction & 0xF800) >> 11; // Extract the prefix bits

    if (prefix == 0b11101 || prefix == 0b11110 || prefix == 0b11111)
    {
        return true; // It's a 32-bit instruction
    }
    else
    {
        return false; // It's a 16-bit instruction
    }
}


void CPU::decodeInstruction(std::uint16_t instruction)
{
    if(is32BitInstruction(instruction))
    {

    }
    else
    {
        decodeInstruction16(instruction);
    }
}

void CPU::decodeInstruction16(std::uint16_t instruction)
{
    for (const InstructionEncoding& encoding : instructions)
    {
        if ((instruction & encoding.mask) == encoding.value)
        {
            (this->*encoding.handler)(instruction); // Call the handler function for the matched instruction
            return;
        }
    }
}

void CPU::decodeInstruction32(std::uint32_t instruction)
{
    // Implement decoding logic for 32-bit instructions
}

void CPU::executeMovs(std::uint16_t instruction)
{
    uint8_t rd = (instruction & 0x0700) >> 8; // Extract destination register (Rd)
    uint8_t imm8 = instruction & 0x00FF; // Extract immediate value (imm8)

    registers[rd] = imm8; // Move the immediate value into the destination register
}

std::uint32_t CPU::getRegisterValue(std::uint8_t index)
{
    if (index < 16)
    {
        return registers[index];
    }
    else
    {
        std::cerr << "Invalid register index: " << static_cast<int>(index) << std::endl;
        return 0; // Return 0 for invalid index
    }
}

void CPU::executeAdd(std::uint16_t instruction)
{
    uint8_t rm = (instruction & 0x01C0) >> 6; // Extract source register (Rm)
    uint8_t rn = (instruction & 0x0038) >> 3; // Extract first operand register (Rn)
    uint8_t rd = instruction & 0x0007; // Extract destination register (Rd)

    registers[rd] = registers[rn] + registers[rm]; // Perform addition and store the result in Rd
}