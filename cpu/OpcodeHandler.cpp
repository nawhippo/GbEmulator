#include <cstdint>
#include <string>
#include <unordered_map>
#include <opCodes.cpp>



//instructinos 4 to 8 bits long
//uint16_ts can be passed as indices to RAM

//TODO: IMM 8 block 3
//$CB PREFIXES


//max len of instructional set it 3 bytes
int pc = 0x100;
int cycle = 0;


class OpcodeHandler {
Singleton globals = Singleton.getInstance();
Registers register = globals.getRegisters();
int& programCounter = globals.getProgramCounter();
uint8_t ROM = globals.getROM();
OpCodes opcodes; 

//r8 operand translation
Register operandTranslation(int translate){
    switch(translate){
    //b
    case (translate == 0b000){
        return B;
    }
    //c
    case (translate == 0b001){
        return C;
    }
    //d
    case (translate == 0b010){
        return D;
    }
    //e
    case (translate == 0b011){
        return E;
    }
    //h
    case (translate == 0b100){
        return H;
    }
    //l
    case (translate == 0b101){
        return L;
    }

    //hl (special addy)
    case (translate == 0b110){
        return register.get16
    }
    case (translate == 0b111){
        return 0;
    }
    }
}




int execute(){ 
    uint16_t instruction = ROM[pc];
    result = executeInstruction(instruction);
    return result;
}





























int executeInstruction(uint16_t instruction){
    switch(instruction){
    //1 byte instructional sets
    //NOP
    case (instruction == 0x00){
        pc += 1;
    }
    //HALT
    case (instruction == 0x76){
        pc+=1;
    }

 // Rotate A left (RL)
    case (instruction == 0b00000111): {
        uint8_t carry = flagsregister.carry;
        // Rotate the A register left, shifting bit 7 into the carry and bit 0 into A
        uint8_t result = registersArr[Register::A] << 1 | carry;
        
        // Update the A register with the result
        registersArr[Register::A] = result;
        
        // Update carry flag based on the old bit 7 of A
        flagsregister.carry = (registersArr[Register::A] >> 7) & 1;
        
        pc += 1;
        break;
    }

    // Rotate A right (RR)
    case (instruction == 0b00001111): {
        uint8_t carry = flagsregister.carry;
        // Rotate the A register right, shifting bit 0 into the carry and bit 7 into A
        uint8_t result = registersArr[Register::A] >> 1 | (carry << 7);
        
        // Update the A register with the result
        registersArr[Register::A] = result;
        
        // Update carry flag based on the old bit 0 of A
        flagsregister.carry = registersArr[Register::A] & 1;
        
        pc += 1;
        break;
    }

    // Rotate A left through carry (RLA)
    case (instruction == 0b00010111): {
        uint8_t carry = flagsregister.carry;
        // Rotate the A register left through carry
        uint8_t result = (registersArr[Register::A] << 1) | carry;
        
        // Update the A register with the result
        registersArr[Register::A] = result;
        
        // Update the carry flag based on the old bit 7 of A
        flagsregister.carry = (registersArr[Register::A] >> 7) & 1;
        
        pc += 1;
        break;
    }

    // Rotate A right through carry (RRA)
    case (instruction == 0b00011111): {
        uint8_t carry = flagsregister.carry;
        // Rotate the A register right through carry
        uint8_t result = (registersArr[Register::A] >> 1) | (carry << 7);
        
        // Update the A register with the result
        registersArr[Register::A] = result;
        
        // Update the carry flag based on the old bit 0 of A
        flagsregister.carry = registersArr[Register::A] & 1;
        
        pc += 1;
        break;
    }

    // Decimal Adjust Accumulator (DAA)
    case (instruction == 0b00100111): {
        uint8_t A = registersArr[Register::A];
        
        if ((A & 0x0F) > 9 || flagsregister.half_carry) {
            A += 0x06;
        }
        if (A > 0x99 || flagsregister.carry) {
            A += 0x60;
            flagsregister.carry = 1;
        } else {
            flagsregister.carry = 0;
        }
        
        // Update the A register with the result
        registersArr[Register::A] = A;
        
        // Reset the half-carry flag
        flagsregister.half_carry = 0;
        
        pc += 1;
        break;
    }

    // Complement (CPL)
    case (instruction == 0b00101111): {
        // Complement all the bits in A
        registersArr[Register::A] = ~registersArr[Register::A];
        
        // Set the half-carry flag
        flagsregister.half_carry = 1;
        
        // Clear the carry flag
        flagsregister.carry = 0;
        
        pc += 1;
        break;
    }

    // Set Carry Flag (SCF)
    case (instruction == 0b00110111): {
        // Set the carry flag
        flagsregister.carry = 1;
        
        // Clear the half-carry flag
        flagsregister.half_carry = 0;
        
        pc += 1;
        break;
    }

    // Complement Carry Flag (CCF)
    case (instruction == 0b00111111): {
        // Complement the carry flag
        flagsregister.carry = !flagsregister.carry;
        
        // Clear the half-carry flag
        flagsregister.half_carry = 0;
        
        pc += 1;
        break;
    }
}

//LD r16, imm16

//BC
case (instruction == 0b00000001): {
    uint8_t lowByte = ROM[pc + 1];
    uint8_t highByte = ROM[pc + 2];
    uint16_t nextInstruction = (highByte << 8) | lowByte;
    register.load16BitRegister(Register::B, Register::C, nextInstruction);
    pc += 3; // 1 byte for opcode + 2 bytes for imm16
}

//DE
case (instruction == 0b00010001): {
    uint8_t lowByte = ROM[pc + 1];
    uint8_t highByte = ROM[pc + 2];
    uint16_t nextInstruction = (highByte << 8) | lowByte;
    register.load16BitRegister(Register::D, Register::E, nextInstruction);
    pc += 3; // 1 byte for opcode + 2 bytes for imm16
}

//HL
case (instruction == 0b00100001): {
    uint8_t lowByte = ROM[pc + 1];
    uint8_t highByte = ROM[pc + 2];
    uint16_t nextInstruction = (highByte << 8) | lowByte;
    register.load16BitRegister(Register::H, Register::L, nextInstruction);
    pc += 3; // 1 byte for opcode + 2 bytes for imm16
}

//SP
case (instruction == 0b00110001): {
    uint8_t lowByte = ROM[pc + 1];
    uint8_t highByte = ROM[pc + 2];
    uint16_t nextInstruction = (highByte << 8) | lowByte;
    register.load16BitRegister(Register::S, Register::P, nextInstruction);
    pc += 3; // 1 byte for opcode + 2 bytes for imm16
}



// LD A, [r16mem]

// BC
case (instruction == 0b00001010): {
    uint16_t addy = register.get16BitRegister(Register::B, Register::C);
    register.registersArr[6] = RAM[addy];
    pc += 1; // Move past the opcode
}

// DE
case (instruction == 0b00011010): {
    uint16_t addy = register.get16BitRegister(Register::D, Register::E);
    register.registersArr[6] = RAM[addy];
    pc += 1; // Move past the opcode
}

// HL
case (instruction == 0b00101010): {
    uint16_t addy = register.get16BitRegister(Register::H, Register::L);
    register.registersArr[6] = RAM[addy];
    pc += 1; // Move past the opcode
}

// AF
case (instruction == 0b00111010): {
    uint16_t addy = register.get16BitRegister(Register::A, Register::F); 
    register.registersArr[6] = RAM[addy];
    pc += 1; // Move past the opcode
}



//ld [imm16], sp
    case (instruction == 0b00001000): {
    uint16_t first = RAM[pc + 1] << 8;
    uint16_t second = RAM[pc + 2];
    uint16_t addy = first | second; 
    RAM[addy] = globals.stackPtr & 0xFF;  
    RAM[addy + 1] = (globals.stackPtr >> 8) & 0xFF;

    pc += 3;  
}



// INC r16

case (instruction == 0b00000011): {
    register.modify16BitRegister(Register::B, Register::C, 1);
    pc += 1;
}

case (instruction == 0b00010011): {
    register.modify16BitRegister(Register::D, Register::E, 1);
    pc += 1;
}

case (instruction == 0b00100011): {
    register.modify16BitRegister(Register::H, Register::L, 1);
    pc += 1;
}

case (instruction == 0b00110011): {
    register.modify16BitRegister(Register::A, Register::F, 1);
    pc += 1;
}

//dec r16 
case(instruction == 0b0000001011): {
    register.modify16BitRegister(Register::B, Register::C, -1);
    pc += 1;
}

case(instruction == 0b0001101011): {
    register.modify16BitRegister(Register::D, Register::E, -1);
    pc += 1;
}

case(instruction == 0b0010101011): {
    register.modify16BitRegister(Register::H, Register::L, -1);
    pc += 1;
}

case(instruction == 0b0011101011): {
    register.modify16BitRegister(Register::A, Register::F, -1);
    pc += 1;
}

//add hl, r16
case(instruction == 0b0000001001): {
    uint16_t valueToAdd = register.get16Register(Register::B, Register::C);
    register.modify16BitRegister(Register::H, Register::L, valueToAdd);
    pc += 1;
}

case(instruction == 0b0001101001): {
    uint16_t valueToAdd = register.get16Register(Register::D, Register::E);
    register.modify16BitRegister(Register::H, Register::L, valueToAdd);
    pc += 1;
}

case(instruction == 0b0010101001): {
    uint16_t valueToAdd = register.get16Register(Register::H, Register::L);
    register.modify16BitRegister(Register::H, Register::L, valueToAdd);
    pc += 1;
}

case(instruction == 0b0011101001): {
    uint16_t valueToAdd = register.get16Register(Register::A, Register::F);
    register.modify16BitRegister(Register::H, Register::L, valueToAdd);
    pc += 1;
}

    //inc r8
case(instruction == 0b000000100): {
    register.registersArr[Register::B] += 1;
    pc += 1;
}

case(instruction == 0b000001100): {
    register.registersArr[Register::C] += 1;
    pc += 1;
}

case(instruction == 0b000010100): {
    register.registersArr[Register::D] += 1;
    pc += 1;
}

case(instruction == 0b000011100): {
    register.registersArr[Register::E] += 1;
    pc += 1;
}

case(instruction == 0b000100100): {
    register.registersArr[Register::H] += 1;
    pc += 1;
}

case(instruction == 0b000101100): {
    register.registersArr[Register::L] += 1;
    pc += 1;
}

case(instruction == 0b000111100): {
    register.registersArr[Register::F] += 1;
    pc += 1;
}

//dec r8
case(instruction == 0b000000101): {
    register.registersArr[Register::A] -= 1;
    pc += 1;
}

case(instruction == 0b000001101): {
    register.registersArr[Register::B] -= 1;
    pc += 1;
}

case(instruction == 0b000010101): {
    register.registersArr[Register::C] -= 1;
    pc += 1;
}

case(instruction == 0b000011101): {
    register.registersArr[Register::D] -= 1;
    pc += 1;
}

case(instruction == 0b000100101): {
    register.registersArr[Register::E] -= 1;
    pc += 1;
}

case(instruction == 0b000101101): {
    register.registersArr[Register::H] -= 1;
    pc += 1;
}

case(instruction == 0b000111101): {
    register.registersArr[Register::L] -= 1;
    pc += 1;
}


//ld r8, imm8
case(instruction == 0b000001110): {
    register.registersArr[Register::A] = ROM[pc + 1];
    pc += 2;
}

case(instruction == 0b000010110): {
    register.registersArr[Register::B] = ROM[pc + 1];
    pc += 2;
}

case(instruction == 0b000011110): {
    register.registersArr[Register::C] = ROM[pc + 1];
    pc += 2;
}

case(instruction == 0b000100110): {
    register.registersArr[Register::D] = ROM[pc + 1];
    pc += 2;
}

case(instruction == 0b000101110): {
    register.registersArr[Register::E] = ROM[pc + 1];
    pc += 2;
}

case(instruction == 0b000111110): {
    register.registersArr[Register::H] = ROM[pc + 1];
    pc += 2;
}



// Add to accumulator
case ((instruction & 0xF8) == 0b10000000): {  // 0x80 -> 10000000
    int a = operandTranslation(instruction >> 6);  // Get the operand
    register.registersArr[Register::A] += a;  // Add operand to the accumulator
    register.flagsregister.zero = (register.registersArr[Register::A] == 0);
    register.flagsregister.subtract = false;
    register.flagsregister.half_carry = ((register.registersArr[Register::A] & 0x0F) < (a & 0x0F));
    register.flagsregister.carry = (register.registersArr[Register::A] > 0xFF);
    pc += 1;
    break;
}

// Add with carry
case ((instruction & 0xF8) == 0b10000110): {  // 0x86 -> 10000110
    int a = operandTranslation(instruction >> 6);  // Get the operand
    uint16_t result = register.registersArr[Register::A] + a + (register.flagsregister.carry ? 1 : 0);  // Add with carry
    register.registersArr[Register::A] = result & 0xFF;  // Keep only the lower 8 bits
    register.flagsregister.zero = (register.registersArr[Register::A] == 0);
    register.flagsregister.subtract = false;
    register.flagsregister.half_carry = ((register.registersArr[Register::A] & 0x0F) < (a & 0x0F) + (register.flagsregister.carry ? 1 : 0));
    register.flagsregister.carry = (result > 0xFF);  // Carry flag if overflow
    pc += 1;
    break;
}

// Subtract from accumulator
case ((instruction & 0xF8) == 0b10001010): {  // 0x8A -> 10001010
    int a = operandTranslation(instruction >> 6);  // Get the operand
    register.registersArr[Register::A] -= a;  // Subtract operand from the accumulator
    register.flagsregister.zero = (register.registersArr[Register::A] == 0);
    register.flagsregister.subtract = true;
    register.flagsregister.half_carry = ((register.registersArr[Register::A] & 0x0F) > (a & 0x0F));  // Check for half-carry
    register.flagsregister.carry = (register.registersArr[Register::A] > 0xFF);  // Carry flag set if result is negative
    pc += 1;
    break;
}

// Subtract with carry
case ((instruction & 0xF8) == 0b10001110): {  // 0x8E -> 10001110
    int a = operandTranslation(instruction >> 6);  // Get the operand
    uint16_t result = register.registersArr[Register::A] - a - (register.flagsregister.carry ? 1 : 0);  // Subtract with carry
    register.registersArr[Register::A] = result & 0xFF;  // Keep only the lower 8 bits
    register.flagsregister.zero = (register.registersArr[Register::A] == 0);
    register.flagsregister.subtract = true;
    register.flagsregister.half_carry = ((register.registersArr[Register::A] & 0x0F) > (a & 0x0F) + (register.flagsregister.carry ? 1 : 0));
    register.flagsregister.carry = (result > 0xFF);  // Carry flag if underflow
    pc += 1;
    break;
}

// AND with accumulator
case ((instruction & 0xF8) == 0b10100110): {  // 0xA6 -> 10100110
    int a = operandTranslation(instruction >> 6);  // Get the operand
    register.registersArr[Register::A] &= a;  // Perform AND operation with accumulator
    register.flagsregister.zero = (register.registersArr[Register::A] == 0);
    register.flagsregister.subtract = false;
    register.flagsregister.half_carry = true;  // Half-carry is always set for AND
    register.flagsregister.carry = false;  // No carry for AND
    pc += 1;
    break;
}



    //ret conds
    //0 nz
    //1 z
    //2 nc
    //3 c
    // RET nz (return if zero flag is not set)
case((instruction) == 0b11000000): {
    if (!(register.flagsregister.zero)) {  // Check if zero flag is not set
        globals.pop();  // Pop address from stack
    }
    pc += 1;  // Move program counter to the next instruction
    break;
}

// RET z (return if zero flag is set)
case((instruction) == 0b11001000): {
    if (register.flagsregister.zero) {  // Check if zero flag is set
        globals.pop();  // Pop address from stack
    }
    pc += 1;  // Move program counter to the next instruction
    break;
}

// RET nc (return if carry flag is not set)
case((instruction) == 0b11010000): {
    if (!(register.flagsregister.carry)) {  // Check if carry flag is not set
        globals.pop();  // Pop address from stack
    }
    pc += 1;  // Move program counter to the next instruction
    break;
}

// RET c (return if carry flag is set)
case((instruction) == 0b11011000): {
    if (register.flagsregister.carry) {  // Check if carry flag is set
        globals.pop();  // Pop address from stack
    }
    pc += 1;  // Move program counter to the next instruction
    break;
}

// RET (unconditional return)
case((instruction) == 0b11001001): {  // 0xC9 for RET
    globals.pop();  // Pop address from stack
    pc += 1;  // Move program counter to the next instruction
    break;
}

// RETI (return from interrupt)
case((instruction) == 0b11001001): {  // 0xC9 for RETI
    globals.pop();  // Pop address from stack
    // Handle enabling interrupts (if applicable for your CPU)
    pc += 1;  // Move program counter to the next instruction
    break;
}




    //jp cond, imm16

 // Jump with condition and 16-bit address (JP cond, imm16)

// Jump if zero flag is not set
case((instruction) == 0b11000010): {
    uint16_t nextInstruction = ROM[pc + 1] | (ROM[pc + 2] << 8);  // Read 16-bit address
    if (!(register.flagsregister.zero)) {
        opcodes.jump(nextInstruction);  // Jump to address
    }
    pc += 3;  // Move the program counter by 3 (2 bytes for the address)
    break;
}

// Jump if zero flag is set
case((instruction) == 0b11001010): {
    uint16_t nextInstruction = ROM[pc + 1] | (ROM[pc + 2] << 8);  // Read 16-bit address
    if (register.flagsregister.zero) {
        opcodes.jump(nextInstruction);  // Jump to address
    }
    pc += 3;  // Move the program counter by 3 (2 bytes for the address)
    break;
}

// Jump if carry flag is not set
case((instruction) == 0b11010010): {
    uint16_t nextInstruction = ROM[pc + 1] | (ROM[pc + 2] << 8);  // Read 16-bit address
    if (!(register.flagsregister.carry)) {
        opcodes.jump(nextInstruction);  // Jump to address
    }
    pc += 3;  // Move the program counter by 3 (2 bytes for the address)
    break;
}

// Jump if carry flag is set
case((instruction) == 0b11011010): {
    uint16_t nextInstruction = ROM[pc + 1] | (ROM[pc + 2] << 8);  // Read 16-bit address
    if (register.flagsregister.carry) {
        opcodes.jump(nextInstruction);  // Jump to address
    }
    pc += 3;  // Move the program counter by 3 (2 bytes for the address)
    break;
}

// REti (Return from Interrupt)
case((instruction) == 0b11011001): {
    globals.pop();  // Pop the return address from the stack
    register.flagsregister.masterinterruptenable = true;  // Enable interrupts
    pc += 1;  // Move to the next instruction
    break;
}

// Jump to HL register (JP HL)
case((instruction) == 0b11101001): {
    uint16_t hlValue = register.get16Register(H, L);  // Get 16-bit value from HL register
    opcodes.jump(hlValue);  // Jump to the address in HL
    pc += 1;  // Move the program counter to the next instruction
    break;
}


// Case: CALL conditional instructions (CALL cond, imm16)

// 0 nz (CALL if zero flag is not set)
case ((instruction) == 0b11000100): {
    uint16_t nextInstruction = ROM[pc + 1] | (ROM[pc + 2] << 8);  // Read 16-bit address from ROM
    if (!(register.flagsregister.zero)) {
        stack.push(nextInstruction);  // Push address of next instruction to the stack
        opcodes.jump(nextInstruction);  // Jump to the target address
    }
    pc += 3;  // Move past the CALL instruction (opcode + 2 bytes for the address)
    break;
}

// 1 z (CALL if zero flag is set)
case ((instruction) == 0b11001100): {
    uint16_t nextInstruction = ROM[pc + 1] | (ROM[pc + 2] << 8);  // Read 16-bit address from ROM
    if (register.flagsregister.zero) {
        stack.push(nextInstruction);  // Push address of next instruction to the stack
        opcodes.jump(nextInstruction);  // Jump to the target address
    }
    pc += 3;  // Move past the CALL instruction (opcode + 2 bytes for the address)
    break;
}

// 2 nc (CALL if carry flag is not set)
case ((instruction) == 0b11010100): {
    uint16_t nextInstruction = ROM[pc + 1] | (ROM[pc + 2] << 8);  // Read 16-bit address from ROM
    if (!(register.flagsregister.carry)) {
        stack.push(nextInstruction);  // Push address of next instruction to the stack
        opcodes.jump(nextInstruction);  // Jump to the target address
    }
    pc += 3;  // Move past the CALL instruction (opcode + 2 bytes for the address)
    break;
}

// 3 c (CALL if carry flag is set)
case ((instruction) == 0b11011100): {
    uint16_t nextInstruction = ROM[pc + 1] | (ROM[pc + 2] << 8);  // Read 16-bit address from ROM
    if (register.flagsregister.carry) {
        stack.push(nextInstruction);  // Push address of next instruction to the stack
        opcodes.jump(nextInstruction);  // Jump to the target address
    }
    pc += 3;  // Move past the CALL instruction (opcode + 2 bytes for the address)
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
    break;
}




//rst tgt3 


// POP BC
case ((instruction) == 0b11000001): {
    uint16_t value = stack.pop();  // Pop the 16-bit value from the stack
    register.set16Register(Register::B, Register::C, value);  // Set the BC register pair
    pc += 1;  // Move past the instruction byte
    break;
}



// POP DE
case ((instruction) == 0b11010001): {
    uint16_t value = stack.pop();  // Pop the 16-bit value from the stack
    register.set16Register(Register::D, Register::E, value);  // Set the DE register pair
    pc += 1;  // Move past the instruction byte
    break;
} 

// POP HL
case ((instruction) == 0b11100001): {
    uint16_t value = stack.pop();  // Pop the 16-bit value from the stack
    register.set16Register(Register::H, Register::L, value);  // Set the HL register pair
    pc += 1;  // Move past the instruction byte
    break;
}


// POP AF
case ((instruction) == 0b11110001): {
    uint16_t value = stack.pop();  // Pop the 16-bit value from the stack
    register.set16Register(Register::A, Register::F, value);  // Set the AF register pair
    pc += 1;  // Move past the instruction byte
    break;
}




//push r16stk 

// PUSH BC
case ((instruction) == 0b11000101): {
    uint16_t value = register.get16Register(Register::B, Register::C);  // Get the value from BC
    stack.push(value);  // Push the 16-bit value onto the stack
    pc += 1;  // Move past the instruction byte
    break;
}  

// PUSH DE
case ((instruction) == 0b11010101): {
    uint16_t value = register.get16Register(Register::D, Register::E);  // Get the value from DE
    stack.push(value);  // Push the 16-bit value onto the stack
    pc += 1;  // Move past the instruction byte
    break;
}


// PUSH HL
case ((instruction) == 0b11110010): {
    uint16_t value = register.get16Register(Register::H, Register::L);  // Get the value from HL
    stack.push(value);  // Push the 16-bit value onto the stack
    pc += 1;  // Move past the instruction byte
    break;
}


// PUSH AF
case ((instruction) == 0b11110101): {
    uint16_t value = register.get16Register(Register::A, Register::F);  // Get the value from AF
    stack.push(value);  // Push the 16-bit value onto the stack
    pc += 1;  // Move past the instruction byte
    break;
}

}


//ldh [c] a











    //HALT
    case(opcode == 0x76){
        break;
    }

    case(opcode == 0x10){
        break;
    }
    
    //2bytes instructional sets

}
    



instruction = instruction
}
return 0;
}











