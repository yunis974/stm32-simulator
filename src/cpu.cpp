#include "cpu.hpp"

CPU::CPU(Memory* mem)
{
    memory = mem;
    instructions = {
        {0xF800, 0x2000, &CPU::executeMovs}, // MOVS instruction encoding
        {0xFE00, 0x1800, &CPU::executeAdd},   // ADD instruction encoding
        {0xFE00, 0x1A00, &CPU::executeSub}    // SUB instruction encoding
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
    std::uint16_t instruction = memory->read16(getPC()); // Fetch 16 bits from memory at the current PC
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
    std::uint8_t rd = (instruction & 0x0700) >> 8; // Extract destination register (Rd)
    std::uint8_t imm8 = instruction & 0x00FF; // Extract immediate value (imm8)

    registers[rd] = imm8; // Move the immediate value into the destination register
    std::uint32_t result = imm8;

    setFlagN(result & (1u << 31));
    setFlagZ(result == 0);
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
    std::uint8_t rm = (instruction & 0x01C0) >> 6; // Extract source register (Rm)
    std::uint8_t rn = (instruction & 0x0038) >> 3; // Extract first operand register (Rn)
    std::uint8_t rd = instruction & 0x0007; // Extract destination register (Rd)


    std::uint64_t fullResult = static_cast<std::uint64_t>(registers[rn]) + static_cast<std::uint64_t>(registers[rm]);
    std::uint32_t result = fullResult;

    registers[rd] = result; // Store the result in the destination register

    setFlagN(result & (1u << 31)); // Set N flag if the result is negative
    setFlagZ(result == 0); // Set Z flag if the result is zero
    setFlagC(fullResult >> 32); // Set C flag if there was a carry out

    bool signRn = registers[rn] & (1u << 31); 
    bool signRm = registers[rm] & (1u << 31);
    bool signResult = result & (1u << 31);

    /* 
    note: The V flag is set if the signs of the operands are the same 
    and the sign of the result is different.
    This indicates that an overflow occurred during the addition.
    */
    setFlagV((signRn == signRm) && (signRn != signResult)); // Set V flag if there was an overflow


}

void CPU::executeSub(std::uint16_t instruction)
{
    std::uint8_t rm = (instruction & 0x01C0) >> 6; // Extract source register (Rm)
    std::uint8_t rn = (instruction & 0x0038) >> 3; // Extract first operand register (Rn)
    std::uint8_t rd = instruction & 0x0007; // Extract destination register (Rd)
    std::uint32_t result = registers[rn] - registers[rm]; // Perform subtraction
    registers[rd] = result; // Store the result in the destination register

    setFlagN(result & (1u << 31)); // Set N flag if the result is negative
    setFlagZ(result == 0); // Set Z flag if the result is zero
    setFlagC(registers[rn] >= registers[rm]); // Set C flag if there was no borrow (Rn >= Rm)

    bool signRn = registers[rn] & (1u << 31);
    bool signRm = registers[rm] & (1u << 31);
    bool signResult = result & (1u << 31);

    /* 
    note: The V flag is set if the signs of the operands are different 
    and the sign of the result is different from the sign of Rn.
    This indicates that an overflow occurred during the subtraction.
    */
    setFlagV((signRn != signRm) && (signRn != signResult)); // Set V flag if there was an overflow


}

bool CPU::getFlagN() const
{
    return (APSR & (1u << 31)); // Check the N flag (bit 31) in the APSR register
}

bool CPU::getFlagZ() const
{
    return (APSR & (1u << 30)); // Check the Z flag (bit 30) in the APSR register
}

bool CPU::getFlagC() const
{
    return (APSR & (1u << 29)); // Check the C flag (bit 29) in the APSR register
}

bool CPU::getFlagV() const 
{
    return (APSR & (1u  << 28)); // Check the V flag (bit 28) in the APSR register
}

void CPU::setFlagN(bool value)
{
    if (value)
    {
        APSR |= (1u << 31); // Set the N flag (bit 31) in the APSR register
    }
    else
    {
        APSR &= ~(1u << 31); // Clear the N flag (bit 31) in the APSR register
    }
}

void CPU::setFlagZ(bool value)
{
    if (value)
    {
        APSR |= (1u << 30); // Set the Z flag (bit 30) in the APSR register
    }
    else
    {
        APSR &= ~(1u << 30); // Clear the Z flag (bit 30) in the APSR register
    }
}

void CPU::setFlagC(bool value)
{
    if (value)
    {
        APSR |= (1u << 29); // Set the C flag (bit 29) in the APSR register
    }
    else
    {
        APSR &= ~(1u << 29); // Clear the C flag (bit 29) in the APSR register
    }
}

void CPU::setFlagV(bool value)
{
    if (value)
    {
        APSR |= (1u  << 28); // Set the V flag (bit 28) in the APSR register
    }
    else
    {
        APSR &= ~(1u  << 28); // Clear the V flag (bit 28) in the APSR register
    }
}

void CPU::setRegisterValue(std::uint8_t index, std::uint32_t value)
{
    if (index < 16)
    {
        registers[index] = value;
    }
    else
    {
        std::cerr << "Invalid register index: " << static_cast<int>(index) << std::endl;
    }
}