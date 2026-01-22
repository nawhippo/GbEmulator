#ifndef SINGLETON_H
#define SINGLETON_H
#include <stdio.h>
#include <cstdint>
#include <algorithm>
#include <vector>
#include <cstring>
#include "Registers.h"
#include "Clock.h"
#include "Accumulator.h"
class Singleton {
    Clock clock;
    Registers registers;
    uint8_t memory_bus[0x10000];
    uint16_t stackPtr = 0xFFFE;
    uint16_t programcounter = 0x0000;
    uint8_t* interruptEnable = nullptr;
    uint8_t* interruptRequested = nullptr;
    uint8_t* romBank0 = nullptr;
    uint8_t* romBankN = nullptr;
    uint8_t* vram = nullptr;
    uint8_t* externalRAM = nullptr;
    uint8_t* wramBank0 = nullptr;
    uint8_t* wramBank1 = nullptr;
    uint8_t* tileRAM = nullptr;
    uint8_t* echoRAM = nullptr;
    uint8_t* OAM = nullptr;
    uint8_t* unusable = nullptr;
    uint8_t* hram = nullptr;
    uint8_t* WY = nullptr;
    uint8_t* WX = nullptr;
    uint8_t* lcd_registers = nullptr;
    uint8_t* lcdc = nullptr;
    uint8_t* stat = nullptr;
    uint8_t* lyc = nullptr;
    uint8_t* ly = nullptr;
    uint8_t* SCX = nullptr;
    uint8_t* SCY = nullptr;
    uint8_t* IF = nullptr;
    uint8_t* IE = nullptr;
    bool* IME = 0;

    // Audio channel length timer references
    uint8_t* channel1LengthTimer = nullptr;
    uint8_t* channel2LengthTimer = nullptr;
    uint8_t* channel3LengthTimer = nullptr;
    uint8_t* channel4LengthTimer = nullptr;

    //SHADOW VALUES FOR READ ONLY VARIABLES.
    uint16_t channel1ShadowFrequency; //nr13 nr14 0xFF13 0xFF14
    uint16_t channel2ShadowFrequency; //nr18 nr19 0xFF18 0xFF19
    uint16_t channel3ShadowFrequency; //nr18 nr19 0xFF1D 0xFF1E

    //32 samples fixed 4-bit each vari speed 
    uint8_t channel3LengthTimer;
    uint8_t channel4LengthTimer;
    bool channel1ShadowTrigger;
    bool channel2ShadowTrigger;
    bool channel3Trigger;
    bool channel4ShadowTrigger;

    
    bool InterruptEnableMaster = 0; //interrupt master enable flag
     





public:
    //cant be bothered with getters and setters for this
    vector<Accumulator> Vectors; 
    static Singleton& getInstance(){
        static Singleton instance;
        return instance;
    
}

    Singleton(Singleton const&) = delete;
    void operator= (Singleton const&) = delete;
    Clock& getClock() { return clock; }
    Registers& getRegisters() { return registers; }
    uint8_t* getChannel1ShadowFrequency(){ return Channel1ShadowFrequency; }
    uint8_t* getChannel2ShadowFrequency(){ return &channel2ShadowFrequency; }
    uint8_t* getChannel3LengthTimer(){ return &channel3LengthTimer; }
    uint8_t* getChannel1LengthTimer() { return channel1LengthTimer; }
    uint8_t* getChannel2LengthTimer() { return channel2LengthTimer; }
    uint8_t* getChannel3LengthTimer() { return channel3LengthTimer; }
    uint8_t* getChannel4LengthTimer() { return channel4LengthTimer; }
    bool getChannel1ShadowTrigger() { return channel1ShadowTrigger; }
    bool getChannel2ShadowTrigger() { return channel2ShadowTrigger; }
    bool getChannel3ShadowTrigger() { return channel3Trigger; }
    bool getChannel4ShadowTrigger() { return channel4ShadowTrigger; }
    uint8_t* getSCX() { return SCX; }
    uint8_t* getSCY() { return SCY; }
    uint8_t* getMemoryBus() { return memory_bus;}
    uint16_t& getProgramCounter() { return programcounter; }
    uint16_t& getStackPtr() { return stackPtr; }
    uint8_t* getROMBank0() { return romBank0; }
    uint8_t* getROMBankN() { return romBankN; }
    uint8_t* getVRAM() { return vram; }
    uint8_t* getExternalRAM() { return externalRAM; }
    uint8_t* getWRAMBank0() { return wramBank0; }
    uint8_t* getWRAMBank1() { return wramBank1; }
    uint8_t* getEchoRAM() { return echoRAM; }
    uint8_t* getOAM() { return OAM; }
    bool* getIME() { return IME; }
    uint8_t* getHRAM() { return hram; }
    uint8_t* getUnusable() { return unusable; }
    uint8_t* getLCDC() { return lcdc; }
    uint8_t* getSTAT() { return stat; }
    uint8_t* getLYC() { return lyc; }
    uint8_t* getLY() { return ly; }
    uint8_t* getLCDRegisters() { return lcd_registers; }
    uint8_t* getIE() { return interruptEnable; }
    uint8_t* getIF() { return IF; }
    uint8_t* getTileRAM() { return tileRAM; }
    uint8_t* getInterruptRequested() { return interruptRequested; }
    
