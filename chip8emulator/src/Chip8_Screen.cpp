#include "Chip8_Screen.h"
#include <cstddef>

Chip8_Screen::Chip8_Screen(olc::PixelGameEngine& instance) : pgeInstance(instance), screenSprite(olc::Sprite(WIDTH, HEIGHT))
{
    ClearScreen(); //This sets each pixel in the screen, which works for initialization as well.
}

void Chip8_Screen::DisplayScreen()
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            Pixel& p = screen[x + y * WIDTH];
            if (p.isDirty)
            {
                screenSprite.SetPixel(x, y, p.color == 1 ? olc::WHITE : olc::BLACK);
                p.isDirty = false;
            }
        }
    }
    pgeInstance.DrawSprite({0, 0}, &screenSprite);
}

void Chip8_Screen::ClearScreen()
{
    for (size_t i = 0; i < WIDTH * HEIGHT; i++)
    {
        Pixel& pixel = screen[i];
        pixel.color = 0x00;
        pixel.isDirty = true;
    }
}

uint8_t Chip8_Screen::GetPixel(uint8_t x, uint8_t y)
{
    return screen[x + y * WIDTH].color;
}

void Chip8_Screen::SetPixel(uint8_t x, uint8_t y, uint8_t color)
{
    screen[x + y * WIDTH].color = color;
    screen[x + y * WIDTH].isDirty = true;
}