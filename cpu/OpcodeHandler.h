#ifndef OPCODEHANDLER_H
#define OPCODEHANDLER_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include "../Singleton.h"
#include "../Registers.h"

class OpcodeHandler {
public:
    // Register enumeration for operand translation
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

    // Constructor (default is fine since no member variables)
    OpcodeHandler() = default;

    // Main instruction execution function
    int executeInstruction(uint16_t instruction);

    // Operand translation for r8 operands (3-bit encoding to register reference)
    uint8_t& operandTranslation(int translate);

    // Instruction category handlers
    void handleArithmetic(uint16_t instruction);
    void handleLoad(uint16_t instruction);
    void handleJump(uint16_t instruction);
    void handleStack(uint16_t instruction);
    void handleRotateShift(uint16_t instruction);
    void handleControl(uint16_t instruction);

    // Specific instruction implementations
    void executeNOP();
    void executeHALT();
    void executeSTOP();
    
    // Arithmetic operations
    void executeADD(uint8_t value);
    void executeADC(uint8_t value);
    void executeSUB(uint8_t value);
    void executeSBC(uint8_t value);
    void executeAND(uint8_t value);
    void executeXOR(uint8_t value);
    void executeOR(uint8_t value);
    void executeCP(uint8_t value);
    void executeINC(uint8_t& reg);
    void executeDEC(uint8_t& reg);
    
    // 16-bit arithmetic
    void executeADD_HL(uint16_t value);
    void executeINC_16(uint8_t high, uint8_t low);
    void executeDEC_16(uint8_t high, uint8_t low);
    
    // Load operations
    void executeLD_r8_r8(uint8_t& dest, uint8_t src);
    void executeLD_r8_imm8(uint8_t& reg, uint8_t value);
    void executeLD_r16_imm16(uint8_t high, uint8_t low, uint16_t value);
    void executeLD_mem_r8(uint16_t address, uint8_t value);
    void executeLD_r8_mem(uint8_t& reg, uint16_t address);
    
    // Jump operations
    void executeJP(uint16_t address);
    void executeJP_cond(bool condition, uint16_t address);
    void executeJR(int8_t offset);
    void executeJR_cond(bool condition, int8_t offset);
    
    // Call/Return operations
    void executeCALL(uint16_t address);
    void executeCALL_cond(bool condition, uint16_t address);
    void executeRET();
    void executeRET_cond(bool condition);
    void executeRETI();
    void executeRST(uint8_t vector);
    
    // Stack operations
    void executePUSH(uint16_t value);
    void executePOP(uint8_t& high, uint8_t& low);
    
    // Rotate and shift operations
    void executeRLCA();
    void executeRLA();
    void executeRRCA();
    void executeRRA();
    void executeRLC(uint8_t& reg);
    void executeRL(uint8_t& reg);
    void executeRRC(uint8_t& reg);
    void executeRR(uint8_t& reg);
    void executeSLA(uint8_t& reg);
    void executeSRA(uint8_t& reg);
    void executeSRL(uint8_t& reg);
    void executeSWAP(uint8_t& reg);
    
    // Bit operations
    void executeBIT(uint8_t bit, uint8_t value);
    void executeSET(uint8_t bit, uint8_t& reg);
    void executeRES(uint8_t bit, uint8_t& reg);
    
    // Miscellaneous operations
    void executeDAA();
    void executeCPL();
    void executeSCF();
    void executeCCF();
    void executeDI();
    void executeEI();

    // Flag condition checking
    bool checkCondition(uint8_t condition);
    
    // Utility functions
    void updateFlags(uint8_t result, bool subtract = false, bool halfCarry = false, bool carry = false);
    void updateFlagsArithmetic(uint8_t a, uint8_t b, uint16_t result, bool subtract = false);

private:
    // Helper functions
    uint8_t readMemory(uint16_t address);
    void writeMemory(uint16_t address, uint8_t value);
    uint16_t read16BitImmediate();
    uint8_t read8BitImmediate();
    
    // Stack helper functions
    void pushToStack(uint16_t value);
    uint16_t popFromStack();
};

#endif // OPCODEHANDLER_H