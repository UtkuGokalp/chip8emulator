#include "Chip8_CPU.h"

Chip8_CPU::Chip8_CPU(Chip8_Keyboard& keyboard, Chip8_Memory& ram, Chip8_Screen& screen) :
    keyboard(keyboard),
    ram(ram),
    screen(screen)
{
    delayTimer = 0x00;
    soundTimer = 0x00;

    //Initial values of these registers might need to be changed later on.
    Iregister = 0x0000;
    sp = 0x00;
    pc = 0x0000;
    memset(stack, 0x0000, sizeof(uint16_t) * 16); //Stack size is hardcoded, which should be fine since stack size doesn't change in the hardware.
    memset(registers, 0x00, sizeof(uint8_t) * (int)RegisterID::COUNT);
}

uint8_t Chip8_CPU::GetGPRegisterValue(Chip8_CPU::RegisterID id)
{
    return registers[(int)id];
}

void Chip8_CPU::SetGPRegisterValue(RegisterID id, uint8_t value)
{
    registers[(int)id] = value;
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
