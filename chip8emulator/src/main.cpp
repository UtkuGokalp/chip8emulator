//The ordering of these defines and includes are important
#define OLC_PGE_APPLICATION
#define OLC_PGEX_DECALS
#include "olcPixelGameEngine.h"
#define OLC_PGEX_SOUND
#include "olcPGEX_Sound.h"
//After here the usual rules apply, ordering isn't important
#include "Chip8_CPU.h"
#include "Chip8_Keyboard.h"
#include "Chip8_Memory.h"
#include "Chip8_Screen.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <cstdlib>

#define CPU_REGISTERS_VIEW_WIDTH                  Chip8_Screen::WIDTH
#define CPU_REGISTERS_VIEW_HEIGHT                 24

#define MEMORY_VIEW_WIDTH             45
#define MEMORY_VIEW_HEIGHT            Chip8_Screen::HEIGHT
#define MEM_VIEW_ROWS                 8
#define MEM_VIEW_COLS                 16
#define BYTES_IN_ONE_PAGE             (MEM_VIEW_ROWS * MEM_VIEW_COLS)
#define MAX_PAGE_COUNT                ((Chip8_Memory::MEMORY_SIZE_IN_BYTES / BYTES_IN_ONE_PAGE) - 1) //Subtract 1 because initial page is 0, not 1

#define KEYBOARD_VIEW_WIDTH           30
#define KEYBOARD_VIEW_HEIGHT          35

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
    unsigned int memoryPageToDisplay;

