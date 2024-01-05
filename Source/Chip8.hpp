#pragma once
#include <stdint.h>
#include <random>

class Chip8
{
private:
std::default_random_engine randGen;
std::uniform_int_distribution<uint8_t> randByte;

//INSTRUCTIONS or OPCODE

/*
00E0:CLS = This will clear the display
*/
void OP_00E0();

/*
00EE:RET = This will return from a subroutine.
*/
void OP_00EE();

/*
1nnn:JP = This will grab the last 12 of the binary from 16 bit and set that to the program counter.
*/
void OP_1nnn();

/*
2nnn:CALL = This will call the instruction at nnn.
*/
void OP_2nnn();

/*
3xkk:CALL = This will skip the next instruction if Vx = kk.
*/
void OP_3xkk();

/*
4xkk:CALL = This will skip the next instruction if Vx != kk.
*/
void OP_4xkk();

/*
5xy0:CALL = This will skip the next instruction if Vx = Vy.
*/
void OP_5xy0();

/*
6xkk:CALL = This will set Vx=kk.
*/
void OP_6xkk();

/*
7xkk:CALL = This will set add Vx = Vx + kk.
*/
void OP_7xkk();

/*
8xy0:CALL = This will set Vx = Vy.
*/
void OP_8xy0();

/*
8xy1:CALL = This will set Vx = Vx or Vy. This means it will grab the binary ones from either
*/
void OP_8xy1();

/*
8xy2:CALL = This will set Vx = Vx and Vy. This means it will grab the binary ones only if both have the one
*/
void OP_8xy2();

/*
8xy3:CALL = This will set Vx = Vx XOR Vy. This means it will grab the binary ones only if one is 1 and the other is 0
*/
void OP_8xy3();
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
