#include "core.h"

#define SCALE 2
#define FONTCANVASWIDTH 10
void DrawFont(uint16 PosX, uint16 PosY, uint8* FontSet, uint16 FontOffSet)
{
    uint16 Pitch = 0;
    uint16 TempX = PosX;
    for(uint16 Offset = 0; Offset < 16; Offset++){

        for(uint16 FontIndex = 0; FontIndex < 5; ++FontIndex)
        {
            uint8 Font = FontSet[FontIndex + FontOffSet * Offset] >> 4;
            uint16 Mask = 0x8; 
            while(Mask)
            {
                if(Font & Mask)
                {
                    DrawRectangle((PosX)*SCALE , (PosY + Pitch)*SCALE, 2*SCALE, 2*SCALE, RAYWHITE);
                }else
                {
                    DrawRectangle((PosX)*SCALE , (PosY + Pitch)*SCALE, 2*SCALE, 2*SCALE, BLACK);
                }
                PosX += 2;
                Mask >>= 1;
                
            }
            PosX = TempX;
            Pitch +=2;
        }
        Pitch +=5;
    }
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



void DrawPixelData(uint8* VideoMemory)
{
    for(int Row = 0; Row < VHEIGHT; ++Row)
    {
        for(int Cols = 0; Cols < VWIDTH; ++Cols)
        {
            if(VideoMemory[Row * VWIDTH + Cols] == 0){
                DrawRectangle(Cols * PIXELSCALE, Row * PIXELSCALE, 20, 20, BLACK);
            }
            else
            {
                DrawRectangle(Cols * PIXELSCALE, Row * PIXELSCALE, 20, 20, RAYWHITE);
            }
        }
        
    }
}

void PrintFonts(uint8* Memory, uint8 FontSetSize)
{
    printf("\nFont::\n");
    for(int FontIndex = 0; FontIndex < FontSetSize; FontIndex++)
    {
        if((FontIndex + 1) % 5 == 0)
        {
            printf("%02X\n", Memory[FONTSET_START + FontIndex]);
            continue;
        }
        printf("%02X ", Memory[FONTSET_START + FontIndex]);

    }   
}

void PrintRom(uint8* Memory, uint16 RomSize)
{
    printf("Rom::\n");
    for(uint32 i = 0; i < RomSize; ++i)
    {
        printf("%02X ", Memory[ROMSTART_ADDRESS + i]);

        if((i + 1)  % 16  == 0)
        printf("\n");
    }
    
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

void RNG(chip8* Ch8, uint8 RegisterIndex)
{
    Ch8->Registers[RegisterIndex] = rand() % 255;
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
    Ch8->ProgramCounter = Ch8->Opcode & 0x0FFF;
}

void OP_2nnn(chip8* Ch8)
{
    Ch8->Stack[Ch8->StackPointer] = Ch8->ProgramCounter;
    ++Ch8->StackPointer;
    Ch8->ProgramCounter = Ch8->Opcode & 0x0FFF;
}

void OP_3xkk(chip8* Ch8)
{
    uint8 RegisterIndex = (Ch8->Opcode & 0xF00) >> 8;
    uint8 KK = (Ch8->Opcode & 0x00FF);
    if(Ch8->Registers[RegisterIndex] == KK)
    {
        Ch8->ProgramCounter += 2;
    }
    
}

void OP_4xkk(chip8* Ch8)
{
    uint8 RegisterIndex = (Ch8->Opcode & 0xF00) >> 8;
    uint8 KK = (Ch8->Opcode & 0x00FF);
    if(Ch8->Registers[RegisterIndex] != KK)
    {
        Ch8->ProgramCounter += 2;
    }
    
}
