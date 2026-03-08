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
    #include <stdlib.h>
    #include <time.h>
    // Basic Type Defined

    typedef uint16_t uint16;
    typedef uint8_t  uint8;
    typedef uint32_t uint32;
    
    typedef int16_t  int16;
    typedef int8_t   int8;
    typedef int32_t  int32;
    typedef int32    bool32;
    
    typedef float    real32;
    typedef double   real64;

    #define SCREEN_WIDTH  1280
    #define SCREEN_HEIGHT 640

    // scale factor for video memory layout to map to window width and height.
    #define PIXELSCALE 20

    // vidoe memory pixel layout.
    #define VWIDTH  64 
    #define VHEIGHT 32 

    // Originally reserved for chip8 interpreter.
    #define MemoryStartChip8 0x000
    #define MemoryEndChip8   0x1FF 

    // Memory address Space for 16 built-in characters.
    #define FONTSET_START 0x050
    #define FONTSET_END   0x0A0

    // Rom will be stored starting from  address 0x200, anything are the
    // Rom, is free to use. 
    #define ROMSTART_ADDRESS 0x200

    typedef struct {
        uint8 Registers[16]; // Registers
        uint8 Memory[4096]; // (RAM)
        uint16 IndexRegister; 
        uint16 ProgramCounter; 
        uint16 Stack[16]; 
        uint8 StackPointer; 
        uint8 DelayTimer; 
        uint8 SoundTimer; 
        uint8 inputKeys[16]; // Input Keys
        uint8 VideoMemory[VWIDTH * VHEIGHT]; // Video Memory
        uint16 Opcode; // Opcode
        uint16 RomSize; // RomSize(In Bytes)
    }chip8;

    void PrintFonts(uint8* Memory, uint8 FontSetSize);
    void PrintRom(uint8* Memory, uint16 RomSize);
    void DrawPixelData(uint8* VideoMemory);
    long GetFileSize(const char *FileName);
    void LoadRom(const char* FilePath, chip8* Ch8);
    void DrawFont(uint16 PosX, uint16 PosY, uint8* FontSet, uint16 FontOffSet);
    void RNG(chip8* Ch8, uint8 RegisterIndex); // Random number generator, to set register with some random value.
    
    //Instructions             Assembly       Description
    
    void OP_00E0(chip8* Ch8);// CLS           clear screen.
    void OP_00EE(chip8* Ch8);// RET           return top of the stack and assign it to Program Counter. Returns from subroutine.
    void OP_1nnn(chip8* Ch8);// JP addr,      Jump to nnn location. A jump doesn’t remember its origin, so no stack interaction required.
    void OP_2nnn(chip8* Ch8);// CALL addr,    Call subroutine at nnn location.
    void OP_3xkk(chip8* Ch8);// SE Vx, byte   Skip next instruction if Vx = kk. Vx just means the register indexed by x in the array V[16].
    void OP_3xkk(chip8* Ch8);// SNE Vx, byte  Skip next instruction if Vx != kk.
    void OP_5xy0(chip8* Ch8);// SE Vx, Vy     Skip next instruction if Vx = Vy.
    void OP_6xkk(chip8* Ch8);// LD Vx, byte   Set Vx = kk.
    void OP_7xkk(chip8* Ch8);// ADD Vx, byte  Set Vx = Vx + kk.
    void OP_8xy0(chip8* Ch8);// LD Vx, Vy     Set Vx = Vy.
    
    #ifdef __cplusplus    
}
#endif

#endif

