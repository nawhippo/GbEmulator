#include <cstdint>
#include <string>
#include <unordered_map>
#include <stdio.io>
class OpcodeHandler {
Singleton globals = Singleton.getInstance();
bool& IME = globals.getIME();
Registers myRegisters = globals.getRegisters();
uint16_t& pc = globals.getProgramCounter();
uint8_t* ROM = globals.getROM();
uint8_t* Memory = globals.getMemoryBus();
Clock& clock = Clock.getInstance();


void handleMemoryWrite(uint16_t instruction) {
    uint16_t address = 0;
    uint8_t value = 0;
    
    // Extract address and value based on instruction
    switch (instruction) {
        case 0x02: // LD (BC),A
            address = myRegisters.get16Register(RegisterEnum::B, RegisterEnum::C);
            value = myRegisters.registersArr[RegisterEnum::A];
            break;
        case 0x12: // LD (DE),A
            address = myRegisters.get16Register(RegisterEnum::D, RegisterEnum::E);
            value = myRegisters.registersArr[RegisterEnum::A];
            break;
        case 0x22: // LD (HL+),A
        case 0x32: // LD (HL-),A
        case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: // LD (HL),r
            address = myRegisters.get16Register(RegisterEnum::H, RegisterEnum::L);
            if (instruction >= 0x70 && instruction <= 0x75) {
                value = operandTranslation8bit(instruction & 0x07);
            } else {
                value = myRegisters.registersArr[RegisterEnum::A];
            }
            break;
        case 0x36: // LD (HL),n
            address = myRegisters.get16Register(RegisterEnum::H, RegisterEnum::L);
            value = ROM[pc + 1];
            break;
        case 0x08: // LD (a16),SP
            address = (ROM[pc + 2] << 8) | ROM[pc + 1];
            value = globals.getStackPtr() & 0xFF;
            Memory[address] = value;
            Memory[address + 1] = (globals.getStackPtr() >> 8) & 0xFF;
            return;
        case 0xEA: // LD (a16),A
            address = (ROM[pc + 2] << 8) | ROM[pc + 1];
            value = myRegisters.registersArr[RegisterEnum::A];
            break;
        default:
            return 1; // Not a memory write instruction
    }
    
    // Apply special handling for specific addresses
    if (address == 0xFF13) {
        // Sound Channel 1 Frequency Low
        globals.channel1ShadowFrequency = (globals.channel1ShadowFrequency & 0xFF00) | value;
    } else if (address == 0xFF14) {
        // Sound Channel 1 Frequency High
        globals.channel1ShadowFrequency = (globals.channel1ShadowFrequency & 0x00FF) | ((value & 0x07) << 8);
    } else if (address == 0xFF18) {
        // Sound Channel 2 Frequency Low
        globals.channel2ShadowFrequency = (globals.channel2ShadowFrequency & 0xFF00) | value;
    } else if (address == 0xFF19) {
        // Sound Channel 2 Frequency High
        globals.channel2ShadowFrequency = (globals.channel2ShadowFrequency & 0x00FF) | ((value & 0x07) << 8);
    } else if (address == 0xFF1D) {
        // Sound Channel 3 Frequency Low
        globals.channel3ShadowFrequency = (globals.channel3ShadowFrequency & 0xFF00) | value;
    } else if (address == 0xFF1E) {
        // Sound Channel 3 Frequency High
        globals.channel3ShadowFrequency = (globals.channel3ShadowFrequency & 0x00FF) | ((value & 0x07) << 8);
    } else if (address == 0xFF41) {
        // STAT register - preserve lower 2 bits
        Memory[address] = (Memory[address] & 0b11) | (value & 0xF8);
    } else if (address >= 0x8000 && address <= 0x9FFF) {
        // VRAM - check PPU mode from STAT register
        uint8_t stat = Memory[0xFF41];
        uint8_t mode = stat & 0b11;  // Bits 0-1 contain the mode
        if (mode != 1) {
            // Only accessible when NOT in mode 1 (H-Blank)
            Memory[address] = value;
            return -1;
        }
    } else if (address >= 0xFE00 && address <= 0xFE9F) {
        // OAM - block writes in mode 2
        uint8_t stat = Memory[0xFF41];
        uint8_t mode = stat & 0b11;
        if (mode != 2) {
            Memory[address] = value;
        }
    } else {
        // Default write
        Memory[address] = value;
        return 1;
    }
}

public: 
enum RegisterEnum {
        A = 0, 
        B = 1, 
        C = 2, 
        D = 3, 
        E = 4, 
        H = 5, 
        L = 6, 
        F = 7  
    };

uint8_t& operandTranslation8bit(int translate) {
    switch (translate) {
        case 0b000:
            return myRegisters.registersArr[RegisterEnum::B];
        case 0b001:
            return myRegisters.registersArr[RegisterEnum::C];
        case 0b010:
            return myRegisters.registersArr[RegisterEnum::D];
        case 0b011:
            return myRegisters.registersArr[RegisterEnum::E];
        case 0b100:
            return myRegisters.registersArr[RegisterEnum::H];
        case 0b101:
            return myRegisters.registersArr[RegisterEnum::L];
        case 0b110: {
            uint16_t hlAddress = myRegisters.get16Register(RegisterEnum::H, RegisterEnum::L);
            return globals.ROM[hlAddress];
        }
        case 0b111:
            return myRegisters.registersArr[RegisterEnum::A];
        default:
            throw std::invalid_argument("Invalid 8-bit operand translation");
    }
}

int executeInstruction(uint16_t instruction){
    handleMemoryWrite(static_cast<uint16_t> instruction);
    switch(instruction){
    case 0x00: {
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }
    
    case 0x76: {
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    case 0x07: {
        uint8_t carry = myRegisters.flagsregister.carry;
        uint8_t result = myRegisters.registersArr[RegisterEnum::A] << 1 | carry;
        myRegisters.registersArr[RegisterEnum::A] = result;
        myRegisters.flagsregister.carry = (result >> 7) & 1;
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    case 0x0F: {
        uint8_t carry = myRegisters.flagsregister.carry;
        uint8_t result = myRegisters.registersArr[RegisterEnum::A] >> 1 | (carry << 7);
        myRegisters.registersArr[RegisterEnum::A] = result;
        myRegisters.flagsregister.carry = result & 1;
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    case 0x17: {
        uint8_t carry = myRegisters.flagsregister.carry;
        uint8_t result = (myRegisters.registersArr[RegisterEnum::A] << 1) | carry;
        myRegisters.registersArr[RegisterEnum::A] = result;
        myRegisters.flagsregister.carry = (result >> 7) & 1;
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(12.0));
        break;
    }

    case 0x1F: {
        uint8_t carry = myRegisters.flagsregister.carry;
        uint8_t result = (myRegisters.registersArr[RegisterEnum::A] >> 1) | (carry << 7);
        myRegisters.registersArr[RegisterEnum::A] = result;
        myRegisters.flagsregister.carry = result & 1;
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    case 0x27: {
        uint8_t A = myRegisters.registersArr[RegisterEnum::A];
        if ((A & 0x0F) > 9 || myRegisters.flagsregister.half_carry) {
            A += 0x06;
        }
        if (A > 0x99 || myRegisters.flagsregister.carry) {
            A += 0x60;
            myRegisters.flagsregister.carry = 1;
        } else {
            myRegisters.flagsregister.carry = 0;
        }
        myRegisters.registersArr[RegisterEnum::A] = A;
        myRegisters.flagsregister.half_carry = 0;
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    case 0x2F: {
        myRegisters.registersArr[RegisterEnum::A] = ~myRegisters.registersArr[RegisterEnum::A];
        myRegisters.flagsregister.half_carry = 1;
        myRegisters.flagsregister.carry = 0;
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    case 0x37: {
        myRegisters.flagsregister.carry = 1;
        myRegisters.flagsregister.half_carry = 0;
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    case 0x3F: {
        myRegisters.flagsregister.carry = !myRegisters.flagsregister.carry;
        myRegisters.flagsregister.half_carry = 0;
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    case 0x01: {
        uint8_t lowByte = ROM[pc + 1];
        uint8_t highByte = ROM[pc + 2];
        uint16_t value = (highByte << 8) | lowByte;
        myRegisters.load16BitRegister(RegisterEnum::B, RegisterEnum::C, value);
        pc += 3;
        Clock::getInstance().tick(static_cast<double>(12.0));
        break;
    }

    case 0x11: {
        uint8_t lowByte = ROM[pc + 1];
        uint8_t highByte = ROM[pc + 2];
        uint16_t value = (highByte << 8) | lowByte;
        myRegisters.load16BitRegister(RegisterEnum::D, RegisterEnum::E, value);
        pc += 3;
        Clock::getInstance().tick(static_cast<double>(12.0));
        break;
    }

    case 0x21: {
        uint8_t lowByte = ROM[pc + 1];
        uint8_t highByte = ROM[pc + 2];
        uint16_t value = (highByte << 8) | lowByte;
        myRegisters.load16BitRegister(RegisterEnum::H, RegisterEnum::L, value);
        pc += 3;
        Clock::getInstance().tick(static_cast<double>(12.0));
        break;
    }

    case 0x31: {
        uint8_t lowByte = ROM[pc + 1];
        uint8_t highByte = ROM[pc + 2];
        uint16_t value = (highByte << 8) | lowByte;
        myRegisters.load16BitRegister(RegisterEnum::S, RegisterEnum::P, value);
        pc += 3;
        Clock::getInstance().tick(static_cast<double>(12.0));
        break;
    }

    case 0x0A: {
        uint16_t addr = myRegisters.get16BitRegister(RegisterEnum::B, RegisterEnum::C);
        myRegisters.registersArr[RegisterEnum::A] = ROM[addr];
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(8.0));
        break;
    }

    case 0x1A: {
        uint16_t addr = myRegisters.get16BitRegister(RegisterEnum::D, RegisterEnum::E);
        myRegisters.registersArr[RegisterEnum::A] = ROM[addr];
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(8.0));
        break;
    }

    case 0x2A: {
        uint16_t addr = myRegisters.get16Register(RegisterEnum::H, RegisterEnum::L);
        myRegisters.registersArr[RegisterEnum::A] = Memory[addr];
        myRegisters.addto16BitRegister(RegisterEnum::H, RegisterEnum::L, 1);
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(8.0));
        break;
    }

    case 0x3A: {
        uint16_t addr = myRegisters.get16Register(RegisterEnum::H, RegisterEnum::L);
        myRegisters.registersArr[RegisterEnum::A] = Memory[addr];
        myRegisters.addto16BitRegister(RegisterEnum::H, RegisterEnum::L, -1);
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(8.0));
        break;
    }

    case 0x08: {
        uint16_t addr = (ROM[pc + 1] << 8) | ROM[pc + 2];
        Memory[addr] = globals.getStackPtr() & 0xFF;
        Memory[addr + 1] = (globals.getStackPtr() >> 8) & 0xFF;
        pc += 3;
        Clock::getInstance().tick(static_cast<double>(20.0));
        break;
    }

    case 0x03: {
        myRegisters.modify16BitRegister(RegisterEnum::B, RegisterEnum::C, 1);
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(8.0));
        break;
    }

    case 0x13: {
        myRegisters.modify16BitRegister(RegisterEnum::D, RegisterEnum::E, 1);
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(8.0));
        break;
    }

    case 0x23: {
        myRegisters.modify16BitRegister(RegisterEnum::H, RegisterEnum::L, 1);
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(8.0));
        break;
    }

    case 0x33: {
        globals.getStackPtr()++;
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(8.0));
        break;
    }

    case 0x0B: {
        myRegisters.modify16BitRegister(RegisterEnum::B, RegisterEnum::C, -1);
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(8.0));
        break;
    }

