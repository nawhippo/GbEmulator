#include "Singleton.h"
#include "OpcodeHandler.cpp"
const double CLOCK_FREQ = 4194304.0;
const double FRAME_RATE = 59.7275;
const int CYCLES_PER_FRAME = CLOCK_FREQ / FRAME_RATE;


class Clock {
Singleton singleton = Singleton.getInstance();
Registers myRegisters = singleton.getRegisters();
uint16_t* pc = singleton.getProgramCounter();

int execute(){
    bool Running = true;
    while (Running){
        uint8_t instruction = ROM[pc];
        OpcodeHandler::executeInstruction(instruction); 
        pc+=1;
        cycle+=1;
    }
}
};