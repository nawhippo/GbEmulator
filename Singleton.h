#ifndef SINGLETON_H
#define SINGLETON_H
#include <cstdint>
#include <algorithm>
#include "Registers.h"

class Singleton {
Registers registers;
uint8_t ROM[256];

uint8_t RAM[0x10000];

uint8_t* ROMPTR;
int programcounter;

//stack is typically at 0xFFFE to 0xD000
//We decrement and move backward
uint8_t* stackPtr = 0xFFFE;


/*
rom - 0x0000 - 0x7fff up to 4 mb game code assets etc
interal ram - -0xc000 -> 8kb general purpose memory
video ram  - 0x8000 - 0x9fff 8 kb graphics data (tiles, maps, sprites )
external ram - 0xA000 0x8fff  up to 32 kb additional ram for data
i/o registers = 0xff00 - 0xff7f 128 bytes control registers for i/o registers
special ram - 0xff80 0xffe 127 bytes zero page memory  quick accesss ram 
interrupt vectors 0x0040 - 0x0048 8 bytes, memory locations for interrupt handling
boot rom - 0x0000 - 0x00ff 256 bytes - initial boot codes
*/



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

uint8_t& getROM() {
    return ROM;
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
