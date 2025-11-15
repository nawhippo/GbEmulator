#ifndef REGISTERS_H
#define REGISTERS_H


#include <cstdint>

class Registers {
public:
    // Named indices for the 8 8-bit registers stored via pointers
    enum RegIndex : uint8_t { REG_A = 0, REG_B = 1, REG_C = 2, REG_D = 3, REG_E = 4, REG_H = 5, REG_L = 6, REG_F = 7 };

    struct Flags {
        bool zero;
        bool subtract;
        bool half_carry;
        bool carry;
    };



    uint8_t* regPtr[8]; // a, b, c, d, e, h, l, f

    uint16_t get16Register(uint8_t indexHigh, uint8_t indexLow) {
        return (static_cast<uint16_t>(*regPtr[indexHigh]) << 8) | *regPtr[indexLow];
    }

    uint8_t convertFlagsTo8Bit(Flags &flags) {
        uint8_t result = 0;
        result |= (flags.zero ?0b10000000 : 0);
        result |= (flags.subtract ? 0b01000000 : 0);
        result |= (flags.half_carry ? 0b00100000 : 0);
        result |= (flags.carry ? 0b00010000 : 0);
        return result;
    }

    void addto16BitRegister(uint8_t high, uint8_t low, int16_t addValue) {
        uint16_t registerValue = get16Register(high, low);
        registerValue = static_cast<uint16_t>(registerValue + addValue);
        *regPtr[high] = static_cast<uint8_t>(registerValue >> 8);
        *regPtr[low] = static_cast<uint8_t>(registerValue & 0xFF);
    }

    void load16BitRegister(uint8_t high, uint8_t low, uint16_t value) {
        *regPtr[high] = static_cast<uint8_t>(value >> 8);
        *regPtr[low] = static_cast<uint8_t>(value & 0xFF);
    }

    void convert8BitToFlags(uint8_t &flags) {
        flagsregister.zero = (flags & 0b10000000) != 0;
        flagsregister.subtract = (flags & 0b01000000) != 0;
        flagsregister.half_carry = (flags & 0b00100000) != 0;
        flagsregister.carry = (flags & 0b00010000) != 0;
    }

    // Map the 8 register bytes to a contiguous region of memory (base must point to at least 8 bytes)
    void mapTo(uint8_t* base) {
        for (int i = 0; i < 8; ++i) regPtr[i] = base + i;
    }

    // Access single 8-bit register by index
    uint8_t get8(uint8_t index) const { return *regPtr[index]; }
    void set8(uint8_t index, uint8_t value) { *regPtr[index] = value; }

    // Convenience named accessors (mapTo should be called first)
    uint8_t getA() const { 
        return get8(REG_A); 
    }
    void setA(uint8_t v) { 
        set8(REG_A, v); 
    }
    uint8_t getB() const { 
        return get8(REG_B); 
    }
    void setB(uint8_t v) { 
        set8(REG_B, v); 
    }
    uint8_t getC() const { 
        return get8(REG_C); 
    }
    void setC(uint8_t v) { 
        set8(REG_C, v); 
    }
    uint8_t getD() const { 
        return get8(REG_D); 
    }
    void setD(uint8_t v) { 
        set8(REG_D, v); 
    }
    uint8_t getE() const { 
        return get8(REG_E); 
    }
    void setE(uint8_t v) { 
        set8(REG_E, v); 
    }
    uint8_t getH() const { 
        return get8(REG_H); 
    }
    void setH(uint8_t v) { 
        set8(REG_H, v); 
    }
    uint8_t getL() const { 
        return get8(REG_L); 
    }
    void setL(uint8_t v) { 
        set8(REG_L, v); 
    }
    uint8_t getF() const { 
        return get8(REG_F); 
    }
    void setF(uint8_t v) { 
        set8(REG_F, v); 
    }
        // Flag bit masks inside the F register (REG_F)
    static constexpr uint8_t FLAG_Z = 0x80; // Zero
    static constexpr uint8_t FLAG_N = 0x40; // Subtract (N)
    static constexpr uint8_t FLAG_H = 0x20; // Half Carry
    static constexpr uint8_t FLAG_C = 0x10; // Carry

    // Read/write flags directly from the F register (which is memory-backed)
    bool getZero() { 
        return regPtr[REG_F] ? ((*regPtr[REG_F] & FLAG_Z) != 0) : false; 
    }
    void setZero(bool v) { if (regPtr[REG_F]) { if (v) *regPtr[REG_F] |= FLAG_Z; else *regPtr[REG_F] &= ~FLAG_Z; } }

    bool getSubtract() const { return regPtr[REG_F] ? ((*regPtr[REG_F] & FLAG_N) != 0) : false; }
    void setSubtract(bool v) { if (regPtr[REG_F]) { if (v) *regPtr[REG_F] |= FLAG_N; else *regPtr[REG_F] &= ~FLAG_N; } }

    bool getHalfCarry() const { return regPtr[REG_F] ? ((*regPtr[REG_F] & FLAG_H) != 0) : false; }
    void setHalfCarry(bool v) { if (regPtr[REG_F]) { if (v) *regPtr[REG_F] |= FLAG_H; else *regPtr[REG_F] &= ~FLAG_H; } }

    bool getCarry() const { return regPtr[REG_F] ? ((*regPtr[REG_F] & FLAG_C) != 0) : false; }
    void setCarry(bool v) { if (regPtr[REG_F]) { if (v) *regPtr[REG_F] |= FLAG_C; else *regPtr[REG_F] &= ~FLAG_C; } }

    // Read the whole F register (upper nibble flags; lower nibble should be zero)
    uint8_t getFlagsByte() const { return regPtr[REG_F] ? *regPtr[REG_F] : 0; }
    void setFlagsByte(uint8_t v) { if (regPtr[REG_F]) *regPtr[REG_F] = v & 0xF0; }
        

    // 16-bit convenience accessors (AF, BC, DE, HL)
    uint16_t getAF() const { 
        return (static_cast<uint16_t>(getA()) << 8) | getF(); 
    }
    void setAF(uint16_t v) { 
        setA(static_cast<uint8_t>(v >> 8)); 
        setF(static_cast<uint8_t>(v & 0xFF)); 
    }
    uint16_t getBC() const { 
        return (static_cast<uint16_t>(getB()) << 8) | getC(); 
    }
    void setBC(uint16_t v) { 
        setB(static_cast<uint8_t>(v >> 8)); setC(static_cast<uint8_t>(v & 0xFF)); 
    }
    uint16_t getDE() const { 
        return (static_cast<uint16_t>(getD()) << 8) | getE(); 
    }
    void setDE(uint16_t v) { 
        setD(static_cast<uint8_t>(v >> 8)); setE(static_cast<uint8_t>(v & 0xFF)); 
    }
    uint16_t getHL() const { 
        return (static_cast<uint16_t>(getH()) << 8) | getL(); 
    }
    void setHL(uint16_t v) { 
        setH(static_cast<uint8_t>(v >> 8)); setL(static_cast<uint8_t>(v & 0xFF)); 
    }

private:
    Flags flagsregister;
};

#endif
