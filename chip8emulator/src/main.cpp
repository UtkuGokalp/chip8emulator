//The ordering of these defines and includes are important
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#define OLC_PGEX_SOUND
#include "olcPGEX_Sound.h"

//After here the usual rules apply, ordering isn't important
#include <Windows.h>
#include <winnt.h>
#include "Chip8HW.h"
#include <map>
#define _USE_MATH_DEFINES
#include <math.h>
#include <exception>
#include <cstdlib>

class Chip8Emulator : public olc::PixelGameEngine
{
private:
    Chip8_Keyboard keyboard;
    Chip8_Screen screen;
    Chip8_Memory ram;
    Chip8_CPU cpu;
    int targetFPS = 3000;
    float targetFrameTime = 1.0f / targetFPS;
    float currentFrameTime = 0.0f;

public:
    Chip8Emulator() :
        keyboard(Chip8_Keyboard(*this)),
        screen(Chip8_Screen(*this)),
        ram(Chip8_Memory()),
        cpu(Chip8_CPU(keyboard, ram, screen))
    {
        sAppName = "Chip-8 Emulator";
        olc::SOUND::InitialiseAudio();
    }

private:
    static float GenerateBuzzerSound(int channelCount, float timeSinceAppStart, float timeStep)
    {
        constexpr float FREQUENCY = 440.0f;
        constexpr float AMPLITUDE = 0.35f;
        return AMPLITUDE * sinf(2.0f * M_PI * FREQUENCY * timeSinceAppStart);
    }

    bool OnUserCreate() override
    {
        //TODO: Remove the exception throwing and log the error message once logging is implemented.
        bool result = ram.LoadROM("roms/demos/Zero Demo [zeroZshadow, 2007].ch8");
        if (!result)
        {
            throw std::exception("Failed to load ROM. Either the file doesn't exist or is too big.");
        }
        return result;
    }

    bool OnUserUpdate(float deltaTime) override
    {
        if (GetKey(olc::Key::ESCAPE).bPressed)
        {
            return false;
        }

        if (currentFrameTime >= targetFrameTime)
        {
            cpu.ExecuteNextInstruction();
            screen.DisplayScreen();

            //Handle timers
            HandleTimerDecrement(Chip8_CPU::TimerRegisterType::DelayTimer, deltaTime);
            HandleTimerDecrement(Chip8_CPU::TimerRegisterType::SoundTimer, deltaTime);

            currentFrameTime = 0.0f;

            return true;
        }
        else
        {
            currentFrameTime += deltaTime;
        }
    }

    bool OnUserDestroy() override
    {
        olc::SOUND::DestroyAudio();
        return true;
    }

    void HandleTimerDecrement(Chip8_CPU::TimerRegisterType type, float deltaTime)
    {
        //decrementTimes[0] is the delay timer.
        //decrementTimes[1] is the sound timer.
        //Which index means which register is defined by their order in the Chip8_Registers::TimerRegisterType enum.
        static float decrementTimes[2] = { Chip8_CPU::TIMER_DECREMENT_RATE, Chip8_CPU::TIMER_DECREMENT_RATE };
        static bool audioSynthFunctionSet = false;
        uint8_t registerValue = cpu.GetTimerValue(type);
        if (registerValue > 0)
        {
            if (decrementTimes[(int)type] <= 0.0f)
            {
                cpu.SetTimerValue(type, registerValue - 1);
                decrementTimes[(int)type] = Chip8_CPU::TIMER_DECREMENT_RATE;
            }
            else
            {
                decrementTimes[(int)type] -= deltaTime;
            }

            //Play the buzzer sound for the sound timer
            if (type == Chip8_CPU::TimerRegisterType::SoundTimer && !audioSynthFunctionSet)
            {
                olc::SOUND::SetUserSynthFunction(GenerateBuzzerSound);
                audioSynthFunctionSet = true;
            }
        }
        else
        {
            decrementTimes[(int)type] = Chip8_CPU::TIMER_DECREMENT_RATE;
            if (type == Chip8_CPU::TimerRegisterType::SoundTimer && audioSynthFunctionSet)
            {
                olc::SOUND::SetUserSynthFunction(nullptr);
                audioSynthFunctionSet = false;
            }
        }
    }
};

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    srand(time(NULL));
    Chip8Emulator emulator;
    if (emulator.Construct(Chip8_Screen::WIDTH, Chip8_Screen::HEIGHT, 10, 10))
    {
        emulator.Start();
    }
    return 0;
}