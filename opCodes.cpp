#include <stdint.h>
#include "Singleton.h"
#include "Registers.h"
#include <cstddef>
//we need this actually manipulating values in rom
//these are indices in our register global. 
Singleton& singletonInstace = Singleton::getInstance();
Registers& reg = singletonInstace.getRegisters(); 
uint8_t * ROM = singletonInstace.getROM();
uint8_t * ROMPTR = singletonInstace.getROMPTR();
void nop(){
    return;
}


//ideally we should increment the program counter before we jump instructions.
void jump(){
    singletonInstace.getInstance.ROMPTR = ROM[singletonInstace.getProgramCounter()];
}

void xornandn(int regAInd, int regBInd){
    if(reg.registersArr[regAInd] | reg.registersArr[regBInd]){
        reg.flagregisters = true; 
    }
}

void cpnandn(int regAInd, int regBInd){
     if(reg.registersArr[regAInd] == reg.registersArr[regBInd]){
        reg.flagregisters. = true; 
    }
}

void movlnton(int regAInd, int regBInd){
reg.registersArr[regAInd] = reg.registersArr[regBInd];
return;
}

void movlntonn(int regAInd, int regBInd){
uint16_t regBConv = static_cast<uint16_t> reg.registersArr[regBInd];
reg.registersArr[regAInd] = regBConv;
return;
}

void movlnntonn(int regAInd, int regBInd){
reg.doubleRegistersArr[regAInd] = reg.doubleRegistersArr[regBInd];
return;
}

void addnton(int destRegInd, int regAInd, int regBInd) {
    reg.registersArr[destRegInd] = reg.registersArr[regAInd] + reg.registersArr[regBInd];
    if (reg.registersArr[destRegInd] > 255){
        reg.flagsregister.carry = true;
        reg.registersArr[destRegInd] = reg.registersArr[destRegInd] % 255;
    } 
    if (reg.registersArr[destRegInd] > 15){
        reg.flagsregister.half_carry = true;
    }
    return;
}


void addnntonn(int destRegInd, int regAInd, int regBInd){
    reg.doubleRegistersArr[destRegInd] = reg.doubleRegistersArr[regAInd] + reg.doubleRegistersArr[regBInd];
    if (reg.registersArr[destRegInd] > 65535){
        reg.flagsregister.carry = true;
        reg.registersArr[destRegInd] = reg.registersArr[destRegInd] % 65535;
    }

    if (reg.registersArr[destRegInd] > 15){
        reg.flagsregister.half_carry = true;
    }
}

void addntonacc(int regAInd, int regBInd){
	reg.accumulator = reg.registersArr[regAInd] + reg.registersArr[regBInd];
}

void shiftrightn(int destRegInd){
    reg.registersArr[destRegInd] = reg.registersArr[regAInd] >> 1;  
}

void shiftleftn(int destRegInd){
    reg.registersArr[destRegInd] = reg.registersArr[regAInd] << 1;  
}

void shiftrightnn(int destRegInd){
    reg.doubleRegisterArr[destRegInd] = reg.doubleRegisterArr[destRegInd] >> 1;
}

void shiftleftnn(int destRegInd){
    reg.doubleRegisterArr[destRegInd] = reg.doubleRegisterArr[destRegInd] << 1;
}

//rotate
void rotaterightn(int destRegInd){
    //should flip the most sig bit
    if((reg.registersArr[destRegInd] & 1) == 1){
    reg.registersArr[destRegInd] = reg.registersArr[destRegInd] >> 1 | 255;  
    } else {
        reg.registersArr[destRegInd] = reg.registersArr[destRegInd] >> 1;
    }
}

void rotateleftn(int destRegInd) {
    if((reg.registersArr[destRegInd] & 255) == 255){
    reg.registersArr[destRegInd] = reg.registersArr[destRegInd] << 1 | 1;  
    } else {
        reg.registersArr[destRegInd] = reg.registersArr[destRegInd] << 1;    
}
}

void rotaterightnn(int destRegInd){
    //should flip the most sig bit
    if((reg.registersArr[destRegInd] & 1) == 1){
    reg.registersArr[destRegInd] = reg.registersArr[destRegInd] >> 1 | 65535;  
    } else {
        reg.registersArr[destRegInd] = reg.registersArr[destRegInd] >> 1;
    }
}

void rotateleftnn(int destRegInd){
    if((reg.registersArr[destRegInd] & 65535) == 65535){
    reg.registersArr[destRegInd] = reg.registersArr[destRegInd] << 1 | 1;  
    } else {
        reg.registersArr[destRegInd] = reg.registersArr[destRegInd] << 1;    
}
}


void addntonn(int destRegInd, int regAInd) {
    uint16_t regBConv = static_cast<uint16_t>(reg.doubleRegisterArr[regAInd]);
    reg.doubleRegisterArr[destRegInd] = regBConv + reg.doubleRegisterArr[destRegInd];
    return;
}

void subnton(int destRegInd, int regAInd, int regBInd) {
    reg.doubleRegisterArr[destRegInd] = reg.doubleRegisterArr[regAInd] - reg.doubleRegisterArr[regBInd];
    return;
}

void incRegn(int destRegInd){
	reg.registersArr[regAInd] = reg.registersArr[regAInd] + 1; 
}


void decRegn(int destRegInd){
	reg.registersArr[destRegInd] = reg.registersArr[destRegInd] - 1; 
}

void incRegnn(int destRegInd){
	reg.doubleRegisterArr[destRegInd] = reg.doubleRegisterArr[destRegInd] + 1; 
}

void decRegnn(int destRegInd){
	reg.doubleRegisterArr[destRegInd] = reg.doubleRegisterArr[destRegInd] - 1; 
}

void multnton(int destRegInd, int regAInd, int regBInd){
	reg.doubleRegisterArr[destRegInd] = reg.doubleRegisterArr[regAInd] * reg.doubleRegisterArr[regBInd];
}



uint_8 jump(int jumpInd){
ROMPTR = ROM[jumpInd];
}

void xorregn(int destRegInd, int regAInd, int regBInd){
	reg.registersArr[destRegInd] = reg.registerArr[regAInd] ^ reg.registerArr[regBInd];
}


void andregn (int destRegInd, int regAInd, int regBInd){
	reg.registersArr[destRegInd] = reg.registerArr[regAInd] & reg.registerArr[regBInd];
}


void orregn(int destRegInd, int regAInd, int regBInd){
	reg.registersArr[destRegInd] = reg.registerArr[regAInd] | reg.registerArr[regBInd];
}

void shiftRightRegn(int destRegInd){
    reg.registersArr[destRegInd] = reg.registersArr[destRegInd] >> 1;
}

void shiftLeftRegn(int destRegInd){
    reg.registersArr[destRegInd] = reg.registersArr[destRegInd] << 1;
}

void swapRegn(int destRegInd){
    int left = (reg.registersArr[destRegInd] & 0b00001111) << 4;
    int right = (reg.registersArr[destRegInd] & 0b11110000) >> 4;
    reg.registersArr[destRegInd] = right | left;
}

void pushnn(int destRegInd){
    ROM[stackPtr / 2] = reg.registersArr[destRegInd];
    stackPtr-=2;
}

void popnn(int destRegInd){
    reg.registerArr[destRegInd] = ROM[stackPtr / 2];
    stackPtr+=2;
}


void jump(uint16_t addy){
    programCounter = addy;
}


void reljump(int offset){
    programCounter = programCounter + offset;
}



}
