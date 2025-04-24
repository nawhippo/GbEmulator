#include <cassert>
#include "../cpu/OpcodeHandler.cpp"
#include "../Registers.h"

// Mock CPU state
Registers registers;
uint8_t memory[0x10000];
uint16_t stack[0x100];
int stackPointer = 0;

// Helper functions
void resetCPUState() {
    // Reset registers
    for (int i = 0; i < 8; i++) registers.registersArr[i] = 0;
    registers.flagsregister = {false, false, false, false};
    // Reset memory
    for (int i = 0; i < 0x10000; i++) memory[i] = 0;
    // Reset stack
    stackPointer = 0;
}

// Mock stack operations
void push(uint16_t value) {
    stack[stackPointer++] = value;
}

uint16_t pop() {
    return stack[--stackPointer];
}

// Test functions
void testNOP() {
    resetCPUState();
    int initialPC = pc;
    executeInstruction(0x00); // NOP
    assert(pc == initialPC + 1); // PC should increment by 1
}

void testLD_BC_d16() {
    resetCPUState();
    memory[pc + 1] = 0x34; // Low byte
    memory[pc + 2] = 0x12; // High byte
    executeInstruction(0x01); // LD BC, d16
    assert(registers.get16Register(Registers::B, Registers::C) == 0x1234);
    assert(pc == 0x103); // PC should increment by 3
}

void testLD_BC_A() {
    resetCPUState();
    registers.registersArr[Registers::A] = 0x56; // Set A
    registers.load16BitRegister(Registers::B, Registers::C, 0x1234); // Set BC
    executeInstruction(0x02); // LD (BC), A
    assert(memory[0x1234] == 0x56); // Memory at BC should equal A
    assert(pc == 0x101); // PC should increment by 1
}

void testINC_BC() {
    resetCPUState();
    registers.load16BitRegister(Registers::B, Registers::C, 0xFFFF); // Set BC
    executeInstruction(0x03); // INC BC
    assert(registers.get16Register(Registers::B, Registers::C) == 0x0000); // BC should wrap around
    assert(pc == 0x101); // PC should increment by 1
}

void testADD_HL_BC() {
    resetCPUState();
    registers.load16BitRegister(Registers::H, Registers::L, 0x1234); // Set HL
    registers.load16BitRegister(Registers::B, Registers::C, 0x1111); // Set BC
    executeInstruction(0x09); // ADD HL, BC
    assert(registers.get16Register(Registers::H, Registers::L) == 0x2345); // HL should equal HL + BC
    assert(pc == 0x101); // PC should increment by 1
}

void testJP() {
    resetCPUState();
    memory[pc + 1] = 0x34; // Low byte
    memory[pc + 2] = 0x12; // High byte
    executeInstruction(0xC3); // JP 0x1234
    assert(pc == 0x1234); // PC should jump to 0x1234
}

void testCALL() {
    resetCPUState();
    memory[pc + 1] = 0x34; // Low byte
    memory[pc + 2] = 0x12; // High byte
    int initialPC = pc;
    executeInstruction(0xCD); // CALL 0x1234
    assert(pc == 0x1234); // PC should jump to 0x1234
    assert(stack[stackPointer - 1] == initialPC + 3); // Return address should be pushed to stack
}

void testRET() {
    resetCPUState();
    push(0x1234); // Push return address
    executeInstruction(0xC9); // RET
    assert(pc == 0x1234); // PC should jump to return address
}

// Test adding 8-bit registers
void testADD_A_r8() {
    resetCPUState();
    registers.registersArr[Registers::A] = 0x12;
    registers.registersArr[Registers::B] = 0x34;
    memory[0x100] = 0x80; // ADD A, B opcode
    executeInstruction(0x80);
    assert(registers.registersArr[Registers::A] == 0x46);
    assert(!registers.flagsregister.zero);
    assert(!registers.flagsregister.carry);
    assert(!registers.flagsregister.subtract);
}

// Test subtracting 8-bit registers
void testSUB_A_r8() {
    resetCPUState();
    registers.registersArr[Registers::A] = 0x56;
    registers.registersArr[Registers::C] = 0x34;
    memory[0x100] = 0x91; // SUB A, C opcode
    executeInstruction(0x91);
    assert(registers.registersArr[Registers::A] == 0x22);
    assert(!registers.flagsregister.zero);
    assert(!registers.flagsregister.carry);
    assert(registers.flagsregister.subtract);
}

// Test loading 8-bit registers
void testLD_r8_imm8() {
    resetCPUState();
    memory[0x100] = 0x06; // LD B, imm8 opcode
    memory[0x101] = 0x42; // Immediate value
    executeInstruction(0x06);
    assert(registers.registersArr[Registers::B] == 0x42);
}

// Test loading 16-bit registers
void testLD_r16_imm16() {
    resetCPUState();
    memory[0x100] = 0x01; // LD BC, imm16 opcode
    memory[0x101] = 0x34; // Low byte
    memory[0x102] = 0x12; // High byte
    executeInstruction(0x01);
    assert(registers.get16Register(Registers::B, Registers::C) == 0x1234);
}

// Main test runner
int main() {
    testNOP();
    testLD_BC_d16();
    testLD_BC_A();
    testINC_BC();
    testADD_HL_BC();
    testJP();
    testCALL();
    testRET();
    testADD_A_r8();
    testSUB_A_r8();
    testLD_r8_imm8();
    testLD_r16_imm16();
    // Add more tests for other opcodes here...
    return 0;
}
