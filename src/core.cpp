#include "core.h"

void Fetch(chip8* Ch8)
{
    Ch8->Opcode = (Ch8->Memory[Ch8->ProgramCounter] << 8u) | Ch8->Memory[Ch8->ProgramCounter + 1];
    // Increment the PC before we execute anything
    Ch8->ProgramCounter += 2;
}

long GetFileSize(const char *FileName)
{
    FILE *Fp = {};
    Fp = fopen(FileName, "rb");

    if (Fp == NULL) return -1;

    if (fseek(Fp, 0, SEEK_END) < 0)
    {
        fclose(Fp);
        return -1;
    }

    long FileSize = ftell(Fp);
    fclose(Fp);
    return FileSize;
}


// some padding issues it look like its stuck to the top of the window.
void DrawPixelData(uint32* VideoMemory)
{
    bool32 Padding = 1;
     
    for(int Row = 0; Row < VHEIGHT; ++Row)
    {
        for(int Cols = 0; Cols < VWIDTH; ++Cols)
        {
            if(VideoMemory[Row * VWIDTH + Cols] == 0){
                DrawRectangle(Cols * PIXELSCALE, (Row * PIXELSCALE), 20, 20, BLACK);
            }
            else
            {
                DrawRectangle(Cols * PIXELSCALE, (Row * PIXELSCALE), 20, 20, RAYWHITE);
            }
        }
        
    }
}


void RNG(chip8* Ch8, uint8 RegisterIndex)
{
    Ch8->Registers[RegisterIndex] = rand() % 255;
}

static uint8 RNG2(){
    return rand() % 256;
}


void LoadRom(const char* FileName, chip8* Ch8)
{
    FILE* File = {};
    File = fopen(FileName, "rb");
    if (!File)
    {
        printf("Failed to open ROM\n");
        return;
    }
    
    long FileSize = GetFileSize(FileName);
    fread(Ch8->Memory + ROMSTART_ADDRESS, 1, FileSize, File);
    Ch8->RomSize = (uint16)(FileSize);
    fclose(File);
}

