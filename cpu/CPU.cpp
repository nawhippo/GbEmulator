#include "Singleton.h"
#include "OpcodeHandler.cpp"
class CPU {
Singleton singleton = Singleton.getInstance();
Registers myRegisters = singleton.getRegisters();
uint16_t& pc = singleton.getProgramCounter();
uint8_t* ROM = singleton.getROM();
uint8_t* RAM = singleton.getRAM();
uint16_t* stackPtr = singleton.getStackPtr();
uint8_t& IE = singleton.getInterruptEnable();
uint8_t* IF = singleton.getIORegisters() + 0x0F; 
int cycle = 0;

int execute(){
    bool Running = true;
    while (Running){
        uint8_t instruction = ROM[pc];
        executeInstruction(instruction);
        if 
        cycle+=1;
    }

}
};