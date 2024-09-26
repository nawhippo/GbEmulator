#include <Cpu.h>
#include <stdint.h>
#include <Globals.h>
#include <Registers.h>

//these are indices in our register global. 
void movlnton(uint16_t regA, uint16_t regB){


}

void movlntonn(uint16_t regA, uint16_t regB){

}

void movlnnton(uint16_t regA, uint16_t regB){

}

void movlnntonn(uint16_t regA, uint16_t regB){

}

void movlntoHL(uint16_t regA, uint16_t regB){

}



//ADD Operations 
//REG A is the destination

//we have to handle over flow.

//registers should be handled by index?
void addnton(uint8_t regA, uint8_t regB){
    regA = regA + regB;
    return regA; 
}






