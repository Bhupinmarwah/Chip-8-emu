#include "Chip8.hpp"
#include <fstream>
#include <cstring>
#include <chrono>

const unsigned int START_ADDRESS = 0x200;

// Memory address where the fonts are loaded.
const unsigned int FONT_START_ADDRESS = 0X50;

//the size of the fonts will be 16 * 5. Each font has 5 bytes, and we have 16 keys therefore 80 bytes
const unsigned int FONTSET_SIZE = 80;

uint8_t fonset[FONTSET_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

//Constructor
Chip8::Chip8()
    : randGen(std::chrono::system_clock::now().time_since_epoch().count())
{
    randByte = std::uniform_int_distribution<uint8_t>(0, 255U);
    //Set this to start address, since we want to start reading instructions from here
    pc = START_ADDRESS;

}

void Chip8::LoadRom(char const* filename)
{
    std::ifstream rom(filename, std::ios::binary | std::ios::ate);

    if (rom.is_open()) {
        std::streampos size = rom.tellg();
        char* buffer = new char[size];

        //go back to the beginning of the file
        rom.seekg(0, std::ios::beg);
        //read the data into the buffer
        rom.read(buffer, size);

        for (unsigned int i = 0; i < size; ++i)
        {
            memory[START_ADDRESS + i] = buffer[i];
        }

        //free the memory
        delete[] buffer;
    }
}

void Chip8::OP_00E0()
{
    //We want to clear the video buffer
    memset(video, 0, sizeof(video));
}

void Chip8::OP_00EE()
{
    //We want to remove the top of the stack since we are returning from it
    --stackPointer;

    //set the program counter to the value in the stack that matches the stack pointer
    pc = stack[stackPointer];
}

void Chip8::OP_1nnn()
{
    // Use a and operator between the opcode and 0000_1111_1111_1111.
    //We use 0x0FFF because that will remove the first 4 values
    uint16_t nnn = opcode & 0x0FFFu;

    pc = nnn;
}

void Chip8::OP_2nnn()
{
    // Use a and operator between the opcode and 0000_1111_1111_1111.
    //We use 0x0FFF because that will remove the first 4 values
    uint16_t nnn = opcode & 0x0FFFu;

    //We want to remember the origin
    stack[stackPointer] = pc;
    ++stackPointer;

    pc = nnn;
}

void Chip8::OP_3xkk()
{
    // Use a and operator between the opcode and 0000_0000_0000_1111.
    // Use the right shift operator to grab the last 4
    //We use 0x0FFF because that will remove the first 4 values
    uint16_t Vx = (opcode >> 8) & 0x000F;
    uint16_t kk = opcode & 0x00FF;

    if (registers[Vx] == kk) {
        pc +=2;
    }
}

void Chip8::OP_4xkk()
{
    // Use a and operator between the opcode and 0000_0000_0000_1111.
    // Use the right shift operator to grab the last 4
    //We use 0x0FFF because that will remove the first 4 values
    uint16_t Vx = (opcode >> 8) & 0x000F;
    uint16_t kk = opcode & 0x00FF;

    if (registers[Vx] != kk) {
        pc +=2;
    }
}

void Chip8::OP_5xy0()
{
    // Use a and operator between the opcode and 0000_0000_0000_1111.
    // Use the right shift operator to grab the last 4
    //We use 0x0FFF because that will remove the first 4 values
    uint16_t Vx = (opcode >> 8) & 0x000F;
    uint16_t Vy = (opcode >> 4) & 0x000F;

    if (registers[Vx] == registers[Vy]) {
        pc +=2;
    }
}

void Chip8::OP_6xkk()
{
    // Use a and operator between the opcode and 0000_0000_0000_1111.
    // Use the right shift operator to grab the last 4
    //We use 0x0FFF because that will remove the first 4 values
    uint16_t Vx = (opcode >> 8) & 0x000F;
    uint16_t kk = opcode & 0x00FF;

    registers[Vx] = kk;
}

void Chip8::OP_7xkk()
{
    // Use a and operator between the opcode and 0000_0000_0000_1111.
    // Use the right shift operator to grab the last 4
    //We use 0x0FFF because that will remove the first 4 values
    uint16_t Vx = (opcode >> 8) & 0x000F;
    uint16_t kk = opcode & 0x00FF;

    registers[Vx] += kk;
}

void Chip8::OP_8xy0()
{
    // Use a and operator between the opcode and 0000_0000_0000_1111.
    // Use the right shift operator to grab the last 4
    //We use 0x0FFF because that will remove the first 4 values
    uint16_t Vx = (opcode >> 8) & 0x000F;
    uint16_t Vy = (opcode >> 4) & 0x000F;

    registers[Vx] = registers[Vy];
}

void Chip8::OP_8xy1()
{
    // Use a and operator between the opcode and 0000_0000_0000_1111.
    // Use the right shift operator to grab the last 4
    //We use 0x0FFF because that will remove the first 4 values
    uint16_t Vx = (opcode >> 8) & 0x000F;
    uint16_t Vy = (opcode >> 4) & 0x000F;

    registers[Vx] |= registers[Vy];
}

void Chip8::OP_8xy2()
{
    // Use a and operator between the opcode and 0000_0000_0000_1111.
    // Use the right shift operator to grab the last 4
    //We use 0x0FFF because that will remove the first 4 values
    uint16_t Vx = (opcode >> 8) & 0x000F;
    uint16_t Vy = (opcode >> 4) & 0x000F;

    registers[Vx] &= registers[Vy];
}

void Chip8::OP_8xy3()
{
    // Use a and operator between the opcode and 0000_0000_0000_1111.
    // Use the right shift operator to grab the last 4
    //We use 0x0FFF because that will remove the first 4 values
    uint16_t Vx = (opcode >> 8) & 0x000F;
    uint16_t Vy = (opcode >> 4) & 0x000F;

    registers[Vx] ^= registers[Vy];
}