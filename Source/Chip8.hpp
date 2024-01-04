#pragma once
#include <stdint.h>

class Chip8
{
private:
    /* data */
public:
    uint8_t registers[16]{};//16 8 bit registers
    uint8_t memory[4096]{}; //4k bytes of memory
    uint16_t index{};  //16 bit index register that stores memory addresses
    uint16_t pc{};//16 bit program counter. tracks which instruction to run next
    uint16_t stack[16]{};    //16 level stack
    uint8_t stackPointer{};//8 bit stack pointer that tracks the stack
    uint8_t delayTimer{};//8 bit delay timer
    uint8_t soundTimer{};// 8 bit sound timer
    uint8_t keypad[16]{};//16 input keys
    uint32_t video[64*32]{};//monochrome display memory
    uint16_t opcode;

    Chip8();
    ~Chip8();

    void LoadRom(char const* filename);
};

Chip8::Chip8(/* args */)
{
}

Chip8::~Chip8()
{
}
