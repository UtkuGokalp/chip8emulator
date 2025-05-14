#include "Chip8HW.h"

//MEMORY
Chip8_Memory::Chip8_Memory()
{
    //Give every byte the 0xAA value in memory as a default for helping with debugging.
    //0xAA most likely means uninitialized memory in this case
    for (int i = 0; i < this->MEMORY_SIZE_IN_BYTES; i++)
    {
        this->memory[i] = 0xAA;
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
