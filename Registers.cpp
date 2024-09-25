#include <stdint.h>

class Registers {
    public:
        uint8_t a;
        uint8_t b;
        uint8_t c;
        uint8_t d;
        uint8_t e;
        uint8_t f;
        uint8_t h;
        uint8_t l;
        uint16_t BC;
        uint16_t HL;
        uint16_t AF;
        

    struct FlagsRegister {
        bool zero;
        bool subtract;
        bool half_carry;
        bool carry;
        };

    const int ZERO_FLAG_BYTE_POSITION = 7;
    const int SUBTRACT_FLAG_BYTE_POSITION = 6;
    const int HALF_CARRY_FLAG_BYTE_POSITION = 5;
    const int CARRY_FLAG_BYTE_POSITION = 4; 



    uint8_t setFlagsRegisterBinary(FlagsRegister flagsRegister) {
        uint8_t num = 0;
            if (flagsRegister.zero){
                num |= 1 << ZERO_FLAG_BYTE_POSITION;
            } else {
                num |= 0 << ZERO_FLAG_BYTE_POSITION;
            }
            if (flagsRegister.subtract){
                num |= 1 << SUBTRACT_FLAG_BYTE_POSITION;
            } else {
                num |= 0 << SUBTRACT_FLAG_BYTE_POSITION;
            }
            if (flagsRegister.half_carry){
                num |= 1 << HALF_CARRY_FLAG_BYTE_POSITION;
            } else {
                num |= 0 << HALF_CARRY_FLAG_BYTE_POSITION;
            }
            if (flagsRegister.carry){
                num |= 1 << CARRY_FLAG_BYTE_POSITION;
            } else {
                num |= 0 << CARRY_FLAG_BYTE_POSITION;
            }
            return num;
    };


//half-carry set true if overflow from first nibble
    uint8_t setRegisterBinarytoFlags(uint8_t byte){
        //set the flag register back. 
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



    uint16_t getBC(){
        uint16_t HL = ((static_cast<uint16_t> (this->b) << 8 | static_cast<uint16_t> (this->c)));
        return HL;

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