#include "Chip8_CPU.h"

Chip8_CPU::Chip8_CPU(Chip8_Keyboard& keyboard, Chip8_Memory& ram, Chip8_Screen& screen) :
    keyboard(keyboard),
    ram(ram),
    screen(screen)
{
    ResetRegisters(); //Since resetting gives the initial values to the registers, it can also be used for initializing them.
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

uint16_t Chip8_CPU::GetProgramCounterValue()
{
    return pc;
}

uint8_t Chip8_CPU::GetStackPointerValue()
{
    return sp;
}

uint8_t Chip8_CPU::GetValueInStack(uint8_t address)
{
    if (address > stack.size() - 1)
    {
        Logger::Log("Address exceeded the stack size!", Logger::LogSeverity::LOGSEVERITY_WARNING);
        address = stack.size() - 1;
    }
    return stack.at(address);
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
    if (sp > stack.size() - 1)
    {
        sp = stack.size() - 1;
        Logger::Log("Stack pointer exceeded maximum stack index", Logger::LogSeverity::LOGSEVERITY_WARNING);
        return false;
    }
    return true;
}

bool Chip8_CPU::DecreaseStackPointer()
{
    sp--;
    if (sp < 0)
    {
        sp = 0;
        Logger::Log("Stack pointer exceeded minimum stack index", Logger::LogSeverity::LOGSEVERITY_WARNING);
        return false;
    }
    return true;
}

void Chip8_CPU::ResetRegisters()
{
    delayTimer = 0x00;
    soundTimer = 0x00;
    pc = 0x200; //Programs will start executing from 0x200
    sp = 0; //Even though this is a pointer, internally it is just an index.
    stack.fill(0);
    registers.fill(0);
    Iregister = 0x0000;
}

bool Chip8_CPU::ExecuteNextInstruction()
{
    // Get the opcode from memory
    uint8_t firstOpcodeByte, secondOpcodeByte;
    ram.GetMemory(pc, firstOpcodeByte);
    ram.GetMemory(pc + 1, secondOpcodeByte);
    uint16_t opcode = (firstOpcodeByte << 8) | secondOpcodeByte;

    // Increment the program counter to fetch the next instruction next time this function is called
    pc += 2;

    // Decode the opcode and execute the instruction
    switch (opcode & 0xF000)
    {
    case 0x0000:
        switch (opcode)
        {
        case 0x00E0: // CLS
            screen.ClearScreen();
            break;
        case 0x00EE: // RET
            pc = stack.at(sp);
            DecreaseStackPointer();
            break;
        default:
            break;
        }
        break;

    case 0x1000: // JP addr
        pc = opcode & 0x0FFF;
        break;

    case 0x2000: // CALL addr
        IncreaseStackPointer();
        stack[sp] = pc;
        pc = opcode & 0x0FFF;
        break;

    case 0x3000: // SE Vx, byte
    {
        uint8_t value = opcode & 0x00FF;
        uint8_t registerIndex = (opcode & 0x0F00) >> 8;
        if (registers[registerIndex] == value)
            pc += 2;
        break;
    }

    case 0x4000: // SNE Vx, byte
    {
        uint8_t value = opcode & 0x00FF;
        uint8_t registerIndex = (opcode & 0x0F00) >> 8;
        if (registers[registerIndex] != value)
            pc += 2;
        break;
    }

    case 0x5000: // SE Vx, Vy
    {
        uint8_t registerIndex1 = (opcode & 0x0F00) >> 8;
        uint8_t registerIndex2 = (opcode & 0x00F0) >> 4;
        if (registers[registerIndex1] == registers[registerIndex2])
            pc += 2;
        break;
    }

    case 0x6000: // LD Vx, byte
    {
        uint8_t registerIndex = (opcode & 0x0F00) >> 8;
        uint8_t value = opcode & 0x00FF;
        registers[registerIndex] = value;
        break;
    }

    case 0x7000: // ADD Vx, byte
    {
        uint8_t registerIndex = (opcode & 0x0F00) >> 8;
        uint8_t value = opcode & 0x00FF;
        registers[registerIndex] += value;
        break;
    }

    case 0x8000:
    {
        uint8_t registerIndex1 = (opcode & 0x0F00) >> 8;
        uint8_t registerIndex2 = (opcode & 0x00F0) >> 4;
        switch (opcode & 0x000F)
        {
        case 0x0: // LD Vx, Vy
            registers[registerIndex1] = registers[registerIndex2];
            break;
        case 0x1: // OR Vx, Vy
            registers[registerIndex1] |= registers[registerIndex2];
            break;
        case 0x2: // AND Vx, Vy
            registers[registerIndex1] &= registers[registerIndex2];
            break;
        case 0x3: // XOR Vx, Vy
            registers[registerIndex1] ^= registers[registerIndex2];
            break;
        case 0x4: // ADD Vx, Vy
        {
            uint16_t result = registers[registerIndex1] + registers[registerIndex2];
            registers[(int)RegisterID::VF] = (result > 0xFF) ? 1 : 0;
            registers[registerIndex1] = result & 0xFF;
            break;
        }
        case 0x5: // SUB Vx, Vy
            registers[(int)RegisterID::VF] = registers[registerIndex1] >= registers[registerIndex2];
            registers[registerIndex1] -= registers[registerIndex2];
            break;
        case 0x6: // SHR Vx {, Vy}
            registers[(int)RegisterID::VF] = (registers[registerIndex1] & 0x01);
            registers[registerIndex1] >>= 1;
            break;
        case 0x7: // SUBN Vx, Vy
            registers[(int)RegisterID::VF] = registers[registerIndex2] >= registers[registerIndex1];
            registers[registerIndex1] = registers[registerIndex2] - registers[registerIndex1];
            break;
        case 0xE: // SHL Vx {, Vy}
            registers[(int)RegisterID::VF] = (registers[registerIndex1] & 0x80) >> 7;
            registers[registerIndex1] <<= 1;
            break;
        default:
            Logger::Log(std::format("Invalid opcode: 0x{:04X}", opcode), Logger::LogSeverity::LOGSEVERITY_ERROR);
            return false;
        }
        break;
    }

    case 0x9000: // SNE Vx, Vy
    {
        uint8_t registerIndex1 = (opcode & 0x0F00) >> 8;
        uint8_t registerIndex2 = (opcode & 0x00F0) >> 4;
        if (registers[registerIndex1] != registers[registerIndex2])
            pc += 2;
        break;
    }

    case 0xA000: // LD I, addr
        Iregister = opcode & 0x0FFF;
        break;

    case 0xB000: // JP V0, addr
        pc = registers[(int)RegisterID::V0] + (opcode & 0x0FFF);
        break;

    case 0xC000: // RND Vx, byte
    {
        uint8_t registerIndex = (opcode & 0x0F00) >> 8;
        uint8_t value = opcode & 0x00FF;
        registers[registerIndex] = (rand() % 256) & value;
        break;
    }

    
    case 0xD000: // DRW Vx, Vy, nibble
    {
        uint8_t registerX = (opcode & 0x0F00) >> 8;
        uint8_t registerY = (opcode & 0x00F0) >> 4;
        uint8_t spriteHeight = opcode & 0x000F;

        registers[(int)RegisterID::VF] = 0;

        for (int row = 0; row < spriteHeight; row++)
        {
            uint8_t spriteByte;
            ram.GetMemory(Iregister + row, spriteByte);

            for (int col = 0; col < 8; col++)
            {
                uint8_t spritePixel = (spriteByte >> (7 - col)) & 0x1;
                if (spritePixel == 0) continue;

                int x = (registers[registerX] + col) % 64;
                int y = (registers[registerY] + row) % 32;

                if (screen.GetPixel(x, y) == 1)
                    registers[(int)RegisterID::VF] = 1;

                uint8_t newPixel = screen.GetPixel(x, y) ^ 1;
                screen.SetPixel(x, y, newPixel);
            }
        }
        break;
    }
    
    case 0xE000:
    {
        uint8_t registerIndex = (opcode & 0x0F00) >> 8;
        uint8_t registerValue = registers[registerIndex];
        olc::HWButton info = keyboard.GetKeyInfo((Chip8_Keyboard::Chip8Key)registerValue);
        switch (opcode & 0x00FF)
        {
        case 0x009E:
            if (info.bHeld)
                pc += 2;
            break;
        case 0x00A1:
            if (!info.bHeld)
                pc += 2;
            break;
        default:
            Logger::Log(std::format("Invalid opcode: 0x{:04X}", opcode), Logger::LogSeverity::LOGSEVERITY_ERROR);
            return false;
        }
        break;
    }

    case 0xF000:
    {
        uint8_t registerIndex = (opcode & 0x0F00) >> 8;
        switch (opcode & 0x00FF)
        {
        case 0x07:
            registers[registerIndex] = GetTimerValue(TimerRegisterType::DelayTimer);
            break;
        case 0x0A:
        {
            Chip8_Keyboard::Chip8Key pressedKey;
            if (!keyboard.AnyKeyPressed(pressedKey))
            {
                pc -= 2;
            }
            else
            {
                registers[registerIndex] = (uint8_t)pressedKey;
            }
            break;
        }
        case 0x15:
            SetTimerValue(TimerRegisterType::DelayTimer, registers[registerIndex]);
            break;
        case 0x18:
            SetTimerValue(TimerRegisterType::SoundTimer, registers[registerIndex]);
            break;
        case 0x1E:
            Iregister += registers[registerIndex];
            break;
        case 0x29:
            Iregister = ram.GetMemoryLocationForHexDigitFont(registers[registerIndex]);
            break;
        case 0x33:
        {
            int decimalValue = registers[registerIndex];
            ram.SetMemory(Iregister + 0, decimalValue / 100);
            ram.SetMemory(Iregister + 1, (decimalValue / 10) % 10);
            ram.SetMemory(Iregister + 2, decimalValue % 10);
            break;
        }
        case 0x55:
            for (int i = 0; i <= registerIndex; i++)
                ram.SetMemory(Iregister + i, registers[i]);
            break;
        case 0x65:
            for (int i = 0; i <= registerIndex; i++)
                ram.GetMemory(Iregister + i, registers[i]);
            break;
        default:
            Logger::Log(std::format("Invalid opcode: 0x{:04X}", opcode), Logger::LogSeverity::LOGSEVERITY_ERROR);
            return false;
        }
        break;
    }

    default:
        Logger::Log(std::format("Invalid opcode: 0x{:04X}", opcode), Logger::LogSeverity::LOGSEVERITY_ERROR);
        return false;
    }
    return true;
}
