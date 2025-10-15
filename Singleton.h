#ifndef SINGLETON_H
#define SINGLETON_H
#include <stdio.h>
#include <cstdint>
#include <algorithm>
#include "Registers.h"

class Singleton {
    Registers registers;
    uint8_t romBank0[0x4000];      // 0000–3FFF
    uint8_t romBankN[0x4000];      // 4000–7FFF
    uint8_t vram[0x2000];          // 8000–9FFF
    uint8_t externalRAM[0x2000];   // A000–BFFF
    uint8_t wramBank0[0x1000];     // C000–CFFF
    uint8_t wramBank1[0x1000];     // D000–DFFF
    uint8_t echoRAM[0x1E00];       // E000–FDFF (mirror of C000–DDFF)
    uint8_t oam[0xA0];             // FE00–FE9F
    uint8_t unusable[0x60];        // FEA0–FEFF (not used)
    uint8_t ioRegisters[0x80];     // FF00–FF7F
    uint8_t hram[0x7F];            // FF80–FFFE
    uint8_t interruptEnable;        // FFFF
    uint8_t* lcdc = &ioRegisters[0x40];               //FF40 - addy bus is 16 bits and data bus is 8 bits
    uint8_t* stat = &ioRegisters[0x41];              //  FF41
    uint8_t* lyc = &ioRegisters[0x42];               //  FF42
    uint8_t* ly = &ioRegisters[0x44];                //  FF44 current horizontal line
    uint8_t* ROMPTR;
    uint16_t programcounter;
    uint16_t* stackPtr = reinterpret_cast<uint16_t*>(0xFFFF);
    uint8_t lcd_registers[0x10]; // FF40 - FF4F
    bool IME = 0; //interrupt master enable flag
    uint8_t* IE = &ioRegisters[0xFFFF]; // FFFF 0 - VBlank 1 - LCD STAT 2 - Timer 3 - Serial 4 - Joypad 5 - 7 NONE (if interrupt is enabled, then we check interrupt flags)
    uint8_t* IF = &ioRegisters[0xFF0F]; // FF0F interrupt flag register (THIS ARE FOR SIGNALS, NOT NECESSARILY ENABLED)
     





public:
    static Singleton& getInstance(){
        static Singleton instance;
        return instance;

}

    Singleton(Singleton const&) = delete;
    void operator=(Singleton const&) = delete;


    uint16_t getProgramCounter() { return programcounter; }
    uint16_t* getStackPtr() { return stackPtr; }
    Registers& getRegisters() { return registers; }
    uint8_t pop() { stackPtr++; return *stackPtr; }

    uint8_t* getROMBank0() { return romBank0; }
    uint8_t* getROMBankN() { return romBankN; }
    uint8_t* getVRAM() { return vram; }
    uint8_t* getExternalRAM() { return externalRAM; }
    uint8_t* getWRAMBank0() { return wramBank0; }
    uint8_t* getWRAMBank1() { return wramBank1; }
    uint8_t* getEchoRAM() { return echoRAM; }
    uint8_t* getOAM() { return oam; }
    uint8_t* getIORegisters() { return ioRegisters; }
    uint8_t* getHRAM() { return hram; }
    uint8_t& getInterruptEnable() { return interruptEnable; }
    uint8_t* getUnusable() { return unusable; }
    uint8_t* getLCDC() { return lcdc; }
    uint8_t* getSTAT() { return stat; }
    uint8_t* getLYC() { return lyc; }
    uint8_t* getLY() { return ly; }
    uint8_t* getLCDRegisters() { return lcd_registers; }
    bool getIME() const { return IME; }
    uint8_t* getIE() { return IE; }
    uint8_t* getIF() { return IF; }
    
    //OAMS Start when - 
    void push(uint16_t value) {
    stackPtr--;
    *stackPtr = value;
    }

//cannot init 
private:
    Singleton() {
    }
};

#endif
