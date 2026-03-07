
#ifndef CORE_H
#define CORE_H

#ifdef __cplusplus

extern "C"
{
#endif
#include <stdio.h>
#include <stdint.h>
#include "raylib.h"
#include <string.h>
// Basic Type Defined

typedef uint16_t uint16;
typedef uint8_t  uint8;
typedef uint32_t uint32;
typedef int16_t  int16;
typedef int8_t   int8;

typedef float    real32;
typedef double   real64;

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 640

#define WIDTH  64 // 20
#define HEIGHT 32 //20

// Originally reserved for chip8 interpreter.
#define MemoryStartChip8 0x000
#define MemoryEndChip8   0x1FF 

// Storage Space for 16 built-in characters.
#define MemoryForCharsStart 0x050
#define MemoryForCharsEnd   0x0A0

// Rom will be stored starting from  address 0x200, anything are the
// Rom, is free to use 
#define ROMSTART_ADDRESS 0x200

typedef struct {
    uint8 Register[16]; // Registers
    uint8 Memory[4096]; // (RAM)
    uint16 IndexRegister; 
    uint16 ProgramCouner; 
    uint16 Stack[16]; 
    uint8 StackPointer; 
    uint8 DelayTimer; 
    uint8 SoundTimer; 
    uint8 inputKeys[16]; // Input Keys.
    uint8 VideoMemory[WIDTH * HEIGHT]; // Video Memory.
    uint16 Opcode; // Opcode
    uint16 RomSize;
}chip8;

void PrintRom(uint8* Memory, uint16 RomSize);
void DrawPixelData(uint8* VideoMemory);
long GetFileSize(const char *FileName);
void LoadRom(const char* FilePath, chip8* Ch8);

#ifdef __cplusplus    
}
#endif


#endif

