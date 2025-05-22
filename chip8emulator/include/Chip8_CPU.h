#pragma once
#pragma once

#include <Logger.h>
#include <stack>
#include <cstdlib>
#include <cstdint>
#include <exception>
#include <Chip8_Memory.h>
#include <Chip8_Screen.h>
#include <Chip8_Keyboard.h>
#include <array>

class Chip8_CPU
{
public:
    enum class RegisterID
    {
        V0 = 0,
        V1,
        V2,
        V3,
        V4,
        V5,
        V6,
        V7,
        V8,
        V9,
        VA,
        VB,
        VC,
        VD,
        VE,
        VF,
        COUNT
    };

    enum class TimerRegisterType
    {
        DelayTimer = 0,
        SoundTimer,
        COUNT
    };

    static constexpr float TIMER_DECREMENT_RATE = (1.0f / 60.0f);

    Chip8_CPU(Chip8_Keyboard& keyboard, Chip8_Memory& ram, Chip8_Screen& screen);
    uint8_t GetGPRegisterValue(Chip8_CPU::RegisterID id);
    void SetGPRegisterValue(Chip8_CPU::RegisterID id, uint8_t value);
    uint16_t GetIRegisterValue();
    void SetIRegisterValue(uint16_t value);
    uint8_t GetTimerValue(TimerRegisterType type);
    void SetTimerValue(TimerRegisterType type, uint8_t value);
    uint16_t GetProgramCounterValue();
    uint8_t GetStackPointerValue();
    const std::array<uint16_t, 16>& GetStack();
    bool ExecuteNextInstruction();
private:
    //Registers accessible to programs
    std::array<uint8_t, (int)RegisterID::COUNT> registers; //V0-VF, VF shouldn't be used by any program
    uint16_t Iregister; //Used to store memory address
    uint8_t delayTimer;
    uint8_t soundTimer;

    //Registers inaccessible to programs
    uint16_t pc; //Program counter
    uint8_t sp; //Stack pointer, points to the top of the stack
    std::array<uint16_t, 16> stack; //stack[0] is at the bottom of the stack. stack[15] is at the top.
    Chip8_Keyboard& keyboard;
    Chip8_Memory& ram;
    Chip8_Screen& screen;

    bool IncreaseStackPointer();
    bool DecreaseStackPointer();
};