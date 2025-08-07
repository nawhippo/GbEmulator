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
    uint8_t* ROMPTR;
    uint16_t programcounter;
    uint16_t* stackPtr = reinterpret_cast<uint16_t*>(0xFFFF);




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
    

    void push(uint16_t value) {
    stackPtr--;
    *stackPtr = value;
    }

void initRAM() {

}



void step(){
    programcounter = getROM(programcounter);

}

//cannot init 
private:
    Singleton() {
    }
};

#endif
