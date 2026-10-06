# STM32F103CBT6 Simulator

A work-in-progress software simulator for the **STM32F103CBT6 development board**, designed to emulate a minimal **ARM Cortex-M3** environment and eventually reproduce the behavior of the development board used in my electrical engineering studies.

The goal is to be able to compile firmware using the standard ARM embedded toolchain and run it inside the simulator without requiring the physical development board.

## 🎯 Project Goal

This project aims to progressively build a software representation of the STM32F103CBT6, starting from the CPU core and memory system and gradually implementing its peripherals.

The long-term goal is to be able to take firmware compiled with:

```text
arm-none-eabi-gcc
```

and execute the resulting ARM/Thumb machine code inside the simulator.

Eventually, the simulator should provide a graphical representation of the development board, allowing virtual peripherals and components to be connected to the board's GPIO pins.

For example:

```text
┌─────────────────────────────┐
│      STM32F103CBT6          │
│                             │
│  GPIO ──── LED              │
│  GPIO ──── Button           │
│  GPIO ──── Sensor           │
│                             │
│  LCD Display                │
│                             │
│  Cortex-M3 CPU              │
└─────────────────────────────┘
```

The idea is to eventually make it possible to develop and test embedded firmware without always having the physical development board available.

## 🧠 Current State

The project is currently in its early stages.

The simulator currently contains:

* A basic memory implementation
* A basic Cortex-M3 CPU structure
* Program counter and instruction fetching
* Basic register manipulation
* A simulated 32-bit APSR
* ARM condition flags (`N`, `Z`, `C`, `V`)
* 16-bit Thumb instruction decoding
* A table-based instruction decoder
* Pointer-to-member-function instruction dispatch
* `MOVS` instruction
* `ADDS` instruction with condition flag updates
* `SUBS` instruction with condition flag updates

The implemented arithmetic instructions currently support the ARM condition flags required for their respective operations.

For example, the simulator can execute a sequence equivalent to:

```asm
MOVS R3, #1
MOVS R4, #1
ADDS R5, R3, R4
```

and produce:

```text
R3 = 1
R4 = 1
R5 = 2

N = 0
Z = 0
C = 0
V = 0
```

`MOVS` correctly updates `N` and `Z` while preserving `C` and `V`.

`ADDS` and `SUBS` calculate `N`, `Z`, `C`, and `V` according to the result of the arithmetic operation.

This is only the beginning of the Cortex-M3 instruction set.

## 🚩 CPU Status Flags

The simulator currently implements the four main condition flags of the ARM APSR:

* `N` — Negative
* `Z` — Zero
* `C` — Carry
* `V` — Overflow

The flags are stored in a simulated 32-bit APSR using their ARM-defined bit positions:

```text
31    30    29    28
 N     Z     C     V
```

### MOVS

`MOVS` updates:

* `N`
* `Z`

while preserving:

* `C`
* `V`

For example:

```text
MOVS R0, #0

N = 0
Z = 1
C = unchanged
V = unchanged
```

### ADDS

`ADDS` updates all four flags.

For example:

```text
0xFFFFFFFF + 1

→ Result: 0x00000000
→ N = 0
→ Z = 1
→ C = 1
→ V = 0
```

and:

```text
0x7FFFFFFF + 1

→ Result: 0x80000000
→ N = 1
→ Z = 0
→ C = 0
→ V = 1
```

### SUBS

`SUBS` also updates all four flags.

For example:

```text
5 - 3

→ Result: 0x00000002
→ N = 0
→ Z = 0
→ C = 1
→ V = 0
```

When subtracting, the `C` flag indicates that no borrow was required.

For example:

```text
3 - 5

→ Result: 0xFFFFFFFE
→ N = 1
→ Z = 0
→ C = 0
→ V = 0
```

The simulator also correctly handles signed overflow:

```text
0x80000000 - 1

→ Result: 0x7FFFFFFF
→ N = 0
→ Z = 0
→ C = 1
→ V = 1
```

These flags provide the foundation for future instructions such as comparisons and conditional branches.

## 🏗️ Planned Architecture

The simulator will progressively be built around several layers:

```text
                    Firmware

                       │

                       ▼

              ARM/Thumb machine code

                       │

                       ▼

              ┌─────────────────┐
              │   Cortex-M3     │
              │      CPU        │
              └────────┬────────┘
                       │
                       ▼
              ┌─────────────────┐
              │     Memory      │
              └────────┬────────┘
                       │
              ┌────────┴────────┐
              ▼                 ▼
           GPIO/RCC          Timers
              │                 │
              ├───────┬─────────┤
              ▼       ▼         ▼
             LED    Button     LCD
```

The CPU and memory system will be implemented first, followed by the peripherals required to reproduce the development board's behavior.

## 🔌 Planned Peripherals

As the project evolves, the simulator will progressively implement STM32 peripherals such as:

* GPIO
* RCC
* SysTick
* Timers
* Interrupts / NVIC
* USART
* SPI
* I²C
* ADC
* Other STM32 peripherals as needed

The exact implementation will depend on the requirements of the development board and the firmware being tested.

