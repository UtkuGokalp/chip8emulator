#include "Chip8_Screen.h"

Chip8_Screen::Chip8_Screen(olc::PixelGameEngine& instance) : pgeInstance(instance)
{
    memset(screen, 0x00, WIDTH * HEIGHT);
}

void Chip8_Screen::DisplayScreen()
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            //IMPORTANT!!!
            //screen[x + y * WIDTH] == 1 ? olc::WHITE : olc::BLACK part is just a placeholder
            //and is a wrong logic! Change when the other parts of the emulator are able to
            //support the actual logic.
            pgeInstance.Draw(x, y, screen[x + y * WIDTH] == 1 ? olc::WHITE : olc::BLACK);
        }
    }
}