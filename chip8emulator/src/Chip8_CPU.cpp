#include "Chip8_CPU.h"

Chip8_CPU::Chip8_CPU(Chip8_Keyboard& keyboard, Chip8_Memory& ram, Chip8_Screen& screen) :
    keyboard(keyboard),
    ram(ram),
    screen(screen)
{
    delayTimer = 0x00;
    soundTimer = 0x00;
    pc = 0x200; //Programs will start executing from 0x200
    sp = 0; //Even though this is a pointer, internally it is just an index.

    //Initial values of these registers might need to be changed later on.
    Iregister = 0x0000;
    memset(stack, 0x0000, sizeof(uint16_t) * 16); //Stack size is hardcoded, which should be fine since stack size doesn't change.
    memset(registers, 0x00, sizeof(uint8_t) * (int)RegisterID::COUNT);
}

uint8_t Chip8_CPU::GetGPRegisterValue(Chip8_CPU::RegisterID id)
{
    return registers[(int)id];
}

void Chip8_CPU::SetGPRegisterValue(RegisterID id, uint8_t value)
{
    registers[(int)id] = value;
}

uint16_t Chip8_CPU::GetIRegisterValue()
{
    return Iregister;
}

void Chip8_CPU::SetIRegisterValue(uint16_t value)
{
    Iregister = value;
}

uint8_t Chip8_CPU::GetTimerValue(TimerRegisterType type)
{
    switch (type)
    {
    case TimerRegisterType::DelayTimer:
        return delayTimer;
        break;
    case TimerRegisterType::SoundTimer:
        return soundTimer;
        break;
    }
    return 0;
}

void Chip8_CPU::SetTimerValue(TimerRegisterType type, uint8_t value)
{
    switch (type)
    {
    case TimerRegisterType::DelayTimer:
        delayTimer = value;
        break;
    case TimerRegisterType::SoundTimer:
        soundTimer = value;
        break;
    }
}

bool Chip8_CPU::IncreaseStackPointer()
{
    sp++;
    if (sp > MAX_STACK_INDEX)
    {
        sp = MAX_STACK_INDEX;
        return false;
    }
    return true;
}

bool Chip8_CPU::DecreaseStackPointer()
{
    sp--;
    if (sp < MIN_STACK_INDEX)
    {
        sp = MIN_STACK_INDEX;
        return false;
    }
    return true;
}

void Chip8_CPU::ExecuteNextInstruction()
{
    //Get the opcode from memory
    uint8_t firstOpcodeByte, secondOpcodeByte;
    ram.GetMemory(pc, firstOpcodeByte);
    ram.GetMemory(pc + 1, secondOpcodeByte);
    uint16_t opcode = (firstOpcodeByte << 8) | secondOpcodeByte;
    
    //Increment the program counter to fetch the next instruction next time this function is called.
    pc += 2;

    //Decode the opcode and execute the instruction
    switch (opcode & 0xF000)
    {
    case 0x0000:
        switch (opcode)
        {
        case 0x00E0: //CLS
            screen.ClearScreen();
            break;
        case 0x00EE: //RET
            pc = stack[sp];
            DecreaseStackPointer();
            break;
        default: //SYS addr, old stuff, should be ignored by modern compilers
            break;
        }
        break;
    case 0x1000:
        pc = opcode & 0x0FFF;
        break;
    case 0x2000:
        break;
    case 0x3000:
        break;
    case 0x4000:
        break;
    case 0x5000:
        break;
    case 0x6000:
        break;
    case 0x7000:
        break;
    case 0x8000:
        break;
    case 0x9000:
        break;
    case 0xA000:
        break;
    case 0xB000:
        break;
    case 0xC000:
        break;
    case 0xD000:
        break;
    case 0xE000:
        break;
    case 0xF000:
        break;
    default: //Invalid opcode
        //TODO: Log invalid opcode
        throw std::exception("Invalid opcode");
        break;
    }
}