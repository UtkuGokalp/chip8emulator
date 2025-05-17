#pragma once

#include <map>
#include <cstdint>
#include <olcPixelGameEngine.h>

class Chip8_Keyboard
{
public:
    enum class Chip8Key
    {
        KEY_0 = 0,
        KEY_1,
        KEY_2,
        KEY_3,
        KEY_4,
        KEY_5,
        KEY_6,
        KEY_7,
        KEY_8,
        KEY_9,
        KEY_A,
        KEY_B,
        KEY_C,
        KEY_D,
        KEY_E,
        KEY_F,
    };
    Chip8_Keyboard(const olc::PixelGameEngine& engineInstance);
    olc::HWButton GetKeyInfo(Chip8Key key);
    bool AnyKeyPressed(Chip8Key& key);
private:
    std::map<Chip8Key, olc::Key> keymap =
    {
        { { Chip8Key::KEY_A }, { olc::Key::A   } },
        { { Chip8Key::KEY_B }, { olc::Key::B   } },
        { { Chip8Key::KEY_C }, { olc::Key::C   } },
        { { Chip8Key::KEY_D }, { olc::Key::D   } },
        { { Chip8Key::KEY_E }, { olc::Key::E   } },
        { { Chip8Key::KEY_F }, { olc::Key::F   } },
        { { Chip8Key::KEY_0 }, { olc::Key::NP0 } },
        { { Chip8Key::KEY_1 }, { olc::Key::NP1 } },
        { { Chip8Key::KEY_2 }, { olc::Key::NP2 } },
        { { Chip8Key::KEY_3 }, { olc::Key::NP3 } },
        { { Chip8Key::KEY_4 }, { olc::Key::NP4 } },
        { { Chip8Key::KEY_5 }, { olc::Key::NP5 } },
        { { Chip8Key::KEY_6 }, { olc::Key::NP6 } },
        { { Chip8Key::KEY_7 }, { olc::Key::NP7 } },
        { { Chip8Key::KEY_8 }, { olc::Key::NP8 } },
        { { Chip8Key::KEY_9 }, { olc::Key::NP9 } },
    };
    const olc::PixelGameEngine& pgeInstance;
};