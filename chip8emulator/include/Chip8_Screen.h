#pragma once

#include <cstdint>
#include "olcPixelGameEngine.h"

class Chip8_Screen
{
public:
    //Apparently there are other resolutions but just support the og resolution right now.
    static constexpr int WIDTH = 64;
    static constexpr int HEIGHT = 32;

private:
    uint8_t screen[WIDTH * HEIGHT];
    olc::PixelGameEngine& pgeInstance;
    olc::Sprite screenSprite;

public:
    Chip8_Screen(olc::PixelGameEngine& instance);
    void DisplayScreen();
    void ClearScreen();
    uint8_t GetPixel(uint8_t x, uint8_t y);
    void SetPixel(uint8_t x, uint8_t y, uint8_t color);
};