    case 0x1B: {
        myRegisters.modify16BitRegister(RegisterEnum::D, RegisterEnum::E, -1);
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(8.0));
        break;
    }

    case 0x2B: {
        myRegisters.modify16BitRegister(RegisterEnum::H, RegisterEnum::L, -1);
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(8.0));
        break;
    }

    case 0x3B: {
        globals.getStackPtr()--;
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(8.0));
        break;
    }

    case 0x09: {
        uint16_t value = myRegisters.get16Register(RegisterEnum::B, RegisterEnum::C);
        myRegisters.modify16BitRegister(RegisterEnum::H, RegisterEnum::L, value);
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(8.0));
        break;
    }

    case 0x19: {
        uint16_t value = myRegisters.get16Register(RegisterEnum::D, RegisterEnum::E);
        myRegisters.modify16BitRegister(RegisterEnum::H, RegisterEnum::L, value);
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(8.0));
        break;
    }

    case 0x29: {
        uint16_t value = myRegisters.get16Register(RegisterEnum::H, RegisterEnum::L);
        myRegisters.modify16BitRegister(RegisterEnum::H, RegisterEnum::L, value);
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(8.0));
        break;
    }

    case 0x39: {
        uint16_t value = globals.getStackPtr();
        myRegisters.modify16BitRegister(RegisterEnum::H, RegisterEnum::L, value);
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(8.0));
        break;
    }

    case 0x04: {
        myRegisters.registersArr[RegisterEnum::B]++;
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    case 0x0C: {
        myRegisters.registersArr[RegisterEnum::C]++;
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    case 0x14: {
        myRegisters.registersArr[RegisterEnum::D]++;
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    case 0x1C: {
        myRegisters.registersArr[RegisterEnum::E]++;
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    case 0x24: {
        myRegisters.registersArr[RegisterEnum::H]++;
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    case 0x2C: {
        myRegisters.registersArr[RegisterEnum::L]++;
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    case 0x3C: {
        myRegisters.registersArr[RegisterEnum::A]++;
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    case 0x05: {
        myRegisters.registersArr[RegisterEnum::A]--;
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    case 0x0D: {
        myRegisters.registersArr[RegisterEnum::B]--;
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    case 0x15: {
        myRegisters.registersArr[RegisterEnum::C]--;
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    case 0x1D: {
        myRegisters.registersArr[RegisterEnum::D]--;
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    case 0x25: {
        myRegisters.registersArr[RegisterEnum::E]--;
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    case 0x2D: {
        myRegisters.registersArr[RegisterEnum::H]--;
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    case 0x3D: {
        myRegisters.registersArr[RegisterEnum::L]--;
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    case 0x3E: {
        myRegisters.registersArr[RegisterEnum::A] = ROM[pc + 1];
        pc += 2;
        Clock::getInstance().tick(static_cast<double>(8.0));
        break;
    }

    case 0x06: {
        myRegisters.registersArr[RegisterEnum::B] = ROM[pc + 1];
        pc += 2;
        Clock::getInstance().tick(static_cast<double>(8.0));
        break;
    }

    case 0x0E: {
        myRegisters.registersArr[RegisterEnum::C] = ROM[pc + 1];
        pc += 2;
        Clock::getInstance().tick(static_cast<double>(8.0));
        break;
    }

    case 0x16: {
        myRegisters.registersArr[RegisterEnum::D] = ROM[pc + 1];
        pc += 2;
        Clock::getInstance().tick(static_cast<double>(8.0));
        break;
    }

    case 0x1E: {
        myRegisters.registersArr[RegisterEnum::E] = ROM[pc + 1];
        pc += 2;
        Clock::getInstance().tick(static_cast<double>(8.0));
        break;
    }

    case 0x26: {
        myRegisters.registersArr[RegisterEnum::H] = ROM[pc + 1];
        pc += 2;
        Clock::getInstance().tick(static_cast<double>(8.0));
        break;
    }

    case 0x2E: {
        myRegisters.registersArr[RegisterEnum::L] = ROM[pc + 1];
        pc += 2;
        Clock::getInstance().tick(static_cast<double>(8.0));
        break;
    }

    case 0x36: {
        uint16_t addr = myRegisters.get16Register(RegisterEnum::H, RegisterEnum::L);
        Memory[addr] = ROM[pc + 1];
        pc += 2;
        Clock::getInstance().tick(static_cast<double>(12.0));
        break;
    }

    case 0x02: {
        uint16_t addr = myRegisters.get16Register(RegisterEnum::B, RegisterEnum::C);
        Memory[addr] = myRegisters.registersArr[RegisterEnum::A];
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(8.0));
        break;
    }

    case 0x12: {
        uint16_t addr = myRegisters.get16Register(RegisterEnum::D, RegisterEnum::E);
        Memory[addr] = myRegisters.registersArr[RegisterEnum::A];
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(8.0));
        break;
    }

    case 0x22: {
        uint16_t addr = myRegisters.get16Register(RegisterEnum::H, RegisterEnum::L);
        Memory[addr] = myRegisters.registersArr[RegisterEnum::A];
        myRegisters.addto16BitRegister(RegisterEnum::H, RegisterEnum::L, 1);
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(8.0));
        break;
    }

    case 0x32: {
        uint16_t addr = myRegisters.get16Register(RegisterEnum::H, RegisterEnum::L);
        Memory[addr] = myRegisters.registersArr[RegisterEnum::A];
        myRegisters.addto16BitRegister(RegisterEnum::H, RegisterEnum::L, -1);
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(8.0));
        break;
    }

    case 0x40: {
        myRegisters.registersArr[RegisterEnum::B] = myRegisters.registersArr[RegisterEnum::B];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x41: {
        myRegisters.registersArr[RegisterEnum::B] = myRegisters.registersArr[RegisterEnum::C];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x42: {
        myRegisters.registersArr[RegisterEnum::B] = myRegisters.registersArr[RegisterEnum::D];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x43: {
        myRegisters.registersArr[RegisterEnum::B] = myRegisters.registersArr[RegisterEnum::E];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x44: {
        myRegisters.registersArr[RegisterEnum::B] = myRegisters.registersArr[RegisterEnum::H];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x45: {
        myRegisters.registersArr[RegisterEnum::B] = myRegisters.registersArr[RegisterEnum::L];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x46: {
        uint16_t addr = myRegisters.get16Register(RegisterEnum::H, RegisterEnum::L);
        myRegisters.registersArr[RegisterEnum::B] = Memory[addr];
        pc += 1;
        Clock::getInstance().tick(8.0);
        break;
    }

    case 0x47: {
        myRegisters.registersArr[RegisterEnum::B] = myRegisters.registersArr[RegisterEnum::A];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x48: {
        myRegisters.registersArr[RegisterEnum::C] = myRegisters.registersArr[RegisterEnum::B];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x49: {
        myRegisters.registersArr[RegisterEnum::C] = myRegisters.registersArr[RegisterEnum::C];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x4A: {
        myRegisters.registersArr[RegisterEnum::C] = myRegisters.registersArr[RegisterEnum::D];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x4B: {
        myRegisters.registersArr[RegisterEnum::C] = myRegisters.registersArr[RegisterEnum::E];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x4C: {
        myRegisters.registersArr[RegisterEnum::C] = myRegisters.registersArr[RegisterEnum::H];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x4D: {
        myRegisters.registersArr[RegisterEnum::C] = myRegisters.registersArr[RegisterEnum::L];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x4E: {
        uint16_t addr = myRegisters.get16Register(RegisterEnum::H, RegisterEnum::L);
        myRegisters.registersArr[RegisterEnum::C] = Memory[addr];
        pc += 1;
        Clock::getInstance().tick(8.0);
        break;
    }

    case 0x4F: {
        myRegisters.registersArr[RegisterEnum::C] = myRegisters.registersArr[RegisterEnum::A];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x50: {
        myRegisters.registersArr[RegisterEnum::D] = myRegisters.registersArr[RegisterEnum::B];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x51: {
        myRegisters.registersArr[RegisterEnum::D] = myRegisters.registersArr[RegisterEnum::C];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x52: {
        myRegisters.registersArr[RegisterEnum::D] = myRegisters.registersArr[RegisterEnum::D];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x53: {
        myRegisters.registersArr[RegisterEnum::D] = myRegisters.registersArr[RegisterEnum::E];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x54: {
        myRegisters.registersArr[RegisterEnum::D] = myRegisters.registersArr[RegisterEnum::H];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x55: {
        myRegisters.registersArr[RegisterEnum::D] = myRegisters.registersArr[RegisterEnum::L];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x56: {
        uint16_t addr = myRegisters.get16Register(RegisterEnum::H, RegisterEnum::L);
        myRegisters.registersArr[RegisterEnum::D] = Memory[addr];
        pc += 1;
        Clock::getInstance().tick(8.0);
        break;
    }

    case 0x57: {
        myRegisters.registersArr[RegisterEnum::D] = myRegisters.registersArr[RegisterEnum::A];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x58: {
        myRegisters.registersArr[RegisterEnum::E] = myRegisters.registersArr[RegisterEnum::B];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x59: {
        myRegisters.registersArr[RegisterEnum::E] = myRegisters.registersArr[RegisterEnum::C];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x5A: {
        myRegisters.registersArr[RegisterEnum::E] = myRegisters.registersArr[RegisterEnum::D];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x5B: {
        myRegisters.registersArr[RegisterEnum::E] = myRegisters.registersArr[RegisterEnum::E];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x5C: {
        myRegisters.registersArr[RegisterEnum::E] = myRegisters.registersArr[RegisterEnum::H];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x5D: {
        myRegisters.registersArr[RegisterEnum::E] = myRegisters.registersArr[RegisterEnum::L];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x5E: {
        uint16_t addr = myRegisters.get16Register(RegisterEnum::H, RegisterEnum::L);
        myRegisters.registersArr[RegisterEnum::E] = Memory[addr];
        pc += 1;
        Clock::getInstance().tick(8.0);
        break;
    }

    case 0x5F: {
        myRegisters.registersArr[RegisterEnum::E] = myRegisters.registersArr[RegisterEnum::A];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x60: {
        myRegisters.registersArr[RegisterEnum::H] = myRegisters.registersArr[RegisterEnum::B];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x61: {
        myRegisters.registersArr[RegisterEnum::H] = myRegisters.registersArr[RegisterEnum::C];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x62: {
        myRegisters.registersArr[RegisterEnum::H] = myRegisters.registersArr[RegisterEnum::D];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x63: {
        myRegisters.registersArr[RegisterEnum::H] = myRegisters.registersArr[RegisterEnum::E];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x64: {
        myRegisters.registersArr[RegisterEnum::H] = myRegisters.registersArr[RegisterEnum::H];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x65: {
        myRegisters.registersArr[RegisterEnum::H] = myRegisters.registersArr[RegisterEnum::L];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x66: {
        uint16_t addr = myRegisters.get16Register(RegisterEnum::H, RegisterEnum::L);
        myRegisters.registersArr[RegisterEnum::H] = Memory[addr];
        pc += 1;
        Clock::getInstance().tick(8.0);
        break;
    }

    case 0x67: {
        myRegisters.registersArr[RegisterEnum::H] = myRegisters.registersArr[RegisterEnum::A];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x68: {
        myRegisters.registersArr[RegisterEnum::L] = myRegisters.registersArr[RegisterEnum::B];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x69: {
        myRegisters.registersArr[RegisterEnum::L] = myRegisters.registersArr[RegisterEnum::C];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x6A: {
        myRegisters.registersArr[RegisterEnum::L] = myRegisters.registersArr[RegisterEnum::D];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x6B: {
        myRegisters.registersArr[RegisterEnum::L] = myRegisters.registersArr[RegisterEnum::E];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x6C: {
        myRegisters.registersArr[RegisterEnum::L] = myRegisters.registersArr[RegisterEnum::H];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x6D: {
        myRegisters.registersArr[RegisterEnum::L] = myRegisters.registersArr[RegisterEnum::L];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x6E: {
        uint16_t addr = myRegisters.get16Register(RegisterEnum::H, RegisterEnum::L);
        myRegisters.registersArr[RegisterEnum::L] = Memory[addr];
        pc += 1;
        Clock::getInstance().tick(8.0);
        break;
    }

    case 0x6F: {
        myRegisters.registersArr[RegisterEnum::L] = myRegisters.registersArr[RegisterEnum::A];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x70: {
        uint16_t addr = myRegisters.get16Register(RegisterEnum::H, RegisterEnum::L);
        Memory[addr] = myRegisters.registersArr[RegisterEnum::B];
        pc += 1;
        Clock::getInstance().tick(8.0);
        break;
    }

    case 0x71: {
        uint16_t addr = myRegisters.get16Register(RegisterEnum::H, RegisterEnum::L);
        Memory[addr] = myRegisters.registersArr[RegisterEnum::C];
        pc += 1;
        Clock::getInstance().tick(8.0);
        break;
    }

    case 0x72: {
        uint16_t addr = myRegisters.get16Register(RegisterEnum::H, RegisterEnum::L);
        Memory[addr] = myRegisters.registersArr[RegisterEnum::D];
        pc += 1;
        Clock::getInstance().tick(8.0);
        break;
    }

    case 0x73: {
        uint16_t addr = myRegisters.get16Register(RegisterEnum::H, RegisterEnum::L);
        Memory[addr] = myRegisters.registersArr[RegisterEnum::E];
        pc += 1;
        Clock::getInstance().tick(8.0);
        break;
    }

    case 0x74: {
        uint16_t addr = myRegisters.get16Register(RegisterEnum::H, RegisterEnum::L);
        Memory[addr] = myRegisters.registersArr[RegisterEnum::H];
        pc += 1;
        Clock::getInstance().tick(8.0);
        break;
    }

    case 0x75: {
        uint16_t addr = myRegisters.get16Register(RegisterEnum::H, RegisterEnum::L);
        Memory[addr] = myRegisters.registersArr[RegisterEnum::L];
        pc += 1;
        Clock::getInstance().tick(8.0);
        break;
    }

    case 0x78: {
        myRegisters.registersArr[RegisterEnum::A] = myRegisters.registersArr[RegisterEnum::B];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x79: {
        myRegisters.registersArr[RegisterEnum::A] = myRegisters.registersArr[RegisterEnum::C];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x7A: {
        myRegisters.registersArr[RegisterEnum::A] = myRegisters.registersArr[RegisterEnum::D];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x7B: {
        myRegisters.registersArr[RegisterEnum::A] = myRegisters.registersArr[RegisterEnum::E];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x7C: {
        myRegisters.registersArr[RegisterEnum::A] = myRegisters.registersArr[RegisterEnum::H];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x7D: {
        myRegisters.registersArr[RegisterEnum::A] = myRegisters.registersArr[RegisterEnum::L];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x7E: {
        uint16_t addr = myRegisters.get16Register(RegisterEnum::H, RegisterEnum::L);
        myRegisters.registersArr[RegisterEnum::A] = Memory[addr];
        pc += 1;
        Clock::getInstance().tick(8.0);
        break;
    }

    case 0x7F: {
        myRegisters.registersArr[RegisterEnum::A] = myRegisters.registersArr[RegisterEnum::A];
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x80: case 0x81: case 0x82: case 0x83:
    case 0x84: case 0x85: case 0x86: case 0x87: {
        uint8_t value = operandTranslation8bit(instruction & 0x07);
        uint8_t a = myRegisters.registersArr[RegisterEnum::A];
        uint16_t result = a + value;
        myRegisters.registersArr[RegisterEnum::A] = static_cast<uint8_t>(result);
        myRegisters.flagsregister.zero = (myRegisters.registersArr[RegisterEnum::A] == 0);
        myRegisters.flagsregister.subtract = false;
        myRegisters.flagsregister.half_carry = ((a & 0xF) + (value & 0xF)) > 0xF;
        myRegisters.flagsregister.carry = result > 0xFF;
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x88: case 0x89: case 0x8A: case 0x8B:
    case 0x8C: case 0x8D: case 0x8E: case 0x8F: {
        uint8_t value = operandTranslation8bit(instruction & 0x07);
        uint8_t a = myRegisters.registersArr[RegisterEnum::A];
        uint8_t carry = myRegisters.flagsregister.carry ? 1 : 0;
        uint16_t result = a + value + carry;
        myRegisters.registersArr[RegisterEnum::A] = static_cast<uint8_t>(result);
        myRegisters.flagsregister.zero = (myRegisters.registersArr[RegisterEnum::A] == 0);
        myRegisters.flagsregister.subtract = 0;
        myRegisters.flagsregister.half_carry = ((a & 0xF) + (value & 0xF) + carry) > 0xF;
        myRegisters.flagsregister.carry = result > 0xFF;
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x90: case 0x91: case 0x92: case 0x93:
    case 0x94: case 0x95: case 0x96: case 0x97: {
        uint8_t value = operandTranslation8bit(instruction & 0x07);
        uint8_t a = myRegisters.registersArr[RegisterEnum::A];
        uint16_t result = a - value;
        myRegisters.registersArr[RegisterEnum::A] = static_cast<uint8_t>(result);
        myRegisters.flagsregister.zero = (myRegisters.registersArr[RegisterEnum::A] == 0);
        myRegisters.flagsregister.subtract = 1;
        myRegisters.flagsregister.half_carry = (a & 0xF) < (value & 0xF);
        myRegisters.flagsregister.carry = a < value;
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x98: case 0x99: case 0x9A: case 0x9B:
    case 0x9C: case 0x9D: case 0x9E: case 0x9F: {
        uint8_t value = operandTranslation8bit(instruction & 0x07);
        uint8_t a = myRegisters.registersArr[RegisterEnum::A];
        uint8_t carry = myRegisters.flagsregister.carry ? 1 : 0;
        uint16_t result = a - value - carry;
        myRegisters.registersArr[RegisterEnum::A] = static_cast<uint8_t>(result);
        myRegisters.flagsregister.zero = (myRegisters.registersArr[RegisterEnum::A] == 0);
        myRegisters.flagsregister.subtract = 1;
        myRegisters.flagsregister.half_carry = (a & 0xF) < ((value & 0xF) + carry);
        myRegisters.flagsregister.carry = a < (value + carry);
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0xA0: case 0xA1: case 0xA2: case 0xA3:
    case 0xA4: case 0xA5: case 0xA6: case 0xA7: {
        uint8_t value = operandTranslation8bit(instruction & 0x07);
        myRegisters.registersArr[RegisterEnum::A] &= value;
        myRegisters.flagsregister.zero = (myRegisters.registersArr[RegisterEnum::A] == 0);
        myRegisters.flagsregister.subtract = 0;
        myRegisters.flagsregister.half_carry = 1;
        myRegisters.flagsregister.carry = 0;
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0xA8: case 0xA9: case 0xAA: case 0xAB:
    case 0xAC: case 0xAD: case 0xAE: case 0xAF: {
        uint8_t value = operandTranslation8bit(instruction & 0x07);
        myRegisters.registersArr[RegisterEnum::A] ^= value;
        myRegisters.flagsregister.zero = (myRegisters.registersArr[RegisterEnum::A] == 0);
        myRegisters.flagsregister.subtract = 0;
        myRegisters.flagsregister.half_carry = 0;
        myRegisters.flagsregister.carry = 0;
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0xB0: case 0xB1: case 0xB2: case 0xB3:
    case 0xB4: case 0xB5: case 0xB6: case 0xB7: {
        uint8_t value = operandTranslation8bit(instruction & 0x07);
        myRegisters.registersArr[RegisterEnum::A] |= value;
        myRegisters.flagsregister.zero = (myRegisters.registersArr[RegisterEnum::A] == 0);
        myRegisters.flagsregister.subtract = 0;
        myRegisters.flagsregister.half_carry = 0;
        myRegisters.flagsregister.carry = 0;
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0xB8: case 0xB9: case 0xBA: case 0xBB:
    case 0xBC: case 0xBD: case 0xBE: case 0xBF: {
        uint8_t value = operandTranslation8bit(instruction & 0x07);
        uint8_t a = myRegisters.registersArr[RegisterEnum::A];
        uint16_t result = a - value;
        myRegisters.flagsregister.zero = ((uint8_t)result == 0);
        myRegisters.flagsregister.subtract = 1;
        myRegisters.flagsregister.half_carry = (a & 0xF) < (value & 0xF);
        myRegisters.flagsregister.carry = a < value;
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0xC6: {
        uint8_t imm8 = ROM[pc + 1];
        uint8_t a = myRegisters.registersArr[RegisterEnum::A];
        uint16_t result = a + imm8;
        myRegisters.registersArr[RegisterEnum::A] = static_cast<uint8_t>(result);
        myRegisters.flagsregister.zero = (myRegisters.registersArr[RegisterEnum::A] == 0);
        myRegisters.flagsregister.subtract = 0;
        myRegisters.flagsregister.half_carry = ((a & 0xF) + (imm8 & 0xF)) > 0xF;
        myRegisters.flagsregister.carry = result > 0xFF;
        pc += 2;
        Clock::getInstance().tick(8.0);
        break;
    }

    case 0xCE: {
        uint8_t imm8 = ROM[pc + 1];
        uint8_t a = myRegisters.registersArr[RegisterEnum::A];
        uint8_t carry = myRegisters.flagsregister.carry ? 1 : 0;
        uint16_t result = a + imm8 + carry;
        myRegisters.registersArr[RegisterEnum::A] = static_cast<uint8_t>(result);
        myRegisters.flagsregister.zero = (myRegisters.registersArr[RegisterEnum::A] == 0);
        myRegisters.flagsregister.subtract = 0;
        myRegisters.flagsregister.half_carry = ((a & 0xF) + (imm8 & 0xF) + carry) > 0xF;
        myRegisters.flagsregister.carry = result > 0xFF;
        pc += 2;
        Clock::getInstance().tick(8.0);
        break;
    }

    case 0xD6: {
        uint8_t imm8 = ROM[pc + 1];
        uint8_t a = myRegisters.registersArr[RegisterEnum::A];
        uint16_t result = a - imm8;
        myRegisters.registersArr[RegisterEnum::A] = static_cast<uint8_t>(result);
        myRegisters.flagsregister.zero = (myRegisters.registersArr[RegisterEnum::A] == 0);
        myRegisters.flagsregister.subtract = 1;
        myRegisters.flagsregister.half_carry = (a & 0xF) < (imm8 & 0xF);
        myRegisters.flagsregister.carry = a < imm8;
        pc += 2;
        Clock::getInstance().tick(8.0);
        break;
    }

    case 0xDE: {
        uint8_t imm8 = ROM[pc + 1];
        uint8_t a = myRegisters.registersArr[RegisterEnum::A];
        uint8_t carry = myRegisters.flagsregister.carry ? 1 : 0;
        uint16_t result = a - imm8 - carry;
        myRegisters.registersArr[RegisterEnum::A] = static_cast<uint8_t>(result);
        myRegisters.flagsregister.zero = (myRegisters.registersArr[RegisterEnum::A] == 0);
        myRegisters.flagsregister.subtract = 1;
        myRegisters.flagsregister.half_carry = (a & 0xF) < ((imm8 & 0xF) + carry);
        myRegisters.flagsregister.carry = a < (imm8 + carry);
        pc += 2;
        Clock::getInstance().tick(8.0);
        break;
    }

    case 0xE6: {
        uint8_t imm8 = ROM[pc + 1];
        myRegisters.registersArr[RegisterEnum::A] &= imm8;
        myRegisters.flagsregister.zero = (myRegisters.registersArr[RegisterEnum::A] == 0);
        myRegisters.flagsregister.subtract = 0;
        myRegisters.flagsregister.half_carry = 1;
        myRegisters.flagsregister.carry = 0;
        pc += 2;
        Clock::getInstance().tick(8.0);
        break;
    }

    case 0xEE: {
        uint8_t imm8 = ROM[pc + 1];
        myRegisters.registersArr[RegisterEnum::A] ^= imm8;
        myRegisters.flagsregister.zero = (myRegisters.registersArr[RegisterEnum::A] == 0);
        myRegisters.flagsregister.subtract = 0;
        myRegisters.flagsregister.half_carry = 0;
        myRegisters.flagsregister.carry = 0;
        pc += 2;
        Clock::getInstance().tick(8.0);
        break;
    }

    case 0xF6: {
        uint8_t imm8 = ROM[pc + 1];
        myRegisters.registersArr[RegisterEnum::A] |= imm8;
        myRegisters.flagsregister.zero = (myRegisters.registersArr[RegisterEnum::A] == 0);
        myRegisters.flagsregister.subtract = 0;
        myRegisters.flagsregister.half_carry = 0;
        myRegisters.flagsregister.carry = 0;
        pc += 2;
        Clock::getInstance().tick(8.0);
        break;
    }

    case 0xFE: {
        uint8_t imm8 = ROM[pc + 1];
        uint8_t a = myRegisters.registersArr[RegisterEnum::A];
        uint16_t result = a - imm8;
        myRegisters.flagsregister.zero = ((uint8_t)result == 0);
        myRegisters.flagsregister.subtract = 1;
        myRegisters.flagsregister.half_carry = (a & 0xF) < (imm8 & 0xF);
        myRegisters.flagsregister.carry = a < imm8;
        pc += 2;
        Clock::getInstance().tick(8.0);
        break;
    }

    case 0xC7: {
        globals.push(pc + 1);
        pc = 0x00;
        Clock::getInstance().tick(16.0);
        break;
    }

    case 0xCF: {
        globals.push(pc + 1);
        pc = 0x08;
        Clock::getInstance().tick(16.0);
        break;
    }

    case 0xD7: {
        globals.push(pc + 1);
        pc = 0x10;
        Clock::getInstance().tick(16.0);
        break;
    }

    case 0xDF: {
        globals.push(pc + 1);
        pc = 0x18;
        Clock::getInstance().tick(16.0);
        break;
    }

    case 0xE7: {
        globals.push(pc + 1);
        pc = 0x20;
        Clock::getInstance().tick(16.0);
        break;
    }

    case 0xEF: {
        globals.push(pc + 1);
        pc = 0x28;
        Clock::getInstance().tick(16.0);
        break;
    }

    case 0xF7: {
        globals.push(pc + 1);
        pc = 0x30;
        Clock::getInstance().tick(16.0);
        break;
    }

    case 0xFF: {
        globals.push(pc + 1);
        pc = 0x38;
        Clock::getInstance().tick(16.0);
        break;
    }

    case 0xC0: {
        if (!(myRegisters.flagsregister.zero)) {
            pc = globals.pop();
            Clock::getInstance().tick(20.0);
        } else {
            pc += 1;
            Clock::getInstance().tick(8.0);
        }
        break;
    }

    case 0xC8: {
        if (myRegisters.flagsregister.zero) {
            pc = globals.pop();
            Clock::getInstance().tick(20.0);
        } else {
            pc += 1;
            Clock::getInstance().tick(8.0);
        }
        break;
    }

    case 0xD0: {
        if (!(myRegisters.flagsregister.carry)) {
            pc = globals.pop();
            Clock::getInstance().tick(20.0);
        } else {
            pc += 1;
            Clock::getInstance().tick(8.0);
        }
        break;
    }

    case 0xD8: {
        if (myRegisters.flagsregister.carry) {
            pc = globals.pop();
            Clock::getInstance().tick(20.0);
        } else {
            pc += 1;
            Clock::getInstance().tick(8.0);
        }
        break;
    }

    case 0xC9: {
        pc = globals.pop();
        Clock::getInstance().tick(16.0);
        break;
    }

    case 0xD9: {
        pc = globals.pop();
        *IME = 1;
        Clock::getInstance().tick(16.0);
        break;
    }

    case 0xC2: {
        uint16_t addr = (ROM[pc + 2] << 8) | ROM[pc + 1];
        if (!(myRegisters.flagsregister.zero)) {
            pc = addr;
            Clock::getInstance().tick(16.0);
        } else {
            pc += 3;
            Clock::getInstance().tick(12.0);
        }
        break;
    }

    case 0xCA: {
        uint16_t addr = (ROM[pc + 2] << 8) | ROM[pc + 1];
        if (myRegisters.flagsregister.zero) {
            pc = addr;
            Clock::getInstance().tick(16.0);
        } else {
            pc += 3;
            Clock::getInstance().tick(12.0);
        }
        break;
    }

    case 0xD2: {
        uint16_t addr = (ROM[pc + 2] << 8) | ROM[pc + 1];
        if (!(myRegisters.flagsregister.carry)) {
            pc = addr;
            Clock::getInstance().tick(16.0);
        } else {
            pc += 3;
            Clock::getInstance().tick(12.0);
        }
        break;
    }

    case 0xDA: {
        uint16_t addr = (ROM[pc + 2] << 8) | ROM[pc + 1];
        if (myRegisters.flagsregister.carry) {
            pc = addr;
            Clock::getInstance().tick(16.0);
        } else {
            pc += 3;
            Clock::getInstance().tick(12.0);
        }
        break;
    }

    case 0xC3: {
        uint16_t addr = (ROM[pc + 2] << 8) | ROM[pc + 1];
        pc = addr;
        Clock::getInstance().tick(16.0);
        break;
    }

    case 0xE9: {
        pc = myRegisters.get16Register(RegisterEnum::H, RegisterEnum::L);
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0xC4: {
        uint16_t addr = (ROM[pc + 2] << 8) | ROM[pc + 1];
        if (!(myRegisters.flagsregister.zero)) {
            globals.push(pc + 3);
            pc = addr;
            Clock::getInstance().tick(24.0);
        } else {
            pc += 3;
            Clock::getInstance().tick(12.0);
        }
        break;
    }

    case 0xCC: {
        uint16_t addr = (ROM[pc + 2] << 8) | ROM[pc + 1];
        if (myRegisters.flagsregister.zero) {
            globals.push(pc + 3);
            pc = addr;
            Clock::getInstance().tick(24.0);
        } else {
            pc += 3;
            Clock::getInstance().tick(12.0);
        }
        break;
    }

    case 0xD4: {
        uint16_t addr = (ROM[pc + 2] << 8) | ROM[pc + 1];
        if (!(myRegisters.flagsregister.carry)) {
            globals.push(pc + 3);
            pc = addr;
            Clock::getInstance().tick(24.0);
        } else {
            pc += 3;
            Clock::getInstance().tick(12.0);
        }
        break;
    }

    case 0xDC: {
        uint16_t addr = (ROM[pc + 2] << 8) | ROM[pc + 1];
        if (myRegisters.flagsregister.carry) {
            globals.push(pc + 3);
            pc = addr;
            Clock::getInstance().tick(24.0);
        } else {
            pc += 3;
            Clock::getInstance().tick(12.0);
        }
        break;
    }

    case 0xCD: {
        uint16_t addr = (ROM[pc + 2] << 8) | ROM[pc + 1];
        globals.push(pc + 3);
        pc = addr;
        Clock::getInstance().tick(24.0);
        break;
    }

    case 0xC1: {
        uint16_t value = globals.pop();
        myRegisters.set16Register(RegisterEnum::B, RegisterEnum::C, value);
        pc += 1;
        Clock::getInstance().tick(12.0);
        break;
    }

    case 0xD1: {
        uint16_t value = globals.pop();
        myRegisters.set16Register(RegisterEnum::D, RegisterEnum::E, value);
        pc += 1;
        Clock::getInstance().tick(12.0);
        break;
    }

    case 0xE1: {
        uint16_t value = globals.pop();
        myRegisters.load16BitRegister(RegisterEnum::H, RegisterEnum::L, value);
        pc += 1;
        Clock::getInstance().tick(12.0);
        break;
    }

    case 0xF1: {
        uint16_t value = globals.pop();
        myRegisters.load16BitRegister(RegisterEnum::A, RegisterEnum::F, value);
        pc += 1;
        Clock::getInstance().tick(12.0);
        break;
    }

    case 0xC5: {
        uint16_t value = myRegisters.get16Register(RegisterEnum::B, RegisterEnum::C);
        globals.push(value);
        pc += 1;
        Clock::getInstance().tick(16.0);
        break;
    }

    case 0xD5: {
        uint16_t value = myRegisters.get16Register(RegisterEnum::D, RegisterEnum::E);
        globals.push(value);
        pc += 1;
        Clock::getInstance().tick(16.0);
        break;
    }

    case 0xE5: {
        uint16_t value = myRegisters.get16Register(RegisterEnum::H, RegisterEnum::L);
        globals.push(value);
        pc += 1;
        Clock::getInstance().tick(16.0);
        break;
    }

    case 0xF5: {
        uint16_t value = myRegisters.get16Register(RegisterEnum::A, RegisterEnum::F);
        globals.push(value);
        pc += 1;
        Clock::getInstance().tick(16.0);
        break;
    }

    case 0xEA: {
        uint16_t addr = (ROM[pc + 2] << 8) | ROM[pc + 1];
        Memory[addr] = myRegisters.registersArr[RegisterEnum::A];
        pc += 3;
        Clock::getInstance().tick(16.0);
        break;
    }

    case 0xFA: {
        uint16_t addr = (ROM[pc + 2] << 8) | ROM[pc + 1];
        myRegisters.registersArr[RegisterEnum::A] = Memory[addr];
        pc += 3;
        Clock::getInstance().tick(16.0);
        break;
    }

    case 0x18: {
        int8_t offset = static_cast<int8_t>(ROM[pc + 1]);
        pc += 2 + offset;
        Clock::getInstance().tick(12.0);
        break;
    }

    case 0x20: {
        int8_t offset = static_cast<int8_t>(ROM[pc + 1]);
        if (!(myRegisters.flagsregister.zero)) {
            pc += 2 + offset;
            Clock::getInstance().tick(12.0);
        } else {
            pc += 2;
            Clock::getInstance().tick(8.0);
        }
        break;
    }

    case 0x28: {
        int8_t offset = static_cast<int8_t>(ROM[pc + 1]);
        if (myRegisters.flagsregister.zero) {
            pc += 2 + offset;
            Clock::getInstance().tick(12.0);
        } else {
            pc += 2;
            Clock::getInstance().tick(8.0);
        }
        break;
    }

    case 0x30: {
        int8_t offset = static_cast<int8_t>(ROM[pc + 1]);
        if (!(myRegisters.flagsregister.carry)) {
            pc += 2 + offset;
            Clock::getInstance().tick(12.0);
        } else {
            pc += 2;
            Clock::getInstance().tick(8.0);
        }
        break;
    }

    case 0x38: {
        int8_t offset = static_cast<int8_t>(ROM[pc + 1]);
        if (myRegisters.flagsregister.carry) {
            pc += 2 + offset;
            Clock::getInstance().tick(12.0);
        } else {
            pc += 2;
            Clock::getInstance().tick(8.0);
        }
        break;
    }

    case 0xF3: {
        *IME = 0;
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0xFB: {
        *IME = 1;
        pc += 1;
        Clock::getInstance().tick(4.0);
        break;
    }

    case 0x10: {
        pc += 2;
        Clock::getInstance().tick(4.0);
        break;
    }
    if (IME && (Memory[0xFF0F] & Memory[0xFFFF])) {
    uint8_t triggered = Memory[0xFF0F] & Memory[0xFFFF];
    
    // Check interrupts in priority order (VBlank is highest)
    if (triggered & 0x01) {  // VBlank
        IME = false;
        globals.push(pc);
        pc = 0x0040;
        Memory[0xFF0F] &= ~0x01;  
        Clock::getInstance().tick(20.0);
    } else if (triggered & 0x02) {  // LCD STAT
        IME = false;
        globals.push(pc);
        pc = 0x0048;
        Memory[0xFF0F] &= ~0x02;
        Clock::getInstance().tick(20.0);
    } else if (triggered & 0x04) {  // Timer
        IME = false;
        globals.push(pc);
        pc = 0x0050;
        Memory[0xFF0F] &= ~0x04;
        Clock::getInstance().tick(20.0);
    } else if (triggered & 0x08) {  // Serial
        IME = false;
        globals.push(pc);
        pc = 0x0058;
        Memory[0xFF0F] &= ~0x08;
        Clock::getInstance().tick(20.0);
    } else if (triggered & 0x10) {  // Joypad
        IME = false;
        globals.push(pc);
        pc = 0x0060;
        Memory[0xFF0F] &= ~0x10;
        Clock::getInstance().tick(20.0);
    }
}
    default:
        pc += 1;
        break;
    }
    return 0;
}


}
