#pragma once
#include <map>
#include <stdint.h>
#include "olcPixelGameEngine.h"

//MEMORY
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
    RWState GetMemory(uint16_t address, uint8_t& value);
    RWState SetMemory(uint16_t address, uint8_t value);
    bool LoadROM(const std::string& filepath);
private:
    uint8_t memory[MEMORY_SIZE_IN_BYTES] = { 0 };
};


//REGISTERS
class Chip8_Registers
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

    uint8_t GetGPRegisterValue(Chip8_Registers::RegisterID id);
    void SetGPRegisterValue(Chip8_Registers::RegisterID id, uint8_t value);
    uint16_t GetIRegisterValue();
    void SetIRegisterValue(uint16_t value);
    uint8_t GetTimerValue(TimerRegisterType type);
    void SetTimerValue(TimerRegisterType type, uint8_t value);
private:
    //Registers accessible to programs
    uint8_t registers[(int)RegisterID::COUNT]; //V0-VF, VF shouldn't be used by any program
    uint16_t I; //Used to store memory address
    uint8_t delayTimer;
    uint8_t soundTimer;

    //Registers inaccessible to programs
    uint16_t pc; //Program counter
    uint8_t sp; //Stack pointer, points to the top of the stack
    uint16_t stack[16];
};


//KEYBOARD
class Chip8_Keyboard
{
public:
    enum class Chip8Key
    {
        KEY_0 = 0,
        KEY_1,
        KEY_2,
        KEY_3,
        KEY_4,
        KEY_5,
        KEY_6,
        KEY_7,
        KEY_8,
        KEY_9,
        KEY_A,
        KEY_B,
        KEY_C,
        KEY_D,
        KEY_E,
        KEY_F,
    };
    Chip8_Keyboard(const olc::PixelGameEngine& engineInstance);
    olc::HWButton GetKeyInfo(Chip8Key key);
private:
    std::map<Chip8Key, olc::Key> keymap =
    {
        { { Chip8Key::KEY_A }, { olc::Key::A   } },
        { { Chip8Key::KEY_B }, { olc::Key::B   } },
        { { Chip8Key::KEY_C }, { olc::Key::C   } },
        { { Chip8Key::KEY_D }, { olc::Key::D   } },
        { { Chip8Key::KEY_E }, { olc::Key::E   } },
        { { Chip8Key::KEY_F }, { olc::Key::F   } },
        { { Chip8Key::KEY_0 }, { olc::Key::NP0 } },
        { { Chip8Key::KEY_1 }, { olc::Key::NP1 } },
        { { Chip8Key::KEY_2 }, { olc::Key::NP2 } },
        { { Chip8Key::KEY_3 }, { olc::Key::NP3 } },
        { { Chip8Key::KEY_4 }, { olc::Key::NP4 } },
        { { Chip8Key::KEY_5 }, { olc::Key::NP5 } },
        { { Chip8Key::KEY_6 }, { olc::Key::NP6 } },
        { { Chip8Key::KEY_7 }, { olc::Key::NP7 } },
        { { Chip8Key::KEY_8 }, { olc::Key::NP8 } },
        { { Chip8Key::KEY_9 }, { olc::Key::NP9 } },
    };
    const olc::PixelGameEngine& pgeInstance;
};


//SCREEN
class Chip8_Screen
{
public:
    //Apparently there are other resolutions but just support the og resolution right now.
    static constexpr int WIDTH  = 64;
    static constexpr int HEIGHT = 32;

private:
    uint8_t screen[WIDTH * HEIGHT];
    olc::PixelGameEngine& pgeInstance;

public:
    Chip8_Screen(olc::PixelGameEngine& instance);
    void DisplayScreen();
};