
#ifndef CORE_H
#define CORE_H

#ifdef __cplusplus

extern "C"
{
#endif

#include <stdint.h>
// Basic Type Defined

typedef uint16_t uint16;
typedef uint8_t uint8;

typedef int16_t int16;
typedef int8_t  int8;

typedef float    real32;
typedef double   real64;

#define WIDTH  64
#define HEIGHT 32
// Originally reserved for chip8 interpreter.
#define MemoryStartChip8 0x000
#define MemoryEndChip8   0x1FF 

// Storage Space for 16 built-in characters.
#define MemoryForCharsStart 0x050
#define MemoryForCharsEnd   0x0A0

// Rom will be stored starting from  address 0x200, anything are the
// Rom, is free to use 
#define MemoryForROMStart 0x200

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
    uint8 DisplayMemrory[WIDTH * HEIGHT]; // Video Memory.
    uint16 Opcode; // Opcode
}Chip8;

void LoadRom(const char* FilePath);
#ifdef __cplusplus    
}
#endif


#endif

