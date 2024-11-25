#include <stdint.h>
#ifndef Registers.h
#define Registers.h
class Registers {
    public:
        uint8_t registersArr[6]; //a b c d e h l acc
        uint16_t doubleRegistersArr[3]; //bc hl af 
        // uint8_t accumulator;

    struct FlagsRegister {
        bool zero;
        bool subtract;
        bool half_carry;
        bool carry;
        bool masterinterruptenable;
        };
        FlagsRegister flagsregister;

    struct IORegister {
        uint16_t InterruptEnableRegister;
        uint16_t InterruptFlagRegister;
        uint16_t InterruptMasterEnableRegister;
    };
        IORegister ioregister;


    const int ZERO_FLAG_BYTE_POSITION = 7;
    const int SUBTRACT_FLAG_BYTE_POSITION = 6;
//half carry is to move lower byte carry to a higher byte. 
    const int HALF_CARRY_FLAG_BYTE_POSITION = 5;
    const int CARRY_FLAG_BYTE_POSITION = 4; 
    




//this tis to convert flags to binary and viceversa. 
//     uint8_t setFlagsRegisterBinary(FlagsRegister flagsRegister, uint8_t num) {
//      num |= (flagsRegister.zero ? 1 : 0) << ZERO_FLAG_BYTE_POSITION;
//      num |= (flagsRegister.subtract ? 1 : 0) << SUBTRACT_FLAG_BYTE_POSITION;
//      num |= (flagsRegister.half_carry ? 1 : 0) << HALF_CARRY_FLAG_BYTE_POSITION;
//      num |= (flagsRegister.carry ? 1 : 0) << CARRY_FLAG_BYTE_POSITION;
//      return num; 
//     }


// //half-carry set true if overflow from first nibble
//     uint8_t setRegisterBinarytoFlags(uint8_t byte){
//        //set the flag register back. 0b1 means 1 in binary notation. 
//         bool zero = ((byte >> ZERO_FLAG_BYTE_POSITION) & 0b1) != 0;
//         bool subtract = ((byte >> SUBTRACT_FLAG_BYTE_POSITION) & 0b1) != 0;
//         bool half_carry = ((byte >> HALF_CARRY_FLAG_BYTE_POSITION) & 0b1) != 0;
//         bool carry = ((byte >> CARRY_FLAG_BYTE_POSITION) & 0b1) != 0;



//grabbing all 16 bit registers 
    uint8_t& getA(){
        return this->registersArr[0];
    }

    uint8_t& getB(){
        return this->registersArr[1];
    }


    uint8_t& getC(){
        return this->registersArr[2];
    }

    uint8_t& getD(){
        return this->registersArr[3];
    }

    uint8_t& getE(){
        return this->registersArr[4];
    }

    uint8_t& getF(){
        return this->registersArr[5];
    }


    uint8_t& getH(){
        return this->registersArr[6];
    }

   uint8_t& getL(){
        return this->registersArr[7];
    }



    uint16_t& getBC(){
        return this->doubleRegisterArr[0];
    };
    uint16_t& getHL(){
        return this->doubleRegisterArr[1];
    };
    
    uint16_t& getAF(){
        return this->doubleRegisterArr[2];
    };

    //enable types of interrupts
    //v=blank interrupt handler --= end of vertical blanking 0
    //lcd stat interrupt, when lcd changes 1
    //timer overflow, when the timer reachers zero 2
    //serial transfer interrupt. - ie: connection cable, transfer when data has changed, is ready to transfer, has been transferred 3
    //keypad interrupt 4
    //5-7 reserved
    uint16_t& getInterruptEnableRegister(){
        return this->ioregister->InterruptEnableRegister;
    }
    
    //status of interrupts
    uint16_t& getInterruptFlagRegister(){
        return this->ioregister->InterruptFlagRegister;
    }
    
    //wehter interrupts are allowed at all 
    uint16_t& getInterruptMasterEnableRegister(){
        return this->ioregister->InterruptMasterEnableRegister;
    }
};





#endif