public:
    Chip8Emulator(const std::string& romPath) :
        keyboard(Chip8_Keyboard(*this)),
        screen(Chip8_Screen(*this)),
        ram(Chip8_Memory()),
        cpu(Chip8_CPU(keyboard, ram, screen)),
        romPath(romPath),
        memoryPageToDisplay(0)
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
        Logger::Log(std::format("Loading ROM: {}", romPath));
        if (ram.LoadROM(romPath) == false)
        {
            Logger::Log("Failed to load ROM. Either the file doesn't exist or is too big.", Logger::LogSeverity::LOGSEVERITY_ERROR);
            return false;
        }
        Logger::Log("Successfully loaded ROM.");

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

        if (GetKey(olc::Key::RIGHT).bPressed || GetMouseWheel() < 0) //If right arrow is pressed or if mouse wheel is scrolled down
        {
            if (memoryPageToDisplay < MAX_PAGE_COUNT)
            {
                memoryPageToDisplay++;
            }
        }
        if (GetKey(olc::Key::LEFT).bPressed || GetMouseWheel() > 0) //If left arrow is pressed or if mouse wheel is scrolled up
        {
            if (memoryPageToDisplay > 0) //Smallest page is always 0
            {
                memoryPageToDisplay--;
            }
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
        DrawMemoryView(memoryPageToDisplay);
        DrawKeyboardView();
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
        //The timers should decrease by 1 every 1/60 seconds (aka 0.01666666 seconds)
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
        DrawStringDecal(topLeftPosition + olc::vf2d(9.5f, 4.5f), std::format("{:02X} {:02X} {:02X} {:02X}",
            cpu.GetGPRegisterValue((Chip8_CPU::RegisterID)0),
            cpu.GetGPRegisterValue((Chip8_CPU::RegisterID)1),
            cpu.GetGPRegisterValue((Chip8_CPU::RegisterID)2),
            cpu.GetGPRegisterValue((Chip8_CPU::RegisterID)3)),
            olc::WHITE, { 0.2f, 0.2f });

        DrawStringDecal(topLeftPosition + olc::vf2d(0.5f, 8.5f), "V4-V7", olc::GREEN, { 0.2f, 0.2f });
        DrawStringDecal(topLeftPosition + olc::vf2d(9.5f, 8.5f), std::format("{:02X} {:02X} {:02X} {:02X}",
            cpu.GetGPRegisterValue((Chip8_CPU::RegisterID)4),
            cpu.GetGPRegisterValue((Chip8_CPU::RegisterID)5),
            cpu.GetGPRegisterValue((Chip8_CPU::RegisterID)6),
            cpu.GetGPRegisterValue((Chip8_CPU::RegisterID)7)),
            olc::WHITE, { 0.2f, 0.2f });

        DrawStringDecal(topLeftPosition + olc::vf2d(0.5f, 12.5f), "V8-VB", olc::GREEN, { 0.2f, 0.2f });
        DrawStringDecal(topLeftPosition + olc::vf2d(9.5f, 12.5f), std::format("{:02X} {:02X} {:02X} {:02X}",
            cpu.GetGPRegisterValue((Chip8_CPU::RegisterID)8),
            cpu.GetGPRegisterValue((Chip8_CPU::RegisterID)9),
            cpu.GetGPRegisterValue((Chip8_CPU::RegisterID)10),
            cpu.GetGPRegisterValue((Chip8_CPU::RegisterID)11)),
            olc::WHITE, { 0.2f, 0.2f });

        DrawStringDecal(topLeftPosition + olc::vf2d(0.5f, 16.5f), "VC-VF", olc::GREEN, { 0.2f, 0.2f });
        DrawStringDecal(topLeftPosition + olc::vf2d(9.5f, 16.5f), std::format("{:02X} {:02X} {:02X} {:02X}",
            cpu.GetGPRegisterValue((Chip8_CPU::RegisterID)12),
            cpu.GetGPRegisterValue((Chip8_CPU::RegisterID)13),
            cpu.GetGPRegisterValue((Chip8_CPU::RegisterID)14),
            cpu.GetGPRegisterValue((Chip8_CPU::RegisterID)15)),
            olc::WHITE, { 0.2f, 0.2f });


        DrawStringDecal(topLeftPosition + olc::vf2d(0.5f, 19.0f), "PC", olc::GREEN, { 0.2f, 0.2f });
        DrawStringDecal(topLeftPosition + olc::vf2d(4.5f, 19.0f), std::format("{:02X}", cpu.GetProgramCounterValue()), olc::WHITE, { 0.2f, 0.2f });
        DrawStringDecal(topLeftPosition + olc::vf2d(10.5f, 19.0f), "SP", olc::GREEN, { 0.2f, 0.2f });
        DrawStringDecal(topLeftPosition + olc::vf2d(14.5f, 19.0f), std::format("{:02X}", cpu.GetStackPointerValue()), olc::WHITE, { 0.2f, 0.2f });
        DrawStringDecal(topLeftPosition + olc::vf2d(0.5f, 21.0f), "DT", olc::GREEN, { 0.2f, 0.2f });
        DrawStringDecal(topLeftPosition + olc::vf2d(4.5f, 21.0f), std::format("{:02X}", cpu.GetTimerValue(Chip8_CPU::TimerRegisterType::DelayTimer)), olc::WHITE, { 0.2f, 0.2f });
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

    void DrawMemoryView(unsigned int page)
    {
        //Check if page is valid
        if (page > MAX_PAGE_COUNT)
        {
            std::stringstream ss;
            ss << std::format("Invalid memory page!({}) Defaulting to MAX_PAGE_COUNT ({}).", page, MAX_PAGE_COUNT);
            Logger::Log(ss.str(), Logger::LogSeverity::LOGSEVERITY_WARNING);
            //Also setting pageToDisplay to 1 so that the log will happen only once. This is somewhat hacky and not necessarily a good practice.
            //But I feel like since the page shouldn't be out of bounds in any condition, this can help with an external bug as well. Possibly.
            //Unless a bigger problem arises, I will keep it this way.
            memoryPageToDisplay = page = MAX_PAGE_COUNT;
        }

        //Draw the memory title
        olc::vf2d memoryTextPosition = { Chip8_Screen::WIDTH + 0.5f, 0.5f }; //0.5f for a little bit of offset
        std::stringstream memoryViewTitleSS;
        memoryViewTitleSS << std::format("MEMORY({}/{})", page, MAX_PAGE_COUNT);
        DrawStringDecal(memoryTextPosition, memoryViewTitleSS.str(), olc::DARK_RED, { 0.2f, 0.2f });

        //Draw the bytes in memory
        for (int col = 0; col < MEM_VIEW_COLS; col++)
        {
            //Display the address
            std::stringstream ss;
            ss << std::format("{:03X}", (page * BYTES_IN_ONE_PAGE) + (col * MEM_VIEW_ROWS));
            DrawStringDecal(memoryTextPosition + olc::vf2d(0.0f, 2.5f + col * 2.0f), ss.str(), olc::GREEN, { 0.2f, 0.2f });

            ss.str(""); //Clear the stringstream

            //Read the memory and display it
            for (int row = 0; row < MEM_VIEW_ROWS; row++)
            {
                uint8_t memValue;
                uint16_t address = (page * BYTES_IN_ONE_PAGE) + (col * MEM_VIEW_ROWS) + row;
                std::stringstream loggerss;
                loggerss << address;
                ram.GetMemory(address, memValue);
                ss << std::format(" {:02X}", memValue);
            }
            DrawStringDecal(memoryTextPosition + olc::vf2d(4.0f, 2.5f + col * 2.0f), ss.str(), olc::WHITE, { 0.2f, 0.2f });
        }
    }

    void DrawKeyboardView()
    {
        olc::vf2d infoTextStartingPos = { 65.0f, 37.0f };
        DrawStringDecal(infoTextStartingPos, "Controls\n\n1 2 3 4\n\nQ W E R\n\nA S D F\n\nZ X C V\n\nMouse\nwheel to\nscroll\nthrough\nmemory\npages", olc::CYAN, { 0.2f, 0.2f });

        olc::vf2d keysStartingPos = { 81.0f, 37.0f };
        constexpr olc::vf2d KEY_SIZE = { 5.0f, 5.0f };
        constexpr float SPACING = 2.0f;
        constexpr int GRID_SIZE = 4; //Chip8 has 16 keys, so a square grid is fine
        std::string KEYS[16] = { "1", "2", "3", "C",
                                 "4", "5", "6", "D",
                                 "7", "8", "9", "E",
                                 "A", "0", "B", "F", };
        typedef Chip8_Keyboard::Chip8Key KEY_ENUM;
        Chip8_Keyboard::Chip8Key keys[16] = {
            KEY_ENUM::KEY_1, KEY_ENUM::KEY_2, KEY_ENUM::KEY_3, KEY_ENUM::KEY_C,
            KEY_ENUM::KEY_4, KEY_ENUM::KEY_5, KEY_ENUM::KEY_6, KEY_ENUM::KEY_D,
            KEY_ENUM::KEY_7, KEY_ENUM::KEY_8, KEY_ENUM::KEY_9, KEY_ENUM::KEY_E,
            KEY_ENUM::KEY_A, KEY_ENUM::KEY_0, KEY_ENUM::KEY_B, KEY_ENUM::KEY_F,
        };
        for (int x = 0; x < GRID_SIZE; x++)
        {
            olc::Pixel normalColor = olc::WHITE;
            olc::Pixel pressedColor = olc::GREEN;
            for (int y = 0; y < GRID_SIZE; y++)
            {
                size_t index = x + GRID_SIZE * y;
                bool pressed = keyboard.GetKeyInfo(keys[index]).bHeld;

                olc::vf2d keyOffset = olc::vf2d((KEY_SIZE.x + SPACING) * x,
                    (KEY_SIZE.y + SPACING) * y);
                olc::vf2d keyPosition = keysStartingPos + keyOffset;
                DrawRectDecal(keyPosition, KEY_SIZE, pressed ? pressedColor : normalColor);
                olc::vf2d keyTextOffset = { 1, 1 };
                DrawStringDecal(keyPosition + keyTextOffset, KEYS[index], pressed ? pressedColor : normalColor, { 0.4f, 0.4f });
            }
        }
    }
};

void EndLogging(void)
{
    Logger::Log("=====================LOGGING END=====================");
    Logger::Flush(); //Flush all the remaining logs before exiting the program.
}

int main(int argc, char** argv)
{
    Logger::Log("=====================LOGGING START=====================");
    srand((unsigned int)time(NULL));
    atexit(EndLogging);
    if (argc != 2)
    {
        Logger::Log("Usage: <binary_name> <rom_path>", Logger::LogSeverity::LOGSEVERITY_ERROR);
        return 1;
    }
    std::string path = std::string(argv[1]);
    Chip8Emulator emulator = Chip8Emulator(path);
    if (emulator.Construct(Chip8_Screen::WIDTH + MEMORY_VIEW_WIDTH,
        Chip8_Screen::HEIGHT + KEYBOARD_VIEW_HEIGHT,
        10, 10, false, true))
    {
        emulator.Start();
    }
    return 0;
}