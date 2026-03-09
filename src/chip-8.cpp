#include "core.h"
#define FONTSET_SIZE 80

uint8 Chip8FontSet[FONTSET_SIZE] =
{ 
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void Chip8Init(chip8 *Ch8)
{
    Ch8->ProgramCounter = ROMSTART_ADDRESS;
    
    for(int i = 0; i < VWIDTH * VHEIGHT; i++)
    {
        Ch8->VideoMemory[i] = 1;
    }
    
    for(int FontIndex = 0; FontIndex < FONTSET_SIZE; FontIndex++)
    {
        Ch8->Memory[FONTSET_START + FontIndex] = Chip8FontSet[FontIndex];
    }
    LoadRom("D:/ch8/src/roms/AstroDodge.ch8", Ch8);
}

int main(int argc, char** argv)
{
    srand(time(NULL));
    chip8 Ch8 = {};
    Chip8Init(&Ch8);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chip8");
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RED);
        DrawPixelData(Ch8.VideoMemory);
        EndDrawing();
    }
    PrintRom(&Ch8);
    for(uint16 i = 0; i < 50; i+=2)
    {
        Ch8.Opcode = (Ch8.Memory[ROMSTART_ADDRESS + i] << 8) | Ch8.Memory[ROMSTART_ADDRESS + i + 1];
        printf("Opcode:: %02X\n", Ch8.Opcode);
    }
    CloseWindow();
    return 0;
}
