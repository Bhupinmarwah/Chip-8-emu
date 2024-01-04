#include "Chip8.hpp"
#include <fstream>

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
{
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
