#ifndef SINGLETON_H
#define SINGLETON_H
#include <stdio.h>
#include <cstdint>
#include <algorithm>
#include <cstring>
#include "Registers.h"

class Singleton {
    Registers registers;
    uint8_t* interruptEnable; // FFFF -> &memory_bus[0xFFFF]
    uint8_t memory_bus[0x10000];
    // region pointers (will be set to point inside memory_bus in the ctor)
    // these are bytes of memory, references. they cannot be iterated thru. 
    uint8_t* romBank0 = &memory_bus[0x0000];     // 0000–3FFF -> &memory_bus[0x0000]
    uint8_t* romBankN = &memory_bus[0x4000];      // 4000–7FFF -> &memory_bus[0x4000]
    uint8_t* vram = &memory_bus[0x8000];          // 8000–9FFF -> &memory_bus[0x8000]
    uint8_t* externalRAM = &memory_bus[0xA000];   // A000–BFFF -> &memory_bus[0xA000]
    uint8_t* wramBank0 = &memory_bus[0xC000];     // C000–CFFF -> &memory_bus[0xC000]
    uint8_t* wramBank1 = &memory_bus[0xD000];    // D000–DFFF -> &memory_bus[0xD000]
    uint8_t* tileRAM = &memory_bus[0x9800];    // 9800–9FFF -> &memory_bus[0x9800]
    uint8_t* echoRAM = &memory_bus[0xE000];       // E000–FDFF -> &memory_bus[0xE000]
    uint8_t* OAM = &memory_bus[0xFE00];           // FE00–FE9F -> &memory_bus[0xFE00]
    uint8_t* unusable = &memory_bus[0xFEA0];      // FEA0–FEFF -> &memory_bus[0xFEA0]
    uint8_t* ioRegisters = &memory_bus[0xFF00];   // FF00–FF7F -> &memory_bus[0xFF00]
    uint8_t* hram = &memory_bus[0xFF80];          // FF80–FFFE -> &memory_bus[0xFF80]
    uint8_t* WY = &memory_bus[0xFF4A];          // FF4A - LCDC Y-Coordinate Window Position 
    uint8_t* WX = &memory_bus[0xFF4B];          // FF4B - LCDC X-Coordinate Window Position
    uint8_t* ROMPTR;
    uint16_t programcounter;
    uint16_t* stackPtr = reinterpret_cast<uint16_t*>(0xFFFF);
    uint8_t* lcd_registers = &memory_bus[0xFF40]; // FF40 - FF4F (points into ioRegisters / memory_bus)
    uint8_t* lcdc = &memory_bus[0xFF40]; // LCD Control Register
    uint8_t* SCX = &memory_bus[0xFF42];
    uint8_t* SCY = &memory_bus[0xFF43];
    bool IME = 0; //interrupt master enable flag
     





public:
    static Singleton& getInstance(){
        static Singleton instance;
        return instance;

}

    Singleton(Singleton const&) = delete;
    void operator=(Singleton const&) = delete;
    uint8_t* getSCX() { return SCX; }
    uint8_t* getSCY() { return SCY; }
    uint8_t* getMemoryBus() { return memory_bus;}
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
    uint8_t* getOAM() { return OAM; }
    uint8_t* getIORegisters() { return ioRegisters; }
    uint8_t* getHRAM() { return hram; }
    uint8_t& getInterruptEnable() { return *interruptEnable; }
    uint8_t* getUnusable() { return unusable; }
    uint8_t* getLCDC() { return &ioRegisters[0x40]; }
    uint8_t* getSTAT() { return &ioRegisters[0x41]; }
    uint8_t* getLYC() { return &ioRegisters[0x42]; }
    uint8_t* getLY() { return &ioRegisters[0x44]; }
    uint8_t* getLCDRegisters() { return lcd_registers; }
    bool getIME() const { return IME; }
    uint8_t* getIE() { return interruptEnable; }
    uint8_t* getIF() { return &ioRegisters[0x0F]; }
    uint8_t* getTileRAM() { return tileRAM; }

    void push(uint16_t value) {
    stackPtr--;
    *stackPtr = value;
    }

    private:
        Singleton() {
            memset(memory_bus, 0, sizeof(memory_bus));
        }
};

#endif
