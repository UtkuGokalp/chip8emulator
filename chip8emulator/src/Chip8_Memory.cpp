#include "Chip8_Memory.h"

Chip8_Memory::Chip8_Memory()
{
    //Zero out all the memory to initialize it.
    memset(memory, 0x00, MEMORY_SIZE_IN_BYTES);

    //Load the hexadecimal digits into the memory, in locations 0x000 to 0x1FF
    uint8_t hexDigits[16][5] =
    {
        { 0xF0, 0x90, 0x90, 0x90, 0xF0 }, //hex 0
        { 0x20, 0x60, 0x20, 0x20, 0x70 }, //hex 1
        { 0xF0, 0x10, 0xF0, 0x80, 0xF0 }, //hex 2
        { 0xF0, 0x10, 0xF0, 0x10, 0xF0 }, //hex 3
        { 0x90, 0x90, 0xF0, 0x10, 0x10 }, //hex 4
        { 0xF0, 0x80, 0xF0, 0x10, 0xF0 }, //hex 5
        { 0xF0, 0x80, 0xF0, 0x90, 0xF0 }, //hex 6
        { 0xF0, 0x10, 0x20, 0x40, 0x40 }, //hex 7
        { 0xF0, 0x90, 0xF0, 0x90, 0xF0 }, //hex 8
        { 0xF0, 0x90, 0xF0, 0x10, 0xF0 }, //hex 9
        { 0xF0, 0x90, 0xF0, 0x90, 0x90 }, //hex A
        { 0xE0, 0x90, 0xE0, 0x90, 0xE0 }, //hex B
        { 0xF0, 0x80, 0x80, 0x80, 0xF0 }, //hex C
        { 0xE0, 0x90, 0x90, 0x90, 0xE0 }, //hex D
        { 0xF0, 0x80, 0xF0, 0x80, 0xF0 }, //hex E
        { 0xF0, 0x80, 0xF0, 0x80, 0x80 }, //hex F
    };
    for (int i = 0; i < 16; i++)
    {
        uint8_t offset = (sizeof(uint8_t) * 5 * i) + FONT_DATA_START_OFFSET;
        memcpy(memory + offset, hexDigits[i], sizeof(uint8_t) * 5);
    }
    //Give the remaining bytes the 0xAA value in memory as a default for helping with debugging.
    //0xAA most likely means uninitialized memory in this case
    for (int i = 0x200; i < this->MEMORY_SIZE_IN_BYTES; i++)
    {
        memory[i] = 0xFF;
    }
}

uint16_t Chip8_Memory::GetMemoryLocationForHexDigitFont(uint8_t digit)
{
    if (digit > 0xF)
    {
        return 0xFFFF;
    }
    return (sizeof(uint8_t) * 5 * digit) + FONT_DATA_START_OFFSET;
}

Chip8_Memory::RWState Chip8_Memory::GetMemory(uint16_t address, uint8_t& value)
{
    if (address >= MEMORY_SIZE_IN_BYTES)
    {
        return RWState::READ_BEYOND_MEMORY_LIMIT;
    }
    else
    {
        value = memory[address];
        return RWState::SUCCESS;
    }
}

Chip8_Memory::RWState Chip8_Memory::SetMemory(uint16_t address, uint8_t value)
{
    if (address >= MEMORY_SIZE_IN_BYTES)
    {
        return RWState::WRITTEN_BEYOND_MEMORY_LIMIT;
    }
    else
    {
        memory[address] = value;
    }
}

bool Chip8_Memory::LoadROM(const std::string& filepath)
{
    //TODO: Implement error logging
    //std::ios::ate immediately jumps to the end of the file upon opening
    //this way we don't have to do that explicitly below when computing
    //the size of the file
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        return false;
    }
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // ROMs must fit into available memory from 0x200 to end
    if (fileSize > (MEMORY_SIZE_IN_BYTES - 0x200))
    {
        return false;
    }

    file.read((char*)(memory + 0x200), fileSize);
    return true;
}