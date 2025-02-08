#include <stdint.h>
#ifndef Registers.h
#define Registers.h
class Registers {
    public:
        uint8_t registersArr[6]; //a b c d e h l acc
        uint16_t doubleRegistersArr[3]; //bc de hl sp 
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