OpcodeFunc Decode(chip8* Ch8)
{
    switch(Ch8->Opcode & 0xF000)
    
    {
        case 0x1000:{
            return OP_1nnn;
        }break;
        
        case 0x2000:{
            return OP_2nnn;
        }break;

        case 0x3000:{
            return OP_3xkk;
        }break;
        
        case 0x4000:{

            return OP_4xkk;
        }break;
        case 0x5000:{

            return OP_5xy0;
        }break;
        case 0x6000:{

            return OP_6xkk;
        }break;
        case 0x7000:{

            return OP_7xkk;
        }break;
        case 0x8000:{

            switch(Ch8->Opcode & 0x000F)
            {
                case 0x0000:{
                    return OP_8xy0;
                }break;
                case 0x0001:{
                    return OP_8xy1;
                }break;
                case 0x0002:{
                    return OP_8xy2;
                }break;
                case 0x0003:{
                    return OP_8xy3;
                }break;
                case 0x0004:{
                    return OP_8xy4;
                }break;
                case 0x0005:{
                    return OP_8xy5;
                }break;
                case 0x0006:{
                    return OP_8xy6;
                }break;
                case 0x0007:{
                    return OP_8xy7;
                }break;
                case 0x000E:{
                    return OP_8xyE;
                }break;
            }
        }break;
        case 0x0000:{

            switch(Ch8->Opcode & 0x000F)
            {
                case 0x0000:
                    {
                        return OP_00E0;
                    }break;
                    case 0x000E:{

                        return OP_00EE;
                    }break;
                }
            }break;

            case 0xE000:{

                switch(Ch8->Opcode & 0x00FF)
                {
                    case 0x00A1:{

                        return OP_ExA1;
                    }break;
                    case 0x009E:{

                        return OP_Ex9E;
                    }break;
                }
            }break;
            case 0xF000:{

                switch(Ch8->Opcode & 0x00FF)
                {
                    case 0x0007:{
                        return OP_Fx07;
                    }break;
                    case 0x000A:{
                        return OP_Fx0A;
                    }break;
                    case 0x0015:{
                        return OP_Fx15;
                    }break;
                    case 0x0018:{
                        return OP_Fx18;
                    }break;
                    case 0x001E:{
                        return OP_Fx1E;
                    }break;
                    case 0x0029:{
                        return OP_Fx29;
                    }break;
                    case 0x0033:{
                        return OP_Fx33;
                    }break;
                    case 0x0055:{
                        return OP_Fx55;
                    }break;
                    case 0x0065:{
                        return OP_Fx65;
                    }break;
                }
            }break;
            case 0x9000:{
                return OP_9xy0;
            } break;

            case 0xA000:{
                return OP_Annn;
            } break;
            case 0xB000:{
                return OP_Bnnn;
            } break;

            case 0xC000:{
                return OP_Cxkk;
            } break;
            
            case 0xD000:{
                return OP_Dxyn;
            } break;
        }
        return NULL;
    }


    void Cycle(chip8* Ch8)
    {
        Fetch(Ch8);
        OpcodeFunc Execute = Decode(Ch8);
		if (Execute)
		{
			Execute(Ch8);
		}

        if (Ch8->DelayTimer > 0)
        {
            --Ch8->DelayTimer;
        }
        if (Ch8->SoundTimer > 0)
        {
            --Ch8->SoundTimer;
        }

    }

    void OP_0nnn(chip8* Ch8)
    {
        Ch8->ProgramCounter = Ch8->Opcode & 0x0FFF;
    }
    
    void OP_00E0(chip8* Ch8)
    {
        memset(Ch8->VideoMemory, 0, sizeof(Ch8->VideoMemory));
    }

    void OP_00EE(chip8* Ch8)
    {
        --Ch8->StackPointer;
        Ch8->ProgramCounter = Ch8->Stack[Ch8->StackPointer];
    }

    void OP_1nnn(chip8* Ch8)
    {
        // 0001 nnnn nnnn nnnn
        Ch8->ProgramCounter = Ch8->Opcode & 0x0FFF;
    }

    void OP_2nnn(chip8* Ch8)
    {
        // 0002 nnnn nnnn nnnn
        Ch8->Stack[Ch8->StackPointer] = Ch8->ProgramCounter;
        ++Ch8->StackPointer;
        Ch8->ProgramCounter = Ch8->Opcode & 0x0FFF;
    }

    void OP_3xkk(chip8* Ch8)
    {
        // 0003 xxxx kkkk kkkk
        uint8 RegisterIndex = (Ch8->Opcode & 0xF00) >> 8;
        uint8 KK = (Ch8->Opcode & 0x00FF);
        if(Ch8->Registers[RegisterIndex] == KK)
        {
            Ch8->ProgramCounter += 2;
        }
        
    }

    void OP_4xkk(chip8* Ch8)
    {
        // 0004 xxxx kkkk kkkk
        uint8 RegisterIndex = (Ch8->Opcode & 0xF00) >> 8;
        uint8 KK = (Ch8->Opcode & 0x00FF);
        if(Ch8->Registers[RegisterIndex] != KK)
        {
            Ch8->ProgramCounter += 2;
        }
        
    }

    void OP_5xy0(chip8* Ch8)
    {
        // 0005 xxxx yyyy 0000
        uint8 Vx = (Ch8->Opcode & 0x0F00) >> 8;
        uint8 Vy = (Ch8->Opcode & 0x00F0) >> 4;
        if(Ch8->Registers[Vx] == Ch8->Registers[Vy])
        {
            Ch8->ProgramCounter += 2;
        }
    }

    void OP_6xkk(chip8* Ch8)
    {
        // 0006 xxxx kkkk kkkk
        uint8 Byte = Ch8->Opcode & 0x00FF;
        uint8 RegisterIndex = (Ch8->Opcode & 0x0F00) >> 8;
        Ch8->Registers[RegisterIndex] = Byte;
    }

    void OP_7xkk(chip8* Ch8)
    {
        // 0007 xxxx kkkk kkkk
        uint8 Vx = (Ch8->Opcode & 0x0F00) >> 8;
        uint8 Byte = Ch8->Opcode & 0x00FF;
        Ch8->Registers[Vx] += Byte;
    }

    void OP_8xy0(chip8* Ch8)
    {
        // 0008 xxxx yyyy 0000
        uint8 Vx = (Ch8->Opcode & 0x0F00) >> 8;
        uint8 Vy = (Ch8->Opcode & 0x00F0) >> 4;
        Ch8->Registers[Vx] = Ch8->Registers[Vy];
    }

    void OP_8xy1(chip8* Ch8)
    {
        // 0008 xxxx yyyy 1111
        uint8 Vx = (Ch8->Opcode & 0x0F00) >> 8;
        uint8 Vy = (Ch8->Opcode & 0x00F0) >> 4;
        Ch8->Registers[Vx] |= Ch8->Registers[Vy];
    }

    void OP_8xy2(chip8* Ch8)
    {
        // 0008 xxxx yyyy 2222
        uint8 Vx = (Ch8->Opcode & 0x0F00) >> 8;
        uint8 Vy = (Ch8->Opcode & 0x00F0) >> 4;
        Ch8->Registers[Vx] &= Ch8->Registers[Vy];
    }

    void OP_8xy3(chip8* Ch8)
    {
        // 0008 xxxx yyyy 2222
        uint8 Vx = (Ch8->Opcode & 0x0F00) >> 8;
        uint8 Vy = (Ch8->Opcode & 0x00F0) >> 4;
        Ch8->Registers[Vx] ^= Ch8->Registers[Vy];
    }

    void OP_8xy4(chip8* Ch8)
    {
        uint8_t Vx = (Ch8->Opcode & 0x0F00) >> 8;
        uint8_t Vy = (Ch8->Opcode & 0x00F0) >> 4;

        uint16 Sum = Ch8->Registers[Vx] + Ch8->Registers[Vy];
        //The values of Vxvoid OP_8xy4(chip8* Ch8); and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.
        if (Sum > 255)
        {
            Ch8->Registers[0xF] = 1;
        }
        else
        {
            Ch8->Registers[0xF] = 0;
        }

        Ch8->Registers[Vx] = Sum & 0xFF;
    }

    void OP_8xy5(chip8* Ch8)
    {
        //If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
        uint8_t Vx = (Ch8->Opcode & 0x0F00) >> 8;
        uint8_t Vy = (Ch8->Opcode & 0x00F0) >> 4;

        if (Ch8->Registers[Vx] > Ch8->Registers[Vy])
        {
            Ch8->Registers[0xF] = 1;
        }
        else
        {
            Ch8->Registers[0xF] = 0;
        }

        Ch8->Registers[Vx] -= Ch8->Registers[Vy];
    }

    void OP_8xy6(chip8* Ch8)
    {
        uint8 Vx = (Ch8->Opcode & 0x0F00) >> 8;
        Ch8->Registers[0xF] = (Ch8->Registers[Vx] & 0x1);
        Ch8->Registers[Vx] >>= 1;
    }

    void OP_8xy7(chip8* Ch8)
    {
        uint8 Vx = (Ch8->Opcode & 0x0F00) >> 8;
        uint8 Vy = (Ch8->Opcode & 0x00F0) >> 4;
        Ch8->Registers[0xF] = (Ch8->Registers[Vy] > Ch8->Registers[Vx]);
        Ch8->Registers[Vx] = Ch8->Registers[Vy] - Ch8->Registers[Vx];
    }

    void OP_8xyE(chip8* Ch8)
    {
        uint8 Vx = (Ch8->Opcode & 0x0F00) >> 8;
        Ch8->Registers[0xF] = (Ch8->Registers[Vx] & 0x80) >> 7;
        Ch8->Registers[Vx] = Ch8->Registers[Vx] << 1;
    }

    void OP_9xy0(chip8* Ch8)
    {
        uint8 Vx = (Ch8->Opcode & 0x0F00) >> 8;
        uint8 Vy = (Ch8->Opcode & 0x00F0) >> 4;
        if(Ch8->Registers[Vx] != Ch8->Registers[Vy])
        {
            Ch8->ProgramCounter += 2;
        }
    }

    void OP_Annn(chip8* Ch8)
    {
        Ch8->IndexRegister = Ch8->Opcode & 0x0FFF;
    }

    void OP_Bnnn(chip8* Ch8)
    {
        Ch8->ProgramCounter = (Ch8->Opcode & 0x0FFF) + Ch8->Registers[0x0];
    }

    void OP_Cxkk(chip8* Ch8)
    {
        uint8 Vx = (Ch8->Opcode & 0x0F00) >> 8;
        Ch8->Registers[Vx] = RNG2() & (Ch8->Opcode & 0x00FF);
    }

    void OP_Dxyn(chip8* Ch8)
    {
        uint8_t Vx = (Ch8->Opcode & 0x0F00) >> 8;
        uint8_t Vy = (Ch8->Opcode & 0x00F0) >> 4;
        uint8_t Height = Ch8->Opcode & 0x000Fu;

        // Wrap if going beyond screen boundaries
        uint8_t xPos = Ch8->Registers[Vx] % VWIDTH;
        uint8_t yPos = Ch8->Registers[Vy] % VHEIGHT;

        Ch8->Registers[0xF] = 0;

        for (unsigned int Row = 0; Row < Height; ++Row)
        {
            uint8_t SpriteByte = Ch8->Memory[Ch8->IndexRegister + Row];

            for (uint32 Col = 0; Col < 8; ++Col)
            {
                uint8_t SpritePixel = SpriteByte & (0x80u >> Col);
                uint32* ScreenPixel = &Ch8->VideoMemory[(yPos + Row) * VWIDTH + (xPos + Col)];

                // Sprite pixel is on
                if (SpritePixel)
                {
                    // Screen pixel also on - collision
                    if (*ScreenPixel == 0xFFFFFFFF)
                    {
                        Ch8->Registers[0xF] = 1;
                    }

                    // Effectively XOR with the sprite pixel
                    *ScreenPixel ^= 0xFFFFFFFF;
                }
            }
        }
    }

    void OP_Ex9E(chip8* Ch8)
    {
        uint8 Vx = (Ch8->Opcode & 0x0F00) >> 8;
        uint8 Key = Ch8->Registers[Vx];
        if(Ch8->KeyPad[Key])
        {
            Ch8->ProgramCounter += 2;
        }
    }

    void OP_ExA1(chip8* Ch8)
    {
        uint8 Vx = (Ch8->Opcode & 0x0F00) >> 8;
        uint8 Key = Ch8->Registers[Vx];
        if(!Ch8->KeyPad[Key])
        {
            Ch8->ProgramCounter += 2;
        }
    }

    void OP_Fx07(chip8* Ch8)
    {
        uint8 Vx = (Ch8->Opcode & 0x0F00) >> 8;
        Ch8->Registers[Vx] = Ch8->DelayTimer;
    }


    void OP_Fx0A(chip8* Ch8)
    {
        uint8_t Vx = (Ch8->Opcode & 0x0F00u) >> 8u;

        if (Ch8->KeyPad[0])
        {
            Ch8->Registers[Vx] = 0;
        }
        else if (Ch8->KeyPad[1])
        {
            Ch8->Registers[Vx] = 1;
        }
        else if (Ch8->KeyPad[2])
        {
            Ch8->Registers[Vx] = 2;
        }
        else if (Ch8->KeyPad[3])
        {
            Ch8->Registers[Vx] = 3;
        }
        else if (Ch8->KeyPad[4])
        {
            Ch8->Registers[Vx] = 4;
        }
        else if (Ch8->KeyPad[5])
        {
            Ch8->Registers[Vx] = 5;
        }
        else if (Ch8->KeyPad[6])
        {
            Ch8->Registers[Vx] = 6;
        }
        else if (Ch8->KeyPad[7])
        {
            Ch8->Registers[Vx] = 7;
        }
        else if (Ch8->KeyPad[8])
        {
            Ch8->Registers[Vx] = 8;
        }
        else if (Ch8->KeyPad[9])
        {
            Ch8->Registers[Vx] = 9;
        }
        else if (Ch8->KeyPad[10])
        {
            Ch8->Registers[Vx] = 10;
        }
        else if (Ch8->KeyPad[11])
        {
            Ch8->Registers[Vx] = 11;
        }
        else if (Ch8->KeyPad[12])
        {
            Ch8->Registers[Vx] = 12;
        }
        else if (Ch8->KeyPad[13])
        {
            Ch8->Registers[Vx] = 13;
        }
        else if (Ch8->KeyPad[14])
        {
            Ch8->Registers[Vx] = 14;
        }
        else if (Ch8->KeyPad[15])
        {
            Ch8->Registers[Vx] = 15;
        }
        else
        {
            Ch8->ProgramCounter -= 2;
        }
    }

    void OP_Fx15(chip8* Ch8)
    {
        uint8 Vx = (Ch8->Opcode & 0x0F00) >> 8;
        Ch8->DelayTimer = Ch8->Registers[Vx];
    }

    void OP_Fx18(chip8* Ch8)
    {
        uint8 Vx = (Ch8->Opcode & 0x0F00) >> 8;
        Ch8->SoundTimer = Ch8->Registers[Vx];
    }

    void OP_Fx1E(chip8* Ch8)
    {
        uint8 Vx = (Ch8->Opcode & 0x0F00) >> 8;
        Ch8->IndexRegister += Ch8->Registers[Vx];
    }

    void OP_Fx29(chip8* Ch8)
    {
        uint8 Vx = (Ch8->Opcode & 0x0F00) >> 8;
        uint8 Digit = Ch8->Registers[Vx];
        Ch8->IndexRegister = FONTSET_START + (5 * Digit);
    }

    void OP_Fx33(chip8* Ch8)
    {
        uint8_t Vx = (Ch8->Opcode & 0x0F00) >> 8;
        uint8_t Value = Ch8->Registers[Vx];
        Ch8->Memory[Ch8->IndexRegister + 2] = Value % 10;
        Value /= 10;
        Ch8->Memory[Ch8->IndexRegister + 1] = Value % 10;
        Value /= 10;
        Ch8->Memory[Ch8->IndexRegister] = Value % 10;
    }

    void OP_Fx55(chip8* Ch8)
    {
        uint8 Vx = (Ch8->Opcode & 0x0F00) >> 8;
        for(int i = 0; i <= Vx; i++ )
        {
            Ch8->Memory[Ch8->IndexRegister + i] = Ch8->Registers[i];
        }
    }

    void OP_Fx65(chip8* Ch8)
    {
        uint8 Vx = (Ch8->Opcode & 0x0F00) >> 8;
        for(int i = 0; i <= Vx; i++ )
        {
            Ch8->Registers[i] = Ch8->Memory[Ch8->IndexRegister + i]; 
        }
    }
