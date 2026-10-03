#include "memory.hpp"
#include "cpu.hpp"
#include <bitset>

int main(void)
{
Memory memory;
memory.write16(0x0000, 0x2301);
memory.write16(0x0002, 0x2401);
memory.write16(0x0004, 0x191D);
CPU cpu(&memory);
cpu.setPC(0x0000);

cpu.decodeInstruction(cpu.fetch());
cpu.decodeInstruction(cpu.fetch());
cpu.decodeInstruction(cpu.fetch());

std::cout << "register 3: " << cpu.getRegisterValue(3) << '\n';
std::cout << "register 4: " << cpu.getRegisterValue(4) << '\n';
std::cout << "register 5: " << cpu.getRegisterValue(5) << '\n';
return 0;
}
