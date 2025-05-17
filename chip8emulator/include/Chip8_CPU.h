#pragma once
#pragma once


#include <stack>
#include <cstdlib>
#include <cstdint>
#include <exception>
#include <Chip8_Memory.h>
#include <Chip8_Screen.h>
#include <Chip8_Keyboard.h>

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

    static constexpr float TIMER_DECREMENT_RATE = 1.0f / 60.0f;

    Chip8_CPU(Chip8_Keyboard& keyboard, Chip8_Memory& ram, Chip8_Screen& screen);
    uint8_t GetGPRegisterValue(Chip8_CPU::RegisterID id);
    void SetGPRegisterValue(Chip8_CPU::RegisterID id, uint8_t value);
    uint16_t GetIRegisterValue();
    void SetIRegisterValue(uint16_t value);
    uint8_t GetTimerValue(TimerRegisterType type);
    void SetTimerValue(TimerRegisterType type, uint8_t value);
    void ExecuteNextInstruction();
private:
    //Registers accessible to programs
    uint8_t registers[(int)RegisterID::COUNT]; //V0-VF, VF shouldn't be used by any program
    uint16_t Iregister; //Used to store memory address
    uint8_t delayTimer;
    uint8_t soundTimer;

    //Registers inaccessible to programs
    uint16_t pc; //Program counter
    uint8_t sp; //Stack pointer, points to the top of the stack
    uint16_t stack[16]; //stack[0] is at the bottom of the stack. stack[15] is at the top.
    Chip8_Keyboard& keyboard;
    Chip8_Memory& ram;
    Chip8_Screen& screen;

    static constexpr int MIN_STACK_INDEX = 0;
    static constexpr int MAX_STACK_INDEX = 15;

    bool IncreaseStackPointer();
    bool DecreaseStackPointer();
};