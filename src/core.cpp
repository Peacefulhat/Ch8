#include "core.h"
#define SCALE 20
void DrawFont(uint16 PosX, uint16 PosY, uint8* FontSet, uint16 FontOffSet)
{
    int Pitch = 0;
    uint16 TempX = PosX;
    for(uint16 FontIndex = 0; FontIndex < 5; ++FontIndex)
    {
        //1111 0000  
        //1001 0000
        //1001 0000
        //1001 0000
        //1111 0000
        uint8 Font = FontSet[FontIndex + FontOffSet] >> 4;
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
}

long GetFileSize(const char *FileName)
{
    FILE *Fp = {};
    fopen_s(&Fp, FileName, "rb");

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
    for(int Row = 0; Row < HEIGHT; ++Row)
    {
        for(int Cols = 0; Cols < WIDTH; ++Cols)
        {
            Color PixelColor = {VideoMemory[Row * WIDTH + Cols], VideoMemory[Row * WIDTH + Cols], VideoMemory[Row * WIDTH + Cols], 255};
                DrawRectangle(Cols * PIXELSCALE, Row * PIXELSCALE, 20, 20, PixelColor);
        }
    }
}

void PrintRom(uint8* Memory, uint16 RomSize)
{
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
    fopen_s(&File, FileName, "rb");
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
