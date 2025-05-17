#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <Windows.h>
#include <winnt.h>
#include "Chip8HW.h"
#include <map>

class Chip8Emulator : public olc::PixelGameEngine
{
private:
    Chip8_Keyboard keyboard;
    Chip8_Memory ram;
    Chip8_Registers registers;
    Chip8_Screen screen;

public:
    Chip8Emulator() : keyboard(Chip8_Keyboard(*this)), screen(Chip8_Screen(*this))
    {
        sAppName = "Chip-8 Emulator";
        ram = Chip8_Memory();
        registers = Chip8_Registers();
    }

private:
    bool OnUserCreate() override
    {
        return true;
    }

    bool OnUserUpdate(float deltaTime) override
    {
        if (GetKey(olc::Key::ESCAPE).bPressed)
        {
            return false;
        }
        return true;
    }
};

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    Chip8Emulator emulator;
    if (emulator.Construct(Chip8_Screen::WIDTH, Chip8_Screen::HEIGHT, 10, 10))
    {
        emulator.Start();
    }
    return 0;
}