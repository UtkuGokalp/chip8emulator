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

public:
    Chip8_Screen(olc::PixelGameEngine& instance);
    void DisplayScreen();
    void ClearScreen();
};
