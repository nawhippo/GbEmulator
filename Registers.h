#ifndef REGISTERS_H
#define REGISTERS_H


#include <cstdint>

class Registers {
public:
    Registers(); // Constructor

    struct Flags {
        bool zero;
        bool subtract;
        bool half_carry;
        bool carry;
    };

    uint16_t get16Register(uint8_t indexHigh, uint8_t indexLow);
    uint8_t convertFlagsTo8Bit(Flags &flags);
    void addto16BitRegister(uint8_t high, uint8_t low, int16_t addValue);
    void load16BitRegister(uint8_t high, uint8_t low, uint16_t value);
    void convert8BitToFlags(uint8_t &flags);

private:
    uint8_t registersArr[8]; // a, b, c, d, e, h, l, f
    Flags flagsregister;
};

#endif
