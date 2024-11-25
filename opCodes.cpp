#include <stdint>
#include <cstdlib>
#include "Singleton.h"
#include "Registers.h"
#include <cstddef>
//we need this actually manipulating values in rom
//these are indices in our register global. 


class OpCodes {

Singleton& singletonInstace = Singleton::getInstance();
Registers& reg = singletonInstace.getRegisters(); 
uint8_t * ROM = singletonInstace.getROM();
uint8_t * ROMPTR = singletonInstace.getROMPTR();
uint8_t * stackPtr = singletonInstace.getInstance.getStackPtr;


void pop(){
    this.stackPtr = stackptr += 1;
}

//go forward ie: further down in addresses
void push(void (*func) (int)){
    this.stackPtr = stackptr -= 1;
    stackPtr* = func();
}
 

void nop(){
    return;
}


//ideally we should increment the program counter before we jump instructions.
void jump(){
    singletonInstace.getInstance.ROMPTR = ROM[singletonInstace.getProgramCounter()];
}



void addntonwcarry(int destRegInd, int regAInd, int regBInd, int byteToAdd = -1) {
    if(byteToAdd != -1){
    bool setHalfCarry = false;
    if (reg.registersArr[destRegInd] < 0x0F){
        setHalfCarry = true;
    }
    reg.registersArr[destRegInd] = reg.registersArr[regAInd] + byteToAdd;
    if (reg.registersArr[destRegInd] > 255){
        reg.flagsregister.carry = true;
        reg.registersArr[destRegInd] = reg.registersArr[destRegInd] % 255;
    } 
    if (reg.registersArr[destRegInd] > 0x0F & (setHalfCarry)){
        reg.flagsregister.half_carry = true;
    }
    return;
    }


    bool setHalfCarry = false;
    if (reg.registersArr[destRegInd] < 0x0F){
        setHalfCarry = true;
    }
    reg.registersArr[destRegInd] = reg.registersArr[regAInd] + reg.registersArr[regBInd];
    if (reg.registersArr[destRegInd] > 255){
        reg.flagsregister.carry = true;
        reg.registersArr[destRegInd] = reg.registersArr[destRegInd] % 255;
    } 
    if (reg.registersArr[destRegInd] > 0x0F & (setHalfCarry)){
        reg.flagsregister.half_carry = true;
    }
    return;
}

void addnton(int destRegInd, int regAInd, int regBInd, int byteToAdd = -1) {
    if(byteToAdd != -1){
        reg.registersArr[destRegInd] = reg.registersArr[regAInd] + byteToAdd;
    if (reg.registersArr[destRegInd] > 255){
        reg.registersArr[destRegInd] = reg.registersArr[destRegInd] % 255;
    } 
        return;
    }

    reg.registersArr[destRegInd] = reg.registersArr[regAInd] + reg.registersArr[regBInd];
    if (reg.registersArr[destRegInd] > 255){
        reg.registersArr[destRegInd] = reg.registersArr[destRegInd] % 255;
    } 
    return;
}


void andnton(int regAInd, int regBInd, int byteToAnd = -1){
 if (byteToAnd != -1){
        reg.registersArr[regAInd] = reg.registersArr[regAInd] & byteToAnd;
        return;
    } else {
    reg.registersArr[regAInd] = reg.registersArr[regAInd] & reg.registersArr[regBInd];
    return;
}
}


void xornton(int regAInd, int regBInd, int byteToXor = -1){
 if (byteToXor != -1){
        reg.registersArr[destRegInd] = reg.registersArr[destRegInd] ^ byteToXor;
        return;
    } else {
    reg.registersArr[regAInd] = reg.registersArr[regAInd] ^ reg.registersArr[regBInd];
    return;
}
}



void ornton(int regAInd, int regBInd, int byteToOr = -1){
 if (byteToAdd != -1){
        reg.registersArr[regAInd] = reg.registersArr[destRegInd] | byteToOr;
        return;
    } else {
    reg.registersArr[regAInd] = reg.registersArr[regAInd] | reg.registersArr[regBInd];
    return;
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



void subnton(int destRegInd, int regAInd, int regBInd, int byteToSub = -1) {
    if (byteToSub != -1){
    uint8_t result = reg.registersArr[regAInd] - byteToSub;

    // If there's a borrow (A < B), carry flag is cleared
    if (reg.registersArr[regAInd] < byteToSub) {
        reg.flagsregister.carry = true;  // There was a borrow
    } else {
        reg.flagsregister.carry = false; // No borrow
    }

    // Store the result of the subtraction, considering unsigned wraparound
    reg.registersArr[destRegInd] = result;

    // Set the half-carry flag (borrow from bit 4)
    if ((reg.registersArr[regAInd] & 0xF) < (reg.registersArr[regBInd] & 0xF)) {
        reg.flagsregister.half_carry = true;  
    } else {
        reg.flagsregister.half_carry = false;  
    }

    // Set the Zero flag if result is 0
    if (result == 0) {
        reg.flagsregister.zero = true;
    } else {
        reg.flagsregister.zero = false;
    }

    // Set the Subtract flag since it's a subtraction
    reg.flagsregister.subtract = true;
    }

    /*...........................................................................*/
    /*...........................................................................*/
    /*...........................................................................*/



    uint8_t result = reg.registersArr[regAInd] - reg.registersArr[regBInd];

    // If there's a borrow (A < B), carry flag is cleared
    if (reg.registersArr[regAInd] < reg.registersArr[regBInd]) {
        reg.flagsregister.carry = true;  // There was a borrow
    } else {
        reg.flagsregister.carry = false; // No borrow
    }

    // Store the result of the subtraction, considering unsigned wraparound
    reg.registersArr[destRegInd] = result;

    // Set the half-carry flag (borrow from bit 4)
    if ((reg.registersArr[regAInd] & 0xF) < (reg.registersArr[regBInd] & 0xF)) {
        reg.flagsregister.half_carry = true;  
    } else {
        reg.flagsregister.half_carry = false;  
    }

    // Set the Zero flag if result is 0
    if (result == 0) {
        reg.flagsregister.zero = true;
    } else {
        reg.flagsregister.zero = false;
    }

    // Set the Subtract flag since it's a subtraction
    reg.flagsregister.subtract = true;
}

void subntoncarry(int destRegInd, int regAInd, int regBInd) {
    // Subtract with Carry: take into account the carry flag
    uint8_t carry = reg.flagsregister.carry ? 1 : 0;
    uint8_t result = reg.registersArr[regAInd] - reg.registersArr[regBInd] - carry;

    // If there's a borrow (A - B - Carry < 0), carry flag is cleared
    if (reg.registersArr[regAInd] - reg.registersArr[regBInd] - carry < 0) {
        reg.flagsregister.carry = true;  // There was a borrow
    } else {
        reg.flagsregister.carry = false; // No borrow
    }

    // Store the result of the subtraction
    reg.registersArr[destRegInd] = result;

    // Set the half-carry flag (borrow from bit 4)
    if ((reg.registersArr[regAInd] & 0xF) < ((reg.registersArr[regBInd] & 0xF) + carry)) {
        reg.flagsregister.half_carry = true;  
    } else {
        reg.flagsregister.half_carry = false;  
    }

    // Set the Zero flag if result is 0
    if (result == 0) {
        reg.flagsregister.zero = true;
    } else {
        reg.flagsregister.zero = false;
    }

    // Set the Subtract flag since it's a subtraction
    reg.flagsregister.subtract = true;
}



void subnntonn(int destRegInd, int regAInd, int regBInd) {
    if (reg.doubleRegistersArr[regAInd] < reg.doubleRegistersArr[regBInd]) {
        reg.flagsregister.carry = true; // Set carry flag for underflow
        reg.doubleRegistersArr[destRegInd] = (65536 + reg.doubleRegistersArr[regAInd]) - reg.doubleRegistersArr[regBInd];
    } else {
        reg.flagsregister.carry = false;
        reg.doubleRegistersArr[destRegInd] = reg.doubleRegistersArr[regAInd] - reg.doubleRegistersArr[regBInd];
    }

    if ((reg.doubleRegistersArr[regAInd] & 0xF) < (reg.doubleRegistersArr[regBInd] & 0xF)) {
        reg.flagsregister.half_carry = true; // Half-carry for lower nibble underflow
    } else {
        reg.flagsregister.half_carry = false;
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

void rotaterightn(int destRegInd){
    //should flip the most sig bit, carousel value
    if((reg.registersArr[destRegInd] & 1) == 1){
    reg.registersArr[destRegInd] = reg.registersArr[destRegInd] >> 1 | 128;  
    } else {
        reg.registersArr[destRegInd] = reg.registersArr[destRegInd] >> 1;
    }
}

void rotateleftn(int destRegInd) {
    if((reg.registersArr[destRegInd] & 128) == 128){
    reg.registersArr[destRegInd] = reg.registersArr[destRegInd] << 1 | 1;  
    } else {
        reg.registersArr[destRegInd] = reg.registersArr[destRegInd] << 1;    
}
}


//rotates with carries ie: store overflow in carry flag


void rotaterightncarry(int destRegInd){
    //should flip the most sig bit, carousel value
    if((reg.registersArr[destRegInd] & 1) == 1){
    reg.reg[destRegInd] = reg.registersArr[destRegInd] >> 1;  
    reg.flagsregister.carry = true;
    } else {
        reg.registersArr[destRegInd] = reg.registersArr[destRegInd] >> 1;
    }
}

void rotateleftncarry(int destRegInd) {
    if((reg.registersArr[destRegInd] & 128) == 128){
    reg.registersArr[destRegInd] = reg.registersArr[destRegInd] << 1;  
    reg.flagsregister.carry = true;
    } else {
        reg.registersArr[destRegInd] = reg.registersArr[destRegInd] << 1;    
}
}




void rotaterightnncarry(int destRegInd){
    //should flip the most sig bit
    if((reg.registersArr[destRegInd] & 1) == 1){
    reg.registersArr[destRegInd] = reg.registersArr[destRegInd] >> 1;  
    reg.flagsregister.carry = true;
    } else {
        reg.registersArr[destRegInd] = reg.registersArr[destRegInd] >> 1;
    }
}



void rotateleftnncarry(int destRegInd){
    if((reg.registersArr[destRegInd] & 32768) == 32768){
    reg.registersArr[destRegInd] = reg.registersArr[destRegInd] << 1;  
    reg.flagsregister.carry = true;
    } else {
        reg.registersArr[destRegInd] = reg.registersArr[destRegInd] << 1;    
}
}





// void addntonn(int destRegInd, int regAInd) {
//     uint16_t regBConv = static_cast<uint16_t>(reg.doubleRegisterArr[regAInd]);
//     reg.doubleRegisterArr[destRegInd] = regBConv + reg.doubleRegisterArr[destRegInd];
//     return;
// }

void subnton(int destRegInd, int regAInd, int regBInd) {
    reg.doubleRegisterArr[destRegInd] = reg.doubleRegisterArr[regAInd] - reg.doubleRegisterArr[regBInd];
     if (registerArr[regAInd] < 0){
        reg.registerArr[regAInd] = 255 - std::abs(reg.registerArr[regAInd]);
    }
    return;
}

void incRegn(int destRegInd){
	reg.registersArr[destRegInd] = reg.registersArr[destRegInd] + 1; 
    if (reg.registerArr[destRegInd] > 255){
        reg.registerArr[destRegInd] = reg.registerArr[destRegInd] %= 0x100;
    }
}


void decRegn(int destRegInd){
	reg.registersArr[destRegInd] = reg.registersArr[destRegInd] - 1; 
    if (registerArr[regAInd] < 0){
        reg.registerArr[regAInd] = 255;
    }
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



uint8_t jump(int jumpInd){
ROMPTR = ROM[jumpInd];
}

void cpnton(int destRegInd, int regAInd, int regBInd, int byteToSub = -1){
    //Subtract with Carry: take into account the carry flag
    uint8_t carry = reg.flagsregister.carry ? 1 : 0;
    uint8_t result = reg.registersArr[regAInd] - reg.registersArr[regBInd] - carry;

    //If there's a borrow (A - B - Carry < 0), carry flag is cleared
    if (reg.registersArr[regAInd] - reg.registersArr[regBInd] - carry < 0) {
        reg.flagsregister.carry = true;  //There was a borrow
    } else {
        reg.flagsregister.carry = false; //No borrow
    }

    //Store the result of the subtraction
    //result is not stored, we just set flags. 
    // reg.registersArr[destRegInd] = result;

    //Set the half-carry flag (borrow from bit 4)
    if ((reg.registersArr[regAInd] & 0xF) < ((reg.registersArr[regBInd] & 0xF) + carry)) {
        reg.flagsregister.half_carry = true;  
    } else {
        reg.flagsregister.half_carry = false;  
    }

    //Set the Zero flag if result is 0
    if (result == 0) {
        reg.flagsregister.zero = true;
    } else {
        reg.flagsregister.zero = false;
    }

    //Set the Subtract flag since it's a subtraction
    reg.flagsregister.subtract = true;
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


void reljump(uint16_t offset){
    programCounter = programCounter + offset;
}


void disableInterrupts(){
    //alter section of I/O memory 0x04000000
}


void setbitn(int destRegInd, int bitno){
    int bitOr = 1 << bitno;
    reg.registersArr[destRegInd] = reg.registerArr[destRegInd] | bitOr;
}

void setbitnn(int destRegInd, int bitno){
    int bitOr = 1 << bitno;
    reg.doubleRegistersArr[destRegInd] = reg.doubleRegistersArr[destRegInd] | bitOr;
}


void resetbitn(int destRegInd, int bitno){
    int bitOr = 1 << bitno;
    reg.registersArr[destRegInd] = reg.registerArr[destRegInd] ^ bitOr;
}

void resetbitnn(int destRegInd, int bitno){
    int bitOr = 1 << bitno;
    reg.doubleRegistersArr[destRegInd] = reg.doubleRegistersArr[destRegInd] ^ bitOr;
}




void complementn(int destRegInd){
  reg.registersArr[destRegInd] = ~(reg.regiestersArr[destRegInd]);
}
void complementnn(int destRegInd){
    reg.doubleRegistersArr[destRegInd] = ~(reg.doubleRegistersArr[destRegInd]);
}

void bittestn(int destRegInd, int bitno){
    uint8_t bitAnd = 1 << bitno
    if ((reg.registersArr[destRegInd] & bitAnd) > 0){
        reg.flagsRegister.zero = true;
    }
}

}
