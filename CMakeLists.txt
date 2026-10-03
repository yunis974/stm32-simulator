cmake_minimum_required(VERSION 3.20)

project(STM32Sim
    VERSION 0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(stm32sim
    test/main.cpp
    src/cpu.cpp
    src/memory.cpp
)

target_include_directories(stm32sim
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)