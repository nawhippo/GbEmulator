#include <cstdint>
#include <string>
#include <unordered_map>

class OpcodeHandler {
Singleton globals = Singleton.getInstance();
bool& IME = globals.getIME();
Registers myRegisters = globals.getRegisters();
uint16_t& pc = globals.getProgramCounter();
uint8_t* ROM = globals.getROM();
uint8_t* Memory = globals.getMemoryBus();
Clock* clock = Clock.getInstance();

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


// r8 (8-bit) operand translation
uint8_t& operandTranslation8bit(int translate) {
    switch (translate) {
        case 0b000: // B
            return myRegisters.registersArr[RegisterEnum::B];
        case 0b001: // C
            return myRegisters.registersArr[RegisterEnum::C];
        case 0b010: // D
            return myRegisters.registersArr[RegisterEnum::D];
        case 0b011: // E
            return myRegisters.registersArr[RegisterEnum::E];
        case 0b100: // H
            return myRegisters.registersArr[RegisterEnum::H];
        case 0b101: // L
            return myRegisters.registersArr[RegisterEnum::L];
        case 0b110: { // (HL)
            uint16_t hlAddress = myRegisters.get16Register(RegisterEnum::H, RegisterEnum::L);
            return globals.getRAM()[hlAddress];
        }
        case 0b111: // A
            return myRegisters.registersArr[RegisterEnum::A];
        default:
            throw std::invalid_argument("Invalid 8-bit operand translation");
    }
}

//insturction could be 8 bit 16 bit or 24 bit
int executeInstruction(uint16_t instruction){
    switch(instruction){
    //1 byte instructional sets
    //NOP
    case (instruction == 0x00){
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0)); // was 1 -> now 4 clocks
        break;
    }
    //HALT
    case (instruction == 0x76){
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

 // Rotate A left (RL)
    case (instruction == 0b00000111): {
        uint8_t carry = flagsregister.carry;
        // Rotate the A myRegisters left, shifting bit 7 into the carry and bit 0 into A
        uint8_t result = registersArr[RegisterEnum::A] << 1 | carry;
        
        // Update the A myRegisters with the result
        registersArr[RegisterEnum::A] = result;
        
        // Update carry flag based on the old bit 7 of A
        flagsregister.carry = (registersArr[RegisterEnum::A] >> 7) & 1;
        
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    // Rotate A right (RR)
    case (instruction == 0b00001111): {
        uint8_t carry = flagsregister.carry;
        // Rotate the A myRegisters right, shifting bit 0 into the carry and bit 7 into A
        //this is 
        uint8_t result = registersArr[RegisterEnum::A] >> 1 | (carry << 7);
        
        // Update the A myRegisters with the result
        registersArr[RegisterEnum::A] = result;
        
        // Update carry flag based on the old bit 0 of A
        flagsregister.carry = registersArr[RegisterEnum::A] & 1;
        
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    // Rotate A left through carry (RLA)
    case (instruction == 0b00010111): {
        uint8_t carry = flagsregister.carry;
        // Rotate the A myRegisters left through carry
        uint8_t result = (registersArr[RegisterEnum::A] << 1) | carry;
        
        // Update the A myRegisters with the result
        registersArr[RegisterEnum::A] = result;
        
        // Update the carry flag based on the old bit 7 of A
        flagsregister.carry = (registersArr[RegisterEnum::A] >> 7) & 1;
        
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(12.0)); // RLA = 3 machine cycles -> 12 clocks
        break;
    }

    // Rotate A right through carry (RRA)
    case (instruction == 0b00011111): {
        uint8_t carry = flagsregister.carry;
        // Rotate the A myRegisters right through carry
        uint8_t result = (registersArr[RegisterEnum::A] >> 1) | (carry << 7);
        
        // Update the A myRegisters with the result
        registersArr[RegisterEnum::A] = result;
        
        // Update the carry flag based on the old bit 0 of A
        flagsregister.carry = registersArr[RegisterEnum::A] & 1;
        
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    // Decimal Adjust Accumulator (DAA)
    case (instruction == 0b00100111): {
        uint8_t A = registersArr[RegisterEnum::A];
        
        if ((A & 0x0F) > 9 || flagsregister.half_carry) {
            A += 0x06;
        }
        if (A > 0x99 || flagsregister.carry) {
            A += 0x60;
            flagsregister.carry = 1;
        } else {
            flagsregister.carry = 0;
        }
        
        // Update the A myRegisters with the result
        registersArr[RegisterEnum::A] = A;
        
        // Reset the half-carry flag
        flagsregister.half_carry = 0;
        
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    // Complement (CPL)
    case (instruction == 0b00101111): {
        // Complement all the bits in A
        registersArr[RegisterEnum::A] = ~registersArr[RegisterEnum::A];
        
        // Set the half-carry flag
        flagsregister.half_carry = 1;
        
        // Clear the carry flag
        flagsregister.carry = 0;
        
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    // Set Carry Flag (SCF)
    case (instruction == 0b00110111): {
        // Set the carry flag
        flagsregister.carry = 1;
        
        // Clear the half-carry flag
        flagsregister.half_carry = 0;
        
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }

    // Complement Carry Flag (CCF)
    case (instruction == 0b00111111): {
        // Complement the carry flag
        flagsregister.carry = !flagsregister.carry;
        
        // Clear the half-carry flag
        flagsregister.half_carry = 0;
        
        pc += 1;
        Clock::getInstance().tick(static_cast<double>(4.0));
        break;
    }
}

//LD r16, imm16

//BC
case (instruction == 0b00000001): {
    uint8_t lowByte = ROM[pc + 1];
    uint8_t highByte = ROM[pc + 2];
    uint16_t nextInstruction = (highByte << 8) | lowByte;
    myRegisters.load16BitRegister(RegisterEnum::B, RegisterEnum::C, nextInstruction);
    pc += 3; // 1 byte for opcode + 2 bytes for imm16
    Clock::getInstance().tick(static_cast<double>(12.0)); // 3 machine cycles -> 12 clocks
    break;
}

//DE
case (instruction == 0b00010001): {
    uint8_t lowByte = ROM[pc + 1];
    uint8_t highByte = ROM[pc + 2];
    uint16_t nextInstruction = (highByte << 8) | lowByte;
    myRegisters.load16BitRegister(RegisterEnum::D, RegisterEnum::E, nextInstruction);
    pc += 3; // 1 byte for opcode + 2 bytes for imm16
    Clock::getInstance().tick(static_cast<double>(12.0)); // 3 machine cycles -> 12 clocks
    break;
}

//HL
case (instruction == 0b00100001): {
    uint8_t lowByte = ROM[pc + 1];
    uint8_t highByte = ROM[pc + 2];
    uint16_t nextInstruction = (highByte << 8) | lowByte;
    myRegisters.load16BitRegister(RegisterEnum::H, RegisterEnum::L, nextInstruction);
    pc += 3; // 1 byte for opcode + 2 bytes for imm16
    Clock::getInstance().tick(static_cast<double>(12.0)); // 3 machine cycles -> 12 clocks
    break;
}

//SP
case (instruction == 0b00110001): {
    uint8_t lowByte = ROM[pc + 1];
    uint8_t highByte = ROM[pc + 2];
    uint16_t nextInstruction = (highByte << 8) | lowByte;
    myRegisters.load16BitRegister(RegisterEnum::S, RegisterEnum::P, nextInstruction);
    pc += 3; // 1 byte for opcode + 2 bytes for imm16
    Clock::getInstance().tick(static_cast<double>(12.0)); // 3 machine cycles -> 12 clocks
    break;
}



// LD A, [r16mem]

// BC
case (instruction == 0b00001010): {
    uint16_t addy = myRegisters.get16BitRegister(RegisterEnum::B, RegisterEnum::C);
    myRegisters.registersArr[6] = RAM[addy];
    pc += 1; // Move past the opcode
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

// DE
case (instruction == 0b00011010): {
    uint16_t addy = myRegisters.get16BitRegister(RegisterEnum::D, RegisterEnum::E);
    myRegisters.registersArr[6] = RAM[addy];
    pc += 1; // Move past the opcode
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

// HL
case (instruction == 0b00101010): {
    uint16_t addy = myRegisters.get16BitRegister(RegisterEnum::H, RegisterEnum::L);
    myRegisters.registersArr[6] = RAM[addy];
    pc += 1; // Move past the opcode
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

// AF
case (instruction == 0b00111010): {
    uint16_t addy = myRegisters.get16BitRegister(RegisterEnum::A, RegisterEnum::F); 
    myRegisters.registersArr[6] = RAM[addy];
    pc += 1; // Move past the opcode
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}



//ld [imm16], sp
    case (instruction == 0b00001000): {
    uint16_t first = RAM[pc + 1] << 8;
    uint16_t second = RAM[pc + 2];
    uint16_t addy = first | second; 
    RAM[addy] = globals.stackPtr & 0xFF;  
    RAM[addy + 1] = (globals.stackPtr >> 8) & 0xFF;

    pc += 3;  
    Clock::getInstance().tick(static_cast<double>(12.0)); // 3 machine cycles -> 12 clocks
    break;
}



// INC r16

case (instruction == 0b00000011): {
    myRegisters.modify16BitRegister(RegisterEnum::B, RegisterEnum::C, 1);
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

case (instruction == 0b00010011): {
    myRegisters.modify16BitRegister(RegisterEnum::D, RegisterEnum::E, 1);
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

case (instruction == 0b00100011): {
    myRegisters.modify16BitRegister(RegisterEnum::H, RegisterEnum::L, 1);
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

case (instruction == 0b00110011): {
    myRegisters.modify16BitRegister(RegisterEnum::A, RegisterEnum::F, 1);
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

//dec r16 
case(instruction == 0b0000001011): {
    myRegisters.modify16BitRegister(RegisterEnum::B, RegisterEnum::C, -1);
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

case(instruction == 0b0001101011): {
    myRegisters.modify16BitRegister(RegisterEnum::D, RegisterEnum::E, -1);
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

case(instruction == 0b0010101011): {
    myRegisters.modify16BitRegister(RegisterEnum::H, RegisterEnum::L, -1);
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

case(instruction == 0b0011101011): {
    myRegisters.modify16BitRegister(RegisterEnum::A, RegisterEnum::F, -1);
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

//add hl, r16
case(instruction == 0b0000001001): {
    uint16_t valueToAdd = myRegisters.get16Register(RegisterEnum::B, RegisterEnum::C);
    myRegisters.modify16BitRegister(RegisterEnum::H, RegisterEnum::L, valueToAdd);
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

case(instruction == 0b0001101001): {
    uint16_t valueToAdd = myRegisters.get16Register(RegisterEnum::D, RegisterEnum::E);
    myRegisters.modify16BitRegister(RegisterEnum::H, RegisterEnum::L, valueToAdd);
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

case(instruction == 0b0010101001): {
    uint16_t valueToAdd = myRegisters.get16Register(RegisterEnum::H, RegisterEnum::L);
    myRegisters.modify16BitRegister(RegisterEnum::H, RegisterEnum::L, valueToAdd);
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

case(instruction == 0b0011101001): {
    uint16_t valueToAdd = myRegisters.get16Register(RegisterEnum::A, RegisterEnum::F);
    myRegisters.modify16BitRegister(RegisterEnum::H, RegisterEnum::L, valueToAdd);
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

    //inc r8
case(instruction == 0b000000100): {
    myRegisters.registersArr[RegisterEnum::B] += 1;
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

case(instruction == 0b000001100): {
    myRegisters.registersArr[RegisterEnum::C] += 1;
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

case(instruction == 0b000010100): {
    myRegisters.registersArr[RegisterEnum::D] += 1;
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

case(instruction == 0b000011100): {
    myRegisters.registersArr[RegisterEnum::E] += 1;
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

case(instruction == 0b000100100): {
    myRegisters.registersArr[RegisterEnum::H] += 1;
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

case(instruction == 0b000101100): {
    myRegisters.registersArr[RegisterEnum::L] += 1;
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

case(instruction == 0b000111100): {
    myRegisters.registersArr[RegisterEnum::F] += 1;
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

//dec r8
case(instruction == 0b000000101): {
    myRegisters.registersArr[RegisterEnum::A] -= 1;
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

case(instruction == 0b000001101): {
    myRegisters.registersArr[RegisterEnum::B] -= 1;
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

case(instruction == 0b000010101): {
    myRegisters.registersArr[RegisterEnum::C] -= 1;
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

case(instruction == 0b000011101): {
    myRegisters.registersArr[RegisterEnum::D] -= 1;
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

case(instruction == 0b000100101): {
    myRegisters.registersArr[RegisterEnum::E] -= 1;
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

case(instruction == 0b000101101): {
    myRegisters.registersArr[RegisterEnum::H] -= 1;
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

case(instruction == 0b000111101): {
    myRegisters.registersArr[RegisterEnum::L] -= 1;
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}


//ld r8, imm8
case(instruction == 0b000001110): {
    myRegisters.registersArr[RegisterEnum::A] = ROM[pc + 1];
    pc += 2;
    Clock::getInstance().tick(static_cast<double>(8.0)); // was 2 -> now 8 clocks
    break;
}

case(instruction == 0b000010110): {
    myRegisters.registersArr[RegisterEnum::B] = ROM[pc + 1];
    pc += 2;
    Clock::getInstance().tick(static_cast<double>(8.0)); // was 2 -> now 8 clocks
    break;
}

case(instruction == 0b000011110): {
    myRegisters.registersArr[RegisterEnum::C] = ROM[pc + 1];
    pc += 2;
    Clock::getInstance().tick(static_cast<double>(8.0)); // was 2 -> now 8 clocks
    break;
}

case(instruction == 0b000100110): {
    myRegisters.registersArr[RegisterEnum::D] = ROM[pc + 1];
    pc += 2;
    Clock::getInstance().tick(static_cast<double>(8.0)); // was 2 -> now 8 clocks
    break;
}

case(instruction == 0b000101110): {
    myRegisters.registersArr[RegisterEnum::E] = ROM[pc + 1];
    pc += 2;
    Clock::getInstance().tick(static_cast<double>(8.0)); // was 2 -> now 8 clocks
    break;
}

case(instruction == 0b000111110): {
    myRegisters.registersArr[RegisterEnum::H] = ROM[pc + 1];
    pc += 2;
    Clock::getInstance().tick(static_cast<double>(8.0)); // was 2 -> now 8 clocks
    break;
}



// Add to accumulator
case ((instruction & 0xF8) == 0b10000000): {  // 0x80 -> 10000000
    int a = operandTranslation(instruction >> 6);  // Get the operand
    myRegisters.registersArr[RegisterEnum::A] += a;  // Add operand to the accumulator
    myRegisters.flagsregister.zero = (myRegisters.registersArr[RegisterEnum::A] == 0);
    myRegisters.flagsregister.subtract = false;
    myRegisters.flagsregister.half_carry = ((myRegisters.registersArr[RegisterEnum::A] & 0x0F) < (a & 0x0F));
    myRegisters.flagsregister.carry = (myRegisters.registersArr[RegisterEnum::A] > 0xFF);
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

// Add with carry
case ((instruction & 0xF8) == 0b10000110): {  // 0x86 -> 10000110
    int a = operandTranslation(instruction >> 6);  // Get the operand
    uint16_t result = myRegisters.registersArr[RegisterEnum::A] + a + (myRegisters.flagsregister.carry ? 1 : 0);  // Add with carry
    myRegisters.registersArr[RegisterEnum::A] = result & 0xFF;  // Keep only the lower 8 bits
    myRegisters.flagsregister.zero = (myRegisters.registersArr[RegisterEnum::A] == 0);
    myRegisters.flagsregister.subtract = false;
    myRegisters.flagsregister.half_carry = ((myRegisters.registersArr[RegisterEnum::A] & 0x0F) < (a & 0x0F) + (myRegisters.flagsregister.carry ? 1 : 0));
    myRegisters.flagsregister.carry = (result > 0xFF);  // Carry flag if overflow
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

// Subtract from accumulator
case ((instruction & 0xF8) == 0b10001010): {  // 0x8A -> 10001010
    int a = operandTranslation(instruction >> 6);  // Get the operand
    myRegisters.registersArr[RegisterEnum::A] -= a;  // Subtract operand from the accumulator
    myRegisters.flagsregister.zero = (myRegisters.registersArr[RegisterEnum::A] == 0);
    myRegisters.flagsregister.subtract = true;
    myRegisters.flagsregister.half_carry = ((myRegisters.registersArr[RegisterEnum::A] & 0x0F) > (a & 0x0F));  // Check for half-carry
    myRegisters.flagsregister.carry = (myRegisters.registersArr[RegisterEnum::A] > 0xFF);  // Carry flag set if result is negative
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

// Subtract with carry
case ((instruction & 0xF8) == 0b10001110): {  // 0x8E -> 10001110
    int a = operandTranslation(instruction >> 6);  // Get the operand
    uint16_t result = myRegisters.registersArr[RegisterEnum::A] - a - (myRegisters.flagsregister.carry ? 1 : 0);  // Subtract with carry
    myRegisters.registersArr[RegisterEnum::A] = result & 0xFF;  // Keep only the lower 8 bits
    myRegisters.flagsregister.zero = (myRegisters.registersArr[RegisterEnum::A] == 0);
    myRegisters.flagsregister.subtract = true;
    myRegisters.flagsregister.half_carry = ((myRegisters.registersArr[RegisterEnum::A] & 0x0F) > (a & 0x0F) + (myRegisters.flagsregister.carry ? 1 : 0));
    myRegisters.flagsregister.carry = (result > 0xFF);  // Carry flag if underflow
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

// AND with accumulator
case ((instruction & 0xF8) == 0b10100110): {  // 0xA6 -> 10100110
    int a = operandTranslation(instruction >> 6);  // Get the operand
    myRegisters.registersArr[RegisterEnum::A] &= a;  // Perform AND operation with accumulator
    myRegisters.flagsregister.zero = (myRegisters.registersArr[RegisterEnum::A] == 0);
    myRegisters.flagsregister.subtract = false;
    myRegisters.flagsregister.half_carry = true;  // Half-carry is always set for AND
    myRegisters.flagsregister.carry = false;  // No carry for AND
    pc += 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}



    //ret conds
    //0 nz
    //1 z
    //2 nc
    //3 c
    // RET nz (return if zero flag is not set)
case((instruction) == 0b11000000): {
    if (!(myRegisters.flagsregister.zero)) {  // Check if zero flag is not set
        uint16_t addr = globals.pop();  // Pop address from stack
        *pc = addr;
    }
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

// RET z (return if zero flag is set)
case((instruction) == 0b11001000): {
    if (myRegisters.flagsregister.zero) {  // Check if zero flag is set
        uint16_t addr = globals.pop();  // Pop address from stack
        *pc = addr;
    }
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

// RET nc (return if carry flag is not set)
case((instruction) == 0b11010000): {
    if (!(myRegisters.flagsregister.carry)) {  // Check if carry flag is not set
        uint16_t addr = globals.pop();  // Pop address from stack
        *pc = addr;
    }
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

// RET c (return if carry flag is set)
case((instruction) == 0b11011000): {
    if (myRegisters.flagsregister.carry) {  // Check if carry flag is set
        uint16_t addr = globals.pop();
        *pc = addr; 
    }
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

// RET (unconditional return)
case((instruction) == 0b11001001): {  // 0xC9 for RET
    uint16_t addr = globals.pop();  // Pop address from stack
    *pc = addr; 
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

// RETI (return from interrupt)
case((instruction) == 0b11011001): {  // 0xD9 for RETI
    uint16_t addr = globals.pop();  // Pop address from stack
    *pc = addr;
    *IME = 1;
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}




    //jp cond, imm16

 // Jump with condition and 16-bit address (JP cond, imm16)

// Jump if zero flag is not set
case((instruction) == 0b11000010): {
    uint16_t nextInstruction = ROM[pc + 1] | (ROM[pc + 2] << 8);  // Read 16-bit address
    if (!(myRegisters.flagsregister.zero)) {
        opcodes.jump(nextInstruction);  // Jump to address
    }
    pc += 3;  // Move the program counter by 3 (2 bytes for the address)
    Clock::getInstance().tick(static_cast<double>(12.0));
    break;
}

// Jump if zero flag is set
case((instruction) == 0b11001010): {
    uint16_t nextInstruction = ROM[pc + 1] | (ROM[pc + 2] << 8);  // Read 16-bit address
    if (myRegisters.flagsregister.zero) {
        opcodes.jump(nextInstruction);  // Jump to address
    }
    pc += 3;  // Move the program counter by 3 (2 bytes for the address)
    Clock::getInstance().tick(static_cast<double>(12.0));
    break;
}

// Jump if carry flag is not set
case((instruction) == 0b11010010): {
    uint16_t nextInstruction = ROM[pc + 1] | (ROM[pc + 2] << 8);  // Read 16-bit address
    if (!(myRegisters.flagsregister.carry)) {
        opcodes.jump(nextInstruction);  // Jump to address
    }
    pc += 3;  // Move the program counter by 3 (2 bytes for the address)
    Clock::getInstance().tick(static_cast<double>(12.0));
    break;
}

// Jump if carry flag is set
case((instruction) == 0b11011010): {
    uint16_t nextInstruction = ROM[pc + 1] | (ROM[pc + 2] << 8);  // Read 16-bit address
    if (myRegisters.flagsregister.carry) {
        opcodes.jump(nextInstruction);  // Jump to address
    }
    pc += 3;  // Move the program counter by 3 (2 bytes for the address)
    Clock::getInstance().tick(static_cast<double>(12.0));
    break;
}

// REti (Return from Interrupt)
case((instruction) == 0b11011001): {
    globals.pop();  // Pop the return address from the stack
    myRegisters.flagsregister.masterinterruptenable = true;  // Enable interrupts
    pc += 1;  // Move to the next instruction
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

// Jump to HL myRegisters (JP HL)
case((instruction) == 0b11101001): {
    uint16_t hlValue = myRegisters.get16Register(H, L);  // Get 16-bit value from HL myRegisters
    opcodes.jump(hlValue);  // Jump to the address in HL
    pc += 1;  // Move the program counter to the next instruction
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}


// Case: CALL conditional instructions (CALL cond, imm16)

// 0 nz (CALL if zero flag is not set)
case ((instruction) == 0b11000100): {
    uint16_t nextInstruction = ROM[pc + 1] | (ROM[pc + 2] << 8);  // Read 16-bit address from ROM
    if (!(myRegisters.flagsregister.zero)) {
        stack.push(nextInstruction);  // Push address of next instruction to the stack
        opcodes.jump(nextInstruction);  // Jump to the target address
    }
    pc += 3;  // Move past the CALL instruction (opcode + 2 bytes for the address)
    Clock::getInstance().tick(static_cast<double>(12.0));
    break;
}

// 1 z (CALL if zero flag is set)
case ((instruction) == 0b11001100): {
    uint16_t nextInstruction = ROM[pc + 1] | (ROM[pc + 2] << 8);  // Read 16-bit address from ROM
    if (myRegisters.flagsregister.zero) {
        stack.push(nextInstruction);  // Push address of next instruction to the stack
        opcodes.jump(nextInstruction);  // Jump to the target address
    }
    pc += 3;  // Move past the CALL instruction (opcode + 2 bytes for the address)
    Clock::getInstance().tick(static_cast<double>(12.0));
    break;
}

// 2 nc (CALL if carry flag is not set)
case ((instruction) == 0b11010100): {
    uint16_t nextInstruction = ROM[pc + 1] | (ROM[pc + 2] << 8);  // Read 16-bit address from ROM
    if (!(myRegisters.flagsregister.carry)) {
        stack.push(nextInstruction);  // Push address of next instruction to the stack
        opcodes.jump(nextInstruction);  // Jump to the target address
    }
    pc += 3;  // Move past the CALL instruction (opcode + 2 bytes for the address)
    Clock::getInstance().tick(static_cast<double>(12.0));
    break;
}

// 3 c (CALL if carry flag is set)
case ((instruction) == 0b11011100): {
    uint16_t nextInstruction = ROM[pc + 1] | (ROM[pc + 2] << 8);  // Read 16-bit address from ROM
    if (myRegisters.flagsregister.carry) {
        stack.push(nextInstruction);  // Push address of next instruction to the stack
        opcodes.jump(nextInstruction);  // Jump to the target address
    }
    pc += 3;  // Move past the CALL instruction (opcode + 2 bytes for the address)
    Clock::getInstance().tick(static_cast<double>(12.0));
    break;
}


case ((instruction) == 0b11001101): {
    // Get the next 16-bit address to jump to
    uint16_t nextInstruction = (ROM[pc + 1] << 8) | ROM[pc + 2];  // Combine two bytes to form the 16-bit address
    
    // Push the current program counter + 1 (address of the instruction following the CALL)
    stack.push(pc + 3);  // Push address of the next instruction (after the CALL instruction)

    // Set the program counter to the 16-bit address (jump)
    pc = nextInstruction;

    // No need to increment pc, as it has been set directly to the jump address
    Clock::getInstance().tick(static_cast<double>(12.0)); // opcode + imm16
    break;
}




//rst tgt3 


// POP BC
case ((instruction) == 0b11000001): {
    uint16_t value = stack.pop();  // Pop the 16-bit value from the stack
    myRegisters.set16Register(RegisterEnum::B, RegisterEnum::C, value);  // Set the BC myRegisters pair
    pc += 1;  // Move past the instruction byte
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}



// POP DE
case ((instruction) == 0b11010001): {
    uint16_t value = stack.pop();  // Pop the 16-bit value from the stack
    myRegisters.set16Register(RegisterEnum::D, RegisterEnum::E, value);  // Set the DE myRegisters pair
    pc += 1;  // Move past the instruction byte
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
} 

// POP HL
case ((instruction) == 0b11100001): {
    uint16_t value = stack.pop();  // Pop the 16-bit value from the stack
    myRegisters.load16BitRegister(RegisterEnum::H, RegisterEnum::L, value);  // Set the HL myRegisters pair
    pc += 1;  // Move past the instruction byte
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

// POP AF
case ((instruction) == 0b11110001): {
    uint16_t value = stack.pop();  // Pop the 16-bit value from the stack
    myRegisters.load16BitRegister(RegisterEnum::A, RegisterEnum::F, value);  // Set the AF myRegisters pair
    pc += 1;  // Move past the instruction byte
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}




//push r16stk 

// PUSH BC
case ((instruction) == 0b11000101): {
    uint16_t value = myRegisters.get16Register(RegisterEnum::B, RegisterEnum::C);  // Get the value from BC
    stack.push(value);  // Push the 16-bit value onto the stack
    pc += 1;  // Move past the instruction byte
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}  

// PUSH DE
case ((instruction) == 0b11010101): {
    uint16_t value = myRegisters.get16Register(RegisterEnum::D, RegisterEnum::E);  // Get the value from DE
    stack.push(value);  // Push the 16-bit value onto the stack
    pc += 1;  // Move past the instruction byte
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}


// PUSH HL
case ((instruction) == 0b11110010): {
    uint16_t value = myRegisters.get16Register(RegisterEnum::H, RegisterEnum::L);  // Get the value from HL
    stack.push(value);  // Push the 16-bit value onto the stack
    pc += 1;  // Move past the instruction byte
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

// PUSH AF
case ((instruction) == 0b11110101): {
    uint16_t value = myRegisters.get16Register(RegisterEnum::A, RegisterEnum::F);  // Get the value from AF
    stack.push(value);  // Push the 16-bit value onto the stack
    pc += 1;  // Move past the instruction byte
    Clock::getInstance().tick(static_cast<double>(4.0));
    break;
}

}


    //HALT
    case(opcode == 0x76){
        break;
    }

    case(opcode == 0x10){
        break;
    }
    

}
return 0;
}


