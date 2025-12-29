#pragma once

#include <cstdint>
#include <cstring>
#include <Logger.h>

class Chip8_Memory
{
    /*
    http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#0.1
    +---------------+= 0xFFF (4095) End of Chip-8 RAM
    |               |
    |               |
    |               |
    |               |
    |               |
    | 0x200 to 0xFFF|
    |     Chip-8    |
    | Program / Data|
    |     Space     |
    |               |
    |               |
    |               |
    +- - - - - - - -+= 0x600 (1536) Start of ETI 660 Chip-8 programs
    |               |
    |               |
    |               |
    +---------------+= 0x200 (512) Start of most Chip-8 programs
    | 0x000 to 0x1FF|
    | Reserved for  |
    |  interpreter  |
    +---------------+= 0x000 (0) Start of Chip-8 RAM
    */
public:
    enum class RWState
    {
        SUCCESS = 0,
        FAIL,
        WRITTEN_BEYOND_MEMORY_LIMIT,
        READ_BEYOND_MEMORY_LIMIT,
        COUNT,
    };
    static const int MEMORY_SIZE_IN_BYTES = 4096;
    Chip8_Memory();
    void ResetMemory();
    RWState GetMemory(uint16_t address, uint8_t& value);
    RWState SetMemory(uint16_t address, uint8_t value);
    bool LoadROM(const std::string& filepath);
    // Returns 0xFFFF if digit isn't a value digit
    uint16_t GetMemoryLocationForHexDigitFont(uint8_t digit);
private:
    //Starting to load the font data from memory location 0x050
    //is a common choice for Chip8 emulators. It leaves room
    //for other interpreter variables.
    static constexpr uint8_t FONT_DATA_START_OFFSET = 0x050;
    uint8_t memory[MEMORY_SIZE_IN_BYTES] = { 0 };
};