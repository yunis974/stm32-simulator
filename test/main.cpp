#include <iostream>
#include "cpu.hpp"
#include "memory.hpp"

int main()
{
    Memory memory;
    CPU cpu(&memory);

    // ============================================================
    // MOVS
    // ============================================================

    std::cout << "========== MOVS TEST ==========\n";

    // MOVS R0, #5
    cpu.setFlagC(true);
    cpu.setFlagV(true);

    cpu.executeMovs(0x2005);

    std::cout << "MOVS R0, #5\n";
    std::cout << "R0 = " << cpu.getRegisterValue(0) << '\n';
    std::cout << "N = " << cpu.getFlagN() << '\n';
    std::cout << "Z = " << cpu.getFlagZ() << '\n';
    std::cout << "C = " << cpu.getFlagC() << '\n';
    std::cout << "V = " << cpu.getFlagV() << '\n';

    // MOVS R1, #0
    cpu.executeMovs(0x2100);

    std::cout << "\nMOVS R1, #0\n";
    std::cout << "R1 = " << cpu.getRegisterValue(1) << '\n';
    std::cout << "N = " << cpu.getFlagN() << '\n';
    std::cout << "Z = " << cpu.getFlagZ() << '\n';
    std::cout << "C = " << cpu.getFlagC() << '\n';
    std::cout << "V = " << cpu.getFlagV() << '\n';


    // ============================================================
    // SUBS
    // ============================================================

    std::cout << "\n========== SUBS TEST ==========\n";

    // SUBS R0, R1, R2
    // Encoding: 0x1A00 | (Rm << 6) | (Rn << 3) | Rd
    //
    // R0 = R1 - R2

    // ------------------------------------------------------------
    // 5 - 3 = 2
    // ------------------------------------------------------------

    cpu.setRegisterValue(1, 5);
    cpu.setRegisterValue(2, 3);

    cpu.executeSub(0x1A00 | (2 << 6) | (1 << 3) | 0);

    std::cout << "\n5 - 3\n";
    std::cout << "R0 = " << cpu.getRegisterValue(0) << '\n';
    std::cout << "N = " << cpu.getFlagN() << '\n';
    std::cout << "Z = " << cpu.getFlagZ() << '\n';
    std::cout << "C = " << cpu.getFlagC() << '\n';
    std::cout << "V = " << cpu.getFlagV() << '\n';


    // ------------------------------------------------------------
    // 3 - 5 = -2
    // ------------------------------------------------------------

    cpu.setRegisterValue(1, 3);
    cpu.setRegisterValue(2, 5);

    cpu.executeSub(0x1A00 | (2 << 6) | (1 << 3) | 0);

    std::cout << "\n3 - 5\n";
    std::cout << "R0 = " << cpu.getRegisterValue(0) << '\n';
    std::cout << "N = " << cpu.getFlagN() << '\n';
    std::cout << "Z = " << cpu.getFlagZ() << '\n';
    std::cout << "C = " << cpu.getFlagC() << '\n';
    std::cout << "V = " << cpu.getFlagV() << '\n';


    // ------------------------------------------------------------
    // 5 - 5 = 0
    // ------------------------------------------------------------

    cpu.setRegisterValue(1, 5);
    cpu.setRegisterValue(2, 5);

    cpu.executeSub(0x1A00 | (2 << 6) | (1 << 3) | 0);

    std::cout << "\n5 - 5\n";
    std::cout << "R0 = " << cpu.getRegisterValue(0) << '\n';
    std::cout << "N = " << cpu.getFlagN() << '\n';
    std::cout << "Z = " << cpu.getFlagZ() << '\n';
    std::cout << "C = " << cpu.getFlagC() << '\n';
    std::cout << "V = " << cpu.getFlagV() << '\n';


    // ------------------------------------------------------------
    // 0 - 1 = 0xFFFFFFFF
    // ------------------------------------------------------------

    cpu.setRegisterValue(1, 0);
    cpu.setRegisterValue(2, 1);

    cpu.executeSub(0x1A00 | (2 << 6) | (1 << 3) | 0);

    std::cout << "\n0 - 1\n";
    std::cout << "R0 = 0x" << std::hex << cpu.getRegisterValue(0) << std::dec << '\n';
    std::cout << "N = " << cpu.getFlagN() << '\n';
    std::cout << "Z = " << cpu.getFlagZ() << '\n';
    std::cout << "C = " << cpu.getFlagC() << '\n';
    std::cout << "V = " << cpu.getFlagV() << '\n';


    // ------------------------------------------------------------
    // 0x80000000 - 1 = 0x7FFFFFFF
    // Overflow signé
    // ------------------------------------------------------------

    cpu.setRegisterValue(1, 0x80000000);
    cpu.setRegisterValue(2, 1);

    cpu.executeSub(0x1A00 | (2 << 6) | (1 << 3) | 0);

    std::cout << "\n0x80000000 - 1\n";
    std::cout << "R0 = 0x" << std::hex << cpu.getRegisterValue(0) << std::dec << '\n';
    std::cout << "N = " << cpu.getFlagN() << '\n';
    std::cout << "Z = " << cpu.getFlagZ() << '\n';
    std::cout << "C = " << cpu.getFlagC() << '\n';
    std::cout << "V = " << cpu.getFlagV() << '\n';


    // ------------------------------------------------------------
    // 0x7FFFFFFF - 0xFFFFFFFF = 0x80000000
    // Overflow signé
    // ------------------------------------------------------------

    cpu.setRegisterValue(1, 0x7FFFFFFF);
    cpu.setRegisterValue(2, 0xFFFFFFFF);

    cpu.executeSub(0x1A00 | (2 << 6) | (1 << 3) | 0);

    std::cout << "\n0x7FFFFFFF - 0xFFFFFFFF\n";
    std::cout << "R0 = 0x" << std::hex << cpu.getRegisterValue(0) << std::dec << '\n';
    std::cout << "N = " << cpu.getFlagN() << '\n';
    std::cout << "Z = " << cpu.getFlagZ() << '\n';
    std::cout << "C = " << cpu.getFlagC() << '\n';
    std::cout << "V = " << cpu.getFlagV() << '\n';

    return 0;
}