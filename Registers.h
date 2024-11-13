#include <stdint.h>
#ifndef Registers.h
#define Registers.h
class Registers {
    public:
        uint8_t registersArr[7]; //a b c d e f h l
        uint16_t doubleRegistersArr[3]; //bc hl af
        uint8_t accumulator;

    struct FlagsRegister {
        bool zero;
        bool subtract;
        bool half_carry;
        bool carry;
        };
        FlagsRegister flagsregister;

    const int ZERO_FLAG_BYTE_POSITION = 7;
    const int SUBTRACT_FLAG_BYTE_POSITION = 6;
//half carry is to move lower byte carry to a higher byte. 
    const int HALF_CARRY_FLAG_BYTE_POSITION = 5;
    const int CARRY_FLAG_BYTE_POSITION = 4; 



    uint8_t setFlagsRegisterBinary(FlagsRegister flagsRegister) {
    uint8_t num = 0;
     num |= (flagsRegister.zero ? 1 : 0) << ZERO_FLAG_BYTE_POSITION;
     num |= (flagsRegister.subtract ? 1 : 0) << SUBTRACT_FLAG_BYTE_POSITION;
     num |= (flagsRegister.half_carry ? 1 : 0) << HALF_CARRY_FLAG_BYTE_POSITION;
     num |= (flagsRegister.carry ? 1 : 0) << CARRY_FLAG_BYTE_POSITION;
     return num; 
    }


//half-carry set true if overflow from first nibble
    uint8_t setRegisterBinarytoFlags(uint8_t byte){
       //set the flag register back. 0b1 means 1 in binary notation. 
        bool zero = ((byte >> ZERO_FLAG_BYTE_POSITION) & 0b1) != 0;
        bool subtract = ((byte >> SUBTRACT_FLAG_BYTE_POSITION) & 0b1) != 0;
        bool half_carry = ((byte >> HALF_CARRY_FLAG_BYTE_POSITION) & 0b1) != 0;
        bool carry = ((byte >> CARRY_FLAG_BYTE_POSITION) & 0b1) != 0;

        FlagsRegister {
            zero, 
            subtract,
            half_carry, 
            carry
        }
    };

//grabbing all 16 bit registers 

    uint16_t getBC(){
        uint16_t BC = ((static_cast<uint16_t> (this->b) << 8 | static_cast<uint16_t> (this->c)));
        return BC;

    };
    uint16_t getHL(){
        uint16_t HL = ((static_cast<uint16_t> (this->h) << 8 | static_cast<uint16_t> (this->l)));
        return HL;

    };
    
    uint16_t getAF(){
        uint16_t AF = ((static_cast<uint16_t> (this->a) << 8 | static_cast<uint16_t> (this->f)));
        return AF;
    };
    
};

#endif
