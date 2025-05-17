#include "Chip8_CPU.h"

uint8_t Chip8_CPU::GetGPRegisterValue(Chip8_CPU::RegisterID id)
{
    return cpu[(int)id];
}

void Chip8_CPU::SetGPRegisterValue(RegisterID id, uint8_t value)
{
    cpu[(int)id] = value;
}

uint16_t Chip8_CPU::GetIRegisterValue()
{
    return Iregister;
}

void Chip8_CPU::SetIRegisterValue(uint16_t value)
{
    Iregister = value;
}

uint8_t Chip8_CPU::GetTimerValue(TimerRegisterType type)
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

void Chip8_CPU::SetTimerValue(TimerRegisterType type, uint8_t value)
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
}
