#include "cpu.hpp"
#include "memory.hpp"
#include <iostream>
#include <iomanip>

void printResult(CPU& cpu)
{
    std::cout << "R0 = 0x"
              << std::hex
              << std::setw(8)
              << std::setfill('0')
              << cpu.getRegisterValue(0)
              << std::dec
              << '\n';

    std::cout << "N = " << cpu.getFlagN() << '\n';
    std::cout << "Z = " << cpu.getFlagZ() << '\n';
    std::cout << "C = " << cpu.getFlagC() << '\n';
    std::cout << "V = " << cpu.getFlagV() << '\n';

    std::cout << "--------------------\n";
}

int main()
{
    Memory memory;
    CPU cpu(&memory);

    // ADD R0, R1, R2
    std::uint16_t instruction = 0x1888;

    // TEST 1
    std::cout << "===== TEST 1 : 5 + 3 =====\n";

    cpu.setRegisterValue(1, 5);
    cpu.setRegisterValue(2, 3);

    cpu.decodeInstruction(instruction);

    printResult(cpu);


    // TEST 2
    std::cout << "===== TEST 2 : 0 + 0 =====\n";

    cpu.setRegisterValue(1, 0);
    cpu.setRegisterValue(2, 0);

    cpu.decodeInstruction(instruction);

    printResult(cpu);


    // TEST 3
    std::cout << "===== TEST 3 : 0xFFFFFFFF + 1 =====\n";

    cpu.setRegisterValue(1, 0xFFFFFFFF);
    cpu.setRegisterValue(2, 1);

    cpu.decodeInstruction(instruction);

    printResult(cpu);


    // TEST 4
    std::cout << "===== TEST 4 : 0x7FFFFFFF + 1 =====\n";

    cpu.setRegisterValue(1, 0x7FFFFFFF);
    cpu.setRegisterValue(2, 1);

    cpu.decodeInstruction(instruction);

    printResult(cpu);


    // TEST 5
    std::cout << "===== TEST 5 : 0x80000000 + 0xFFFFFFFF =====\n";

    cpu.setRegisterValue(1, 0x80000000);
    cpu.setRegisterValue(2, 0xFFFFFFFF);

    cpu.decodeInstruction(instruction);

    printResult(cpu);

    return 0;
}