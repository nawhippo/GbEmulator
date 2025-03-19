#ifndef SINGLETON_H
#define SINGLETON_H
#include <stdio.h>
#include <cstdint>
#include <algorithm>
#include "Registers.h"

namespace singleton {
class Singleton {
    Registers registers;
    uint8_t ROM[256];
    uint8_t RAM[0xFFFF];
    uint8_t* ROMPTR;
    uint16_t programcounter;
    uint16_t* stackPtr = reinterpret_cast<uint16_t*>(0xFFFF);



//CPU Memory Bus
    struct MemoryBus {
        uint8_t memory[0XFFFF];
    } memorybus;



public:
    static Singleton& getInstance(){
        static Singleton instance;
        return instance;

}

    Singleton(Singleton const&) = delete;
    void operator=(Singleton const&) = delete;

    uint8_t readByte(uint16_t address) {
        return memorybus.memory[address];
    }

    uint16_t getProgramCounter() {
        return programcounter;
    }

    uint16_t* getStackPtr() {
        return stackPtr;
    }

    uint8_t* getROM() {
        return ROM;
    }

    uint8_t* getRAM() {
        return RAM;
    }

    Registers& getRegisters() {
        return registers;
    }

    uint8_t pop() {
        stackPtr++;
        return *stackPtr;
    }

    void push(uint16_t value) {
    // Move the stack pointer down (decrement) before pushing the value
    stackPtr--;

    // Store the value at the current top of the stack
    *stackPtr = value;
    }

void initRAM() {

}



void step(){
    programcounter = readByte(programcounter);

}


private:
    Singleton() {
    }
};

}
#endif
