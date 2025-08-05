
#include <cstdint>


class Registers {
typedef uint16_t (*funcpointer)();
public:
    uint8_t registersArr[8]; //a, b, c, d, e, h, l, f


    struct Flags {
        bool zero;
        bool subtract;
        bool half_carry;
        bool carry;
    } flagsregister;

    //add methods for uint8_t operations here
    uint16_t get16Register(uint8_t indexHigh, uint8_t indexLow){
        return (registersArr[indexHigh] << 8 ) | registersArr[indexLow];
    }

    uint8_t convertFlagsTo8Bit(Flags &flags){
        uint8_t result = 0;
        result |= (flagsregister.zero ? 0b10000000 : 0);
        result |= (flagsregister.subtract ? 0b01000000 : 0);
        result |= (flagsregister.half_carry ? 0b00100000 : 0);
        result |= (flagsregister.carry ? 0b00010000 : 0);
        return result;
    }

    void addto16BitRegister(uint8_t high, uint8_t low, int16_t addValue) {
        uint16_t registerValue = get16Register(high, low);  
        registerValue += addValue;

        registersArr[high] = static_cast<uint8_t>(registerValue >> 8);  
        registersArr[low] = static_cast<uint8_t>(registerValue & 0xFF);
    }


    void load16BitRegister(uint8_t high, uint8_t low, uint16_t value) {
        uint16_t currentValue = get16Register(high, low);
        currentValue = value;

        registersArr[high] = static_cast<uint8_t>(currentValue >> 8);
        registersArr[low] = static_cast<uint8_t>(currentValue & 0xFF);
}


    void convert8BitToFlags(uint8_t &flags){
        flagsregister.zero = (flags & 0b10000000) != 0;
        flagsregister.subtract = (flags & 0b01000000) != 0;
        flagsregister.half_carry = (flags & 0b00100000) != 0;
        flagsregister.carry = (flags & 0b00010000) != 0;
    }
};