    void push(uint16_t value) {
    memory_bus[stackPtr] = value >> 8 & 0xFFFF;
    memory_bus[stackPtr + 1] = static_cast<uint8_t>(value & 0xFF);
    stackPtr-=2;
    }

    uint16_t pop() {
    uint16_t result = static_cast<uint16_t>(memory_bus[stackPtr]) | (static_cast<uint16_t>(memory_bus[stackPtr + 1]) << 8);
    stackPtr += 2;
    return result;
    }

    private:
        Singleton() {
            memset(memory_bus, 0, sizeof(memory_bus));
            // set pointers after memory_bus exists
            interruptEnable = &memory_bus[0xFFFF];
            interruptRequested = &memory_bus[0xFF0F];
            romBank0       = &memory_bus[0x0000];
            romBankN       = &memory_bus[0x4000];
            vram           = &memory_bus[0x8000];
            externalRAM    = &memory_bus[0xA000];
            wramBank0      = &memory_bus[0xC000];
            wramBank1      = &memory_bus[0xD000];
            tileRAM        = &memory_bus[0x9800];
            echoRAM        = &memory_bus[0xE000];
            OAM            = &memory_bus[0xFE00];
            unusable       = &memory_bus[0xFEA0];
            hram           = &memory_bus[0xFF80];
            lcd_registers  = &memory_bus[0xFF40];
            lcdc           = &memory_bus[0xFF40];
            stat           = &memory_bus[0xFF41];
            lyc            = &memory_bus[0xFF44];
            ly             = &memory_bus[0xFF45];
            SCX            = &memory_bus[0xFF42];
            SCY            = &memory_bus[0xFF43];
            WY             = &memory_bus[0xFF4A];
            WX             = &memory_bus[0xFF4B];
            IF             = &memory_bus[0xFF0F];
            IE             = &memory_bus[0xFFFF];
            
            // Initialize audio channel length timer references
            channel1LengthTimer = &memory_bus[0xFF11]; // NR11 - Channel 1 Sound length/Wave pattern duty
            channel2LengthTimer = &memory_bus[0xFF16]; // NR21 - Channel 2 Sound length/Wave pattern duty  
            channel3LengthTimer = &memory_bus[0xFF1B]; // NR31 - Channel 3 Sound length
            channel4LengthTimer = &memory_bus[0xFF20]; // NR41 - Channel 4 Sound length
            
            channel1ShadowFrequency = (static_cast<uint16_t>(memory_bus[0xFF14] & 0x07) << 8) | memory_bus[0xFF13];
            channel2ShadowFrequency = ((memory_bus[0xFF19] & 0b111) << 8) | memory_bus[0xFF18];
            channel3ShadowFrequency = ((memory_bus[0xFF1E] & 0b111) << 8) | memory_bus[0xFF1D];
        }
};

#endif
