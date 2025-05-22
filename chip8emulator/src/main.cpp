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

#define CPU_REGISTERS_VIEW_WIDTH                  Chip8_Screen::WIDTH
#define CPU_REGISTERS_VIEW_HEIGHT                 24

#define DISASSEMBLY_VIEW_WIDTH             40
#define DISASSEMBLY_VIEW_HEIGHT            (Chip8_Screen::HEIGHT + MEMORY_VIEW_HEIGHT)


const olc::Pixel BACKGROUND_COLOR = olc::DARK_GREY;

class Chip8Emulator : public olc::PixelGameEngine
{
private:
    Chip8_Keyboard keyboard;
    Chip8_Screen screen;
    Chip8_Memory ram;
    Chip8_CPU cpu;
    const std::string& romPath;
    bool emulationRunning;

public:
    Chip8Emulator(const std::string& romPath) :
        keyboard(Chip8_Keyboard(*this)),
        screen(Chip8_Screen(*this)),
        ram(Chip8_Memory()),
        cpu(Chip8_CPU(keyboard, ram, screen)),
        romPath(romPath)
    {
        sAppName = "Chip-8 Emulator (PRESS SPACE TO START THE EMULATION)";
        emulationRunning = false;
        olc::SOUND::InitialiseAudio();
    }

private:
    static float GenerateBuzzerSound(int channelCount, float timeSinceAppStart, float timeStep)
    {
        constexpr float FREQUENCY = 440.0f;
        constexpr float AMPLITUDE = 0.35f;
        return AMPLITUDE * sinf(2.0f * (float)M_PI * FREQUENCY * timeSinceAppStart);
    }

    bool OnUserCreate() override
    {
        Clear(BACKGROUND_COLOR);
        if (ram.LoadROM(romPath) == false)
        {
            Logger::Log("Failed to load ROM. Either the file doesn't exist or is too big.", Logger::LogSeverity::LOGSEVERITY_ERROR);
            return false;
        }
        return true;
    }

