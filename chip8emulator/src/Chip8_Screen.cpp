#include "Chip8_Screen.h"

Chip8_Screen::Chip8_Screen(olc::PixelGameEngine& instance) : pgeInstance(instance)
{
    memset(screen, 0x00, WIDTH * HEIGHT);
    screenSprite = olc::Sprite(WIDTH, HEIGHT);
}

void Chip8_Screen::DisplayScreen()
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            screenSprite.SetPixel(x, y, GetPixel(x, y) == 1 ? olc::WHITE : olc::BLACK);
        }
    }
    pgeInstance.DrawSprite({0, 0}, &screenSprite);
}

void Chip8_Screen::ClearScreen()
{
    memset(screen, 0x00, WIDTH * HEIGHT);
}

uint8_t Chip8_Screen::GetPixel(uint8_t x, uint8_t y)
{
    return screen[x + y * WIDTH];
}

void Chip8_Screen::SetPixel(uint8_t x, uint8_t y, uint8_t color)
{
    screen[x + y * WIDTH] = color;
}