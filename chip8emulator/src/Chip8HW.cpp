#include "Chip8HW.h"

//MEMORY
Chip8_Memory::Chip8_Memory()
{
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
        //Starting to load the font data from memory location 0x050
        //is a common choice for Chip8 emulators. It leaves room
        //for other interpreter variables.
        uint8_t offset = (sizeof(uint8_t) * 5 * i) + 0x050;
        memcpy(memory + offset, hexDigits[i], sizeof(uint8_t) * 5);
    }
    //Give the remaining bytes the 0xAA value in memory as a default for helping with debugging.
    //0xAA most likely means uninitialized memory in this case
    for (int i = 0x200; i < this->MEMORY_SIZE_IN_BYTES; i++)
    {
        memory[i] = 0xFF;
    }
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


//REGISTERS
uint8_t Chip8_Registers::GetGPRegisterValue(Chip8_Registers::RegisterID id)
{
    return registers[(int)id];
}

void Chip8_Registers::SetGPRegisterValue(RegisterID id, uint8_t value)
{
    registers[(int)id] = value;
}

uint16_t Chip8_Registers::GetIRegisterValue()
{
    return I;
}

void Chip8_Registers::SetIRegisterValue(uint16_t value)
{
    I = value;
}

uint8_t Chip8_Registers::GetTimerValue(TimerRegisterType type)
{
    switch (type)
    {
    case TimerRegisterType::DelayTimer:
        return delayTimer;
        break;
    case TimerRegisterType::SoundTimer:
        return soundTimer;
        break;
    }
    return 0;
}

void Chip8_Registers::SetTimerValue(TimerRegisterType type, uint8_t value)
{
    switch (type)
    {
    case TimerRegisterType::DelayTimer:
        delayTimer = value;
        break;
    case TimerRegisterType::SoundTimer:
        soundTimer = value;
        break;
    }
    //TODO: When the timer registers are non-zero, they should be decremented at a rate of 60Hz.
}

//KEYBOARD
Chip8_Keyboard::Chip8_Keyboard(const olc::PixelGameEngine& engineInstance) :
    pgeInstance(engineInstance)
{

}

olc::HWButton Chip8_Keyboard::GetKeyInfo(Chip8Key key)
{
    return pgeInstance.GetKey(keymap[key]);
}
