#ifndef SINGLETON_H
#define SINGLETON_H
#include <cstdint>
#include <algorithm>
#include "Registers.h"

class Singleton {
Registers registers;
uint8_t ROM[256];


uint8_t RAM[256];

uint8_t* ROMPTR;
uint16_t programcounter;


//stack is typically at 0xFFFE to 0xD000
//We decrement and move backward
uint8_t* stackPtr = 0xFFFE;






public:
Singleton(Singleton const&) = delete;
void operator=(Singleton const&) = delete;

static Singleton& getInstance(){
    static Singleton instance;
    return instance;
}
int& getProgramCounter() {
    return programcounter;
}

int& getStackPtr() {
    return stackPtr; 
}


uint8_t* getRAM() {
    return RAM; 
}


Registers& getRegisters() {
    return registers; 
}



private:

Singleton(Register newregister, uint8_t ram[256], uint8_t rom[256]){
    std::copy(rom, rom + 256, ROM);
    register = newregister; 
    std::copy(ram, ram + 256, RAM);
    }

Singleton(){}



};

#endif
