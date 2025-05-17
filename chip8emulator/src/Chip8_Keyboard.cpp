#include "Chip8_Keyboard.h"

Chip8_Keyboard::Chip8_Keyboard(const olc::PixelGameEngine& engineInstance) :
    pgeInstance(engineInstance)
{

}

olc::HWButton Chip8_Keyboard::GetKeyInfo(Chip8Key key)
{
    return pgeInstance.GetKey(keymap[key]);
}

bool Chip8_Keyboard::AnyKeyPressed(Chip8Key& key)
{
    for (auto& pair : keymap)
    {
        if (pgeInstance.GetKey(pair.second).bHeld)
        {
            key = pair.first;
            return true;
        }
    }
    return false;
}