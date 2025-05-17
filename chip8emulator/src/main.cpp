#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <Windows.h>
#include <winnt.h>
#include "Chip8HW.h"
#include <map>

class Renderer : public olc::PixelGameEngine
{
public:
    Renderer()
    {
        sAppName = "Chip-8 Emulator";
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
    Chip8_Memory ram;
    Renderer renderer;
    if (renderer.Construct(Chip8_Screen::WIDTH, Chip8_Screen::HEIGHT, 10, 10))
    {
        renderer.Start();
    }
    return 0;
}