    bool OnUserUpdate(float deltaTime) override
    {
        if (GetKey(olc::Key::ESCAPE).bPressed)
        {
            return false;
        }

        if (!emulationRunning && GetKey(olc::Key::SPACE).bPressed)
        {
            emulationRunning = true;
            sAppName = "Chip-8 Emulator";
            return true;
        }

        if (emulationRunning)
        {
            //Tick the CPU
            if (!cpu.ExecuteNextInstruction())
            {
                Logger::Log("Error during instruction execution! Closing the application.", Logger::LogSeverity::LOGSEVERITY_ERROR);
                return false;
            }

            //Update the screen
            screen.DisplayScreen();

            //Handle timers
            HandleTimerDecrement(Chip8_CPU::TimerRegisterType::DelayTimer, deltaTime);
            HandleTimerDecrement(Chip8_CPU::TimerRegisterType::SoundTimer, deltaTime);
        }

        DrawCPURegisterView();

        return true;
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

    void DrawCPURegisterView()
    {
        //Draw the register view
        olc::vf2d topLeftPosition = { 0, Chip8_Screen::HEIGHT };
        //DRAW REGISTERS
        DrawStringDecal(topLeftPosition + 0.5f, "REGISTERS", olc::DARK_RED, { 0.2f, 0.2f });
        DrawStringDecal(topLeftPosition + olc::vf2d(0.5f, 4.5f), "V0-V3", olc::GREEN, { 0.2f, 0.2f });
        DrawStringDecal(topLeftPosition + olc::vf2d(0.5f, 8.5f), "V4-V7", olc::GREEN, { 0.2f, 0.2f });
        DrawStringDecal(topLeftPosition + olc::vf2d(0.5f, 12.5f), "V8-VB", olc::GREEN, { 0.2f, 0.2f });
        DrawStringDecal(topLeftPosition + olc::vf2d(0.5f, 16.5f), "VC-VF", olc::GREEN, { 0.2f, 0.2f });
        for (int gridY = 0; gridY < 4; gridY++)
        {
            for (int gridX = 0; gridX < 4; gridX++)
            {
                constexpr olc::vf2d START_OFFSET = olc::vf2d(6.0f, 2.0f);
                constexpr olc::vf2d SPACING = olc::vf2d(4.0f, 4.0f);
                olc::vf2d position =
                {
                    topLeftPosition.x + START_OFFSET.x + SPACING.x * gridX,
                    topLeftPosition.y + START_OFFSET.y + SPACING.y * gridY
                };
                olc::vf2d startOffset =
                {
                    3.5f,
                    2.5f
                };
                uint8_t value = cpu.GetGPRegisterValue((Chip8_CPU::RegisterID)(gridY * 4 + gridX)); //4 is the max grid width (max gridX)
                DrawStringDecal(startOffset + position, std::format("{:02X}", value), olc::WHITE, { 0.2f, 0.2f });
            }
        }
        DrawStringDecal(topLeftPosition + olc::vf2d( 0.5f, 19.0f), "PC", olc::GREEN, { 0.2f, 0.2f });
        DrawStringDecal(topLeftPosition + olc::vf2d( 4.5f, 19.0f), std::format("{:02X}", cpu.GetProgramCounterValue()), olc::WHITE, { 0.2f, 0.2f });
        DrawStringDecal(topLeftPosition + olc::vf2d(10.5f, 19.0f), "SP", olc::GREEN, { 0.2f, 0.2f });
        DrawStringDecal(topLeftPosition + olc::vf2d(14.5f, 19.0f), std::format("{:02X}", cpu.GetStackPointerValue()), olc::WHITE, { 0.2f, 0.2f });
        DrawStringDecal(topLeftPosition + olc::vf2d( 0.5f, 21.0f), "DT", olc::GREEN, { 0.2f, 0.2f });
        DrawStringDecal(topLeftPosition + olc::vf2d( 4.5f, 21.0f), std::format("{:02X}", cpu.GetTimerValue(Chip8_CPU::TimerRegisterType::DelayTimer)), olc::WHITE, { 0.2f, 0.2f });
        DrawStringDecal(topLeftPosition + olc::vf2d(10.5f, 21.0f), "ST", olc::GREEN, { 0.2f, 0.2f });
        DrawStringDecal(topLeftPosition + olc::vf2d(14.5f, 21.0f), std::format("{:02X}", cpu.GetTimerValue(Chip8_CPU::TimerRegisterType::SoundTimer)), olc::WHITE, { 0.2f, 0.2f });

        //DRAW STACK
        DrawStringDecal(topLeftPosition + olc::vf2d(33.5f, 0.5f), "STACK", olc::DARK_RED, { 0.2f, 0.2f });
        for (int gridX = 0; gridX < 2; gridX++)
        {
            for (int gridY = 0; gridY < 8; gridY++)
            {
                constexpr olc::vf2d START_OFFSET = olc::vf2d(30.0f, 1.6f);
                constexpr olc::vf2d SPACING = olc::vf2d(5.0f, 2.5f);
                olc::vf2d position =
                {
                    topLeftPosition.x + START_OFFSET.x + SPACING.x * gridX,
                    topLeftPosition.y + START_OFFSET.y + SPACING.y * gridY
                };
                olc::vf2d startOffset =
                {
                    3.5f,
                    2.5f
                };
                uint8_t stackAddress = gridX * 8 + gridY;
                uint8_t stackValue = cpu.GetValueInStack(stackAddress); //2 is the max grid width (max gridX)
                DrawStringDecal(olc::vf2d(startOffset.x * (gridX + 1), startOffset.y) + position, std::format("{:02X}", stackAddress), olc::GREEN, { 0.2f, 0.2f });
                DrawStringDecal(olc::vf2d(startOffset.x * (gridX + 2), startOffset.y) + position, std::format("{:02X}", stackValue), olc::WHITE, { 0.2f, 0.2f });
            }
        }
    }
};

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    Logger::Log("=====================LOGGING START=====================");
    srand((unsigned int)time(NULL));
    std::string path = std::string(lpCmdLine);
    Chip8Emulator emulator = Chip8Emulator(path);
    if (emulator.Construct(Chip8_Screen::WIDTH + DISASSEMBLY_VIEW_WIDTH,
        Chip8_Screen::HEIGHT + CPU_REGISTERS_VIEW_HEIGHT,
        10, 10))
    {
        emulator.Start();
    }
    Logger::Log("=====================LOGGING END=====================");
    Logger::Flush(); //Flush all the remaining logs before exiting the program.
    return 0;
}