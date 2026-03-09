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
        uint8  Registers[16]; // Registers
        uint8  Memory[4096]; // (RAM)
        uint16 IndexRegister; 
        uint16 ProgramCounter; 
        uint16 Stack[16]; 
        uint8  StackPointer; 
        uint8  DelayTimer; 
        uint8  SoundTimer; 
        uint8  KeyPad[16]; // Input Keys
        uint32 VideoMemory[VWIDTH * VHEIGHT];
        uint16 Opcode; // Opcode
        uint16 RomSize; // RomSize(In Bytes)
    }chip8;

    void PrintFonts(uint8* Memory, uint8 FontSetSize);
    void PrintRom(chip8* Ch8);
    void DrawPixelData(uint32* VideoMemory);
    long GetFileSize(const char *FileName);
    void LoadRom(const char* FilePath, chip8* Ch8);
    void Cycle(chip8* Ch8);
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
    void OP_8xy1(chip8* Ch8);// OR Vx, Vy     Set Vx = Vx OR Vy.
    void OP_8xy2(chip8* Ch8);// AND Vx, Vy    Set Vx = Vx AND Vy.
    void OP_8xy3(chip8* Ch8);// XOR Vx, Vy    Set Vx = Vx XOR Vy.
    void OP_8xy4(chip8* Ch8);// ADD Vx, Vy    Set Vx = Vx + Vy, set VF = carry.
    void OP_8xy5(chip8* Ch8);// SUB Vx, Vy    Set Vx = Vx - Vy, set VF = NOT borrow.
    void OP_8xy6(chip8* Ch8);// SHR Vx {, Vy} Set Vx = Vx SHR 1.If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
    
    void OP_8xy7(chip8* Ch8);// SUBN Vx, Vy   Set Vx = Vy - Vx, set VF = NOT borrow.If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
    
    void OP_8xyE(chip8* Ch8);// SHL Vx {, Vy}    Set Vx = Vx SHL 1. If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.

    void OP_9xy0(chip8* Ch8);// SNE Vx, Vy       Skip next instruction if Vx != Vy. The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
    void OP_Annn(chip8* Ch8);// LD I, addr       Set I = nnn. The value of register I is set to nnn.

    void OP_Bnnn(chip8* Ch8);// JP V0, addr      Jump to location nnn + V0. The program counter is set to nnn plus the value of V0.
    
    void OP_Cxkk(chip8* Ch8);// RND Vx, byte     Set Vx = random byte AND kk.The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk. The results are stored in Vx. See instruction 8xy2 for more information on AND.
    /*Dxyn - DRW Vx, Vy, nibble

    Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.

    We iterate over the sprite, row by row and column by column. We know there are eight columns because a sprite is guaranteed to be eight pixels wide.

    If a sprite pixel is on then there may be a collision with what’s already being displayed, so we check if our screen pixel in the same location is set. If so we must set the VF register to express collision.

    Then we can just XOR the screen pixel with 0xFFFFFFFF to essentially XOR it with the sprite pixel (which we now know is on). We can’t XOR directly because the sprite pixel is either 1 or 0 while our video pixel is either 0x00000000 or 0xFFFFFFFF.
    */
    void OP_Dxyn(chip8* Ch8);//  DRW Vx, Vy, nibble    Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
    void OP_Ex9E(chip8* Ch8);//  SKP Vx                Skip next instruction if key with the value of Vx is pressed.
    void OP_ExA1(chip8* Ch8);//  SKNP Vx               Skip next instruction if key with the value of Vx is not pressed.
    void OP_Fx07(chip8* Ch8);//  LD Vx, DT             Set Vx = delay timer value.
    void OP_Fx0A(chip8* Ch8);//  LD Vx, K              Wait for a key press, store the value of the key in Vx.
    //The easiest way to “wait” is to decrement the PC by 2 whenever a keypad value is not detected. This has the effect of running the same instruction repeatedly.
    void OP_Fx15(chip8* Ch8);//  LD DT, Vx             Set delay timer = Vx. DT is set equal to the value of Vx.
    void OP_Fx18(chip8* Ch8);//  LD ST, Vx             Set sound timer = Vx. ST is set equal to the value of Vx.
    void OP_Fx1E(chip8* Ch8);//  ADD I, Vx             Set I = I + Vx.
    void OP_Fx29(chip8* Ch8);//  LD F, Vx              Set I = location of sprite for digit Vx. The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx.
    //We know the font characters are located at 0x50, and we know they’re five bytes each, so we can get the address of the first byte of any character by taking an offset from the start address.
    void OP_Fx33(chip8* Ch8);// LD B, Vx               Store BCD representation of Vx in memory locations I, I+1, and I+2.
    //The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.
    void OP_Fx55(chip8* Ch8);// LD [I], Vx             Store registers V0 through Vx in memory starting at location I.The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.
    void OP_Fx65(chip8* Ch8);// LD Vx, [I]                 Read registers V0 through Vx from memory starting at location I.

    #ifdef __cplusplus    
}
#endif

#endif