## 🖥️ Graphical Interface

A major long-term goal is to create a graphical interface representing the development board.

The interface could allow virtual components to be attached to GPIO pins, such as:

* LEDs
* Push buttons
* Sensors
* Displays
* Other virtual electronic components

The board's LCD will also eventually be represented inside the simulator, allowing firmware that writes to the LCD to be visualized directly.

The goal is to make the simulator feel more like interacting with the real development board rather than simply running instructions in a terminal.

## 🛠️ Technologies

The project is currently written in:

* C++
* CMake
* ARM GNU Toolchain (`arm-none-eabi-gcc`)
* ARM Cortex-M3 / Thumb instruction set

The simulator itself does not rely on STM32 HAL or CubeMX. The intention is to reproduce the hardware behavior at a lower level, making the project useful for understanding how the processor and peripherals actually work.

## 🚧 Roadmap

### CPU

* [x] Program counter
* [x] Instruction fetching
* [x] Basic register system
* [x] Simulated APSR
* [x] `N`, `Z`, `C`, `V` condition flags
* [x] 16-bit Thumb instruction decoding
* [x] Table-based instruction decoder
* [x] `MOVS`
* [x] `MOVS` N/Z flag updates
* [x] `ADDS`
* [x] `ADDS` N/Z/C/V flag updates
* [x] `SUBS`
* [x] `SUBS` N/Z/C/V flag updates
* [ ] `CMP`
* [ ] More Thumb instructions
* [ ] Thumb-2 32-bit instructions
* [ ] Branch instructions
* [ ] Stack operations
* [ ] Exceptions
* [ ] Interrupt handling

### Memory

* [x] Byte-addressable memory
* [x] 8-bit reads/writes
* [x] 16-bit reads/writes
* [x] 32-bit reads/writes
* [ ] STM32 memory map
* [ ] Flash memory
* [ ] SRAM
* [ ] Memory-mapped peripherals

### STM32 Peripherals

* [ ] RCC
* [ ] GPIO
* [ ] SysTick
* [ ] TIM2
* [ ] NVIC
* [ ] USART
* [ ] SPI
* [ ] I²C
* [ ] ADC
* [ ] LCD controller

### Graphical Interface

* [ ] Virtual development board
* [ ] GPIO visualization
* [ ] Virtual LEDs
* [ ] Virtual buttons
* [ ] LCD display
* [ ] Virtual sensors/components
* [ ] Interactive peripheral configuration

## 📚 Why This Project?

This project is primarily a learning project.

Instead of treating the STM32 as a black box, the goal is to understand what happens between compiled C code and the physical hardware:

```text
C / C++ firmware

       ↓

arm-none-eabi-gcc

       ↓

ARM/Thumb machine code

       ↓

Cortex-M3 CPU

       ↓

Memory + Peripherals

       ↓

Physical hardware
```

The simulator attempts to reproduce that process in software, one component at a time.

It is also intended to eventually become a useful tool for experimenting with STM32 firmware without requiring access to the physical development board.

## ⚠️ Project Status

This project is **experimental and heavily work in progress**.

It currently implements only a very small subset of the Cortex-M3 instruction set and does not yet emulate the STM32F103CBT6 hardware or its peripherals completely.

The architecture will likely evolve significantly as more of the processor and STM32 hardware are implemented.

## 🚀 Installation & Usage

### Requirements

You will need:

* A C++20 compiler
* CMake 3.20 or newer
* Git

### Clone the repository

```bash
git clone https://github.com/yunis974/stm32-simulator.git
cd stm32-simulator
```

### Build

The project uses CMake with an out-of-source build directory:

```bash
cmake -S . -B build
cmake --build build
```

This will generate the simulator executable in:

```text
build/stm32sim
```

Run it with:

```bash
./build/stm32sim
```

### Testing the CPU

The current `test/main.cpp` contains small programs that directly exercise the CPU instruction handlers.

For example:

```asm
MOVS R3, #1
MOVS R4, #1
ADDS R5, R3, R4
```

The instructions can be written directly into simulated memory:

```cpp
memory.write16(0x0000, 0x2301);
memory.write16(0x0002, 0x2401);
memory.write16(0x0004, 0x191D);
```

The CPU then fetches and executes them sequentially:

```cpp
CPU cpu(&memory);

cpu.setPC(0x0000);

cpu.decodeInstruction(cpu.fetch());
cpu.decodeInstruction(cpu.fetch());
cpu.decodeInstruction(cpu.fetch());
```

The expected result is:

```text
R5 = 2
```

The arithmetic instructions also update the APSR flags.

The current execution pipeline is:

```text
Memory
   ↓
Fetch
   ↓
Instruction Decode
   ↓
Instruction Handler
   ↓
CPU Registers
   ↓
APSR Flags
```

As more instructions are implemented, the test programs will progressively be replaced by actual ARM/Thumb machine code generated from compiled firmware.

## 📄 License

This project is licensed under the **MIT License**.

You are free to use, modify, copy, and distribute this software, including for commercial purposes, provided that the original copyright notice and license are included with the software.

See the [`LICENSE`](LICENSE) file for the full license text.
