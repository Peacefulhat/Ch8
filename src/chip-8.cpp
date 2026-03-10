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
    for(uint32 i = 0; i < Ch8->Memory; i++)
    {
        Ch8->Memory[i] = 0;
    }
    
    for(uint32 i = 0; i < VWIDTH * VHEIGHT; i++)
    {
        Ch8->VideoMemory[i] = 1;
    }
    
    for(uint8 i = 0; i < 16; i++)
    {
        Ch8->Stack[i] = 0;
    }

    for(int FontIndex = 0; FontIndex < FONTSET_SIZE; FontIndex++)
    {
        Ch8->Memory[FONTSET_START + FontIndex] = Chip8FontSet[FontIndex];
    }
    LoadRom("D:/ch8/src/roms/AstroDodge.ch8", Ch8);
}

void PrintOpcode(chip8* Ch8)
{
    printf("\nOpcode:: \n");
    for(int i = 0; i<Ch8->RomSize; i++){
        printf("%02X\n", (Ch8->Memory[ROMSTART_ADDRESS + i] << 8) | (Ch8->Memory[ROMSTART_ADDRESS + i + 1]));
    }
}

void DebugOut(chip8* Ch8)
{
    printf("Registers::\n");
    for(int i=0; i<16; i++){
        
        printf("Register[%d]:: %02X\n",i, Ch8->Registers[i]);
    }
    printf("Stack::\n");
    for(int i=0; i<16; i++){
        
        printf("Stack[%d]:: %02X\n",i, Ch8->Stack[i]);
    } 
    printf("\nIndexRegister:: %d\n", Ch8->IndexRegister);
    printf("ProgramCounter:: %d\n", Ch8->ProgramCounter); 
   
    printf("DelayTimer:: %02X\n", Ch8->DelayTimer);
    printf("SoundTimer:: %02X\n", Ch8->SoundTimer);
    printf("Opcode:: %02X\n", Ch8->Opcode);
}
int main(int argc, char** argv)
{
    srand(time(NULL));
    chip8 Ch8 = {};
    Chip8Init(&Ch8);
    //    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chip8");
    PrintOpcode(&Ch8);
    while (true)
    {
//        BeginDrawing();
  //      ClearBackground(RED);
    //    DrawPixelData(Ch8.VideoMemory);
    Cycle(&Ch8);
    DebugOut(&Ch8);
      //  EndDrawing();
    }

//    CloseWindow();
    return 0;
}
