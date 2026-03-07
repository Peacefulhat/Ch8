#include "core.h"

int main(int argc, char** argv)
{

    chip8 Ch8 = {};
    for (int i = 0; i < 2048; i++)
    {
        Ch8.VideoMemory[i] = 1;
    }

    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chip8");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawPixelData(Ch8.VideoMemory);
        EndDrawing();
    }
    CloseWindow();
    LoadRom("D:/ch8/src/roms/AstroDodge.ch8", &Ch8);
    PrintRom(Ch8.Memory, Ch8.RomSize);
    
    return 0;
}
