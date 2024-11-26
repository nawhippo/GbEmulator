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


class CPU {
Singleton globals = Singleton.getInstance();
Registers register = globals.getRegister();
int& programCounter = globals.getProgramCounter();
uint8_t ROM = globals.getROM();
OpCodes opcodes; 

//r8 operand translation
int operandTranslation(int translate){
    switch(translate){
    //b
    case (translate == 0b000){
        return 1;
    }
    //c
    case (translate == 0b001){
        return 2;
    }
    //d
    case (translate == 0b010){
        return 3;
    }
    //e
    case (translate == 0b011){
        return 4;
    }
    //h
    case (translate == 0b100){
        return 5;
    }
    //h
    case (translate == 0b101){
        return 6;
    }

    //hl (special case)
    case (translate == 0b110){
        
    }
    case (translate == 0b111){
        return 0;
    }
    }
}




int executeLoop(){
    while(true){
//flags first
    switch(instruction){
    //pc is incremented by bytes of the instruction
    uint8_t instruction = ROM[pc];
    //1 byte instructional sets
    case (instruction == 0x00){
        pc += 1;
    }
    //HALT
    case (instruction == 0x76){
        pc+=1;
    }


    //rcla accumulator rotate right 
    case (instruction == 0b00000111){
        OpCodes.rotateleftcircular(6);
    }

    //rrca 
    case (instruction == 0b00001111){
        OpCodes.rotaterightcircular(6);
    }

    //rla
    case (instruction == 0b00010111){
        OpCodes.rotateleft(6);
    }

    //rra 
    case (instruction == 0b00011111){
        OpCodes.rotateright(6);
    }

    //daa 
    case (instruction == 0b00100111){
        OpCodes.adjustAcc();
    }
    
    //cpl
    case (instruction == 0b00101111){
        OpCodes.complement();
    }

    //scf - set carry flag
    case (instruction == 0b00110111){
        OpCodes.setCarryFlag();
    }

    //ccf - complement carry flag
    case (instruction == 0b00111111){
        OpCodes.ComplementCarryFlag();
    }


    //ld r16 imm16
    case(instruction == 0b00000001){
        int nextInstruction = ROM[pc + 1];
        register.doubleRegistersArr[0] = nextInstruction;
        pc+=2;
    }


    case(instruction == 0b00010001){
        int nextInstruction = ROM[pc + 1];
        register.doubleRegistersArr[1] = nextInstruction;
        pc+=2;
    }

    case(instruction == 0b00100001){
        int nextInstruction = ROM[pc + 1];
        register.doubleRegistersArr[2] = nextInstruction;
        pc+=2;
    }

    case(instruction == 0b00110001){
        int nextInstruction = ROM[pc + 1];
        register.doubleRegistersArr[3] = nextInstruction;
        pc+=2;
    }


    //ld [r16mem], a
    case(instruction == 0b00000010){
        uint16_t addy = register.doubleRegistersArr[0];
        uint16_t valueToStore = <uint16_t> register.registersArr[6];
        RAM[addy] = valueToStore; 
        pc += 1;
    }

    case(instruction == 0b00010010){
        uint16_t addy = register.doubleRegistersArr[1];
        uint16_t valueToStore = <uint16_t> register.registersArr[6];
        RAM[addy] = valueToStore;
        pc += 1;
    } 
    
    case(instruction == 0b00 10 0010){
        uint16_t addy = register.doubleRegistersArr[2];
        uint16_t valueToStore = <uint16_t> register.registersArr[6];
        RAM[addy] = valueToStore;
        pc += 1;
    }

    case(instruction == 0b00110010){
        uint16_t addy = register.doubleRegistersArr[3];
        uint16_t valueToStore = <uint16_t> register.registersArr[6];
        RAM[addy] = valueToStore;
    }





    //ld a, [r16mem]

    //bc
    case (instruction == 0b00001010) {
        uint16_t addy = register.doubleRegistersArr[0]; 
        register.registersArr[6] = RAM[addy];         
        pc += 1;
    }

    //de
    case (instruction == 0b00011010) {
        uint16_t addy = register.doubleRegistersArr[1]; 
        register.registersArr[6] = RAM[addy];           
        pc += 1;
    }

    //hl
    case (instruction == 0b00101010): {
        uint16_t addy = register.doubleRegistersArr[2]; 
        register.registersArr[6] = RAM[addy];           
        pc += 1;
    }

    //af
    case (instruction == 0b00111010): {
        uint16_t addy = register.doubleRegistersArr[3]; 
        register.registersArr[6] = RAM[addy];         
        pc += 1;
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



    //inc r16 

    //bc
    case(instruction == 0b00 00 0011){
        register.doubleRegistersArr[0] += 1;
        pc+=1;
    }

    //de 
    case(instruction == 0b00 01 0011){
        register.doubleRegistersArr[1] += 1;
        pc+=1;
    }

    //hl
    case(instruction == 0b00 10 0011){
        register.doubleRegistersArr[2] += 1;
        pc+=1;
    }

    //af
    case(instruction == 0b00 11 0011){
        register.doubleRegistersArr[3] += 1;        
        pc+=1;
    }



    //dec r16 

    //bc
    case(instruction == 0b00 00 1011){
        register.doubleRegistersArr[0] -= 1;
    }

    //de 
    case(instruction == 0b00 01 1011){
        register.doubleRegistersArr[1] -= 1;
    }

    //hl
    case(instruction == 0b00 10 1011){
        register.doubleRegistersArr[2] -= 1;
    }

    //af
    case(instruction == 0b00 11 1011){
        register.doubleRegistersArr[3] -= 1;
    }

    //add hl, r16
    //bc
    case(instruction == 0b00 00 1001){
        register.doubleRegistersArr[2] += register.doubleRegistersArr[0];
    }

    //de
    case(instruction == 0b00 01 1001){
        register.doubleRegistersArr[2] += register.doubleRegistersArr[1];
    }

    //hl
    case(instruction == 0b00 10 1001){
        register.doubleRegistersArr[2] += register.doubleRegistersArr[2];
    }

    //sp
    case(instruction == 0b00 11 1001){
        register.doubleRegistersArr[2] += register.doubleRegistersArr[3];
    }


    //inc r8
    case(instruction == 0b00 000 100){
        register.registersArr[0]+=1;
    }

    case(instruction == 0b00 001 100){
        register.registersArr[1]+=1;
    }

    case(instruction == 0b00 010 100){
        register.registersArr[2]+=1;
    }

    case(instruction == 0b00 011 100){
        register.registersArr[3]+=1;
    }

    case(instruction == 0b00 100 100){
        register.registersArr[4]+=1;
    }

    case(instruction == 0b00 101 100){
        register.registersArr[5]+=1;
    }

    case(instruction == 0b00 111 100){
        register.registersArr[6]+=1;
    }


    //dec r8
    case(instruction == 0b00 000 101){
        register.registersArr[0]-=1;
    }

    case(instruction == 0b00 001 101){
        register.registersArr[1]-=1;
    }

    case(instruction == 0b00 010 101){
        register.registersArr[2]-=1;
    }

    case(instruction == 0b00 011 101){
        register.registersArr[3]-=1;
    }

    case(instruction == 0b00 100 101){
        register.registersArr[4]-=1;
    }

    case(instruction == 0b00 101 101){
        register.registersArr[5]-=1;
    }

    case(instruction == 0b00 111 101){
        register.registersArr[6]-=1;
    }


    //ld r8, imm8 
    case(instruction == 0b00 001 110){
        register.registersArr[0] = ROM[pc + 1];
    }


    case(instruction == 0b00 010 110){
        register.registersArr[1] = ROM[pc + 1];
    }

    case(instruction == 0b00 011 110){
        register.registersArr[2] = ROM[pc + 1];
        
    }

    case(instruction == 0b00 100 110){
        register.registersArr[3] = ROM[pc + 1];
        
    }

    case(instruction == 0b00 101 110){
        register.registersArr[4] = ROM[pc + 1];
    }

    case(instruction == 0b00 111 110){
        register.registersArr[5] = ROM[pc + 1];
        
    }












    //8 bit arithmetic operations

    case ((instruction >> 3) == 0b100000){
        //add to accumululator 
        //last three bits 
        int a = operandTranslation(instruction >> 6);
        opcodes.addnton(6, a, 6);
        pc+=1;
    }


    case ((instruction >> 3) == 0b100001){
        //add to accumululator 
        //last three bits 
        int a = operandTranslation(instruction >> 6);
        opcodes.addntonwcarry(6, a, 6);
        pc+=1;
    }

    case ((instruction >> 3) == 0b10010){
        int a = operandTranslation(instruction >> 6);
        opcodes.subnton(6, a, 6);
        pc+=1;
    }

    case ((instruction >> 3) == 0b10011){
        int a = operandTranslation(instruction >> 6);
        opcodes.subntoncarry(6, a, 6);
        pc+=1;
    }


    case ((instruction >> 3) == 0b10100){
        int a = operandTranslation(instruction >> 6);
        opcodes.subntoncarry(6, a, 6);
        pc+=1;
    }

    case ((instruction >> 3) == 0b10100){
        int a = operandTranslation(instruction >> 6);
        opcodes.andregn(6, a, 6);
        pc+=1;
    }
    
    case ((instruction >> 3) == 0b10101){
        int a = operandTranslation(instruction >> 6);
        opcodes.xorregn(6, a, 6);
        pc+=1;
    }
    
        
    case ((instruction >> 3) == 0b10110){
        int a = operandTranslation(instruction >> 6);
        opcodes.orregn(6, a, 6);
        pc+=1;
    }

            
    case ((instruction >> 3) == 0b10111){
        int a = operandTranslation(instruction >> 6);
        opcodes.cpregn(6, a, 6);
        pc+=1;
    }
    


        
    case((instruction) == 0b11000110){
        int nextbyte = instruction >> 8;
        opcodes.addnton(6, 0, 0, nextbyte);
        //we move two bytes now
        pc+=2;
    }
    

    //carry
    case((instruction) == 0b11001110){
        int nextbyte = instruction >> 8;
        opcodes.addntoncarry(6, 0, 0, nextbyte);
        //we move two bytes now
        pc+=2;
    }
    //sub
    case((instruction) == 0b11010110){
        int nextbyte = instruction >> 8;
        opcodes.subnton(6, 0, 0, nextbyte);
        //we move two bytes now
        pc+=2;
    }

    //subc
    case((instruction) == 0b11011110){
        int nextbyte = instruction >> 8;
        opcodes.subntoncarry(6, 0, 0, nextbyte);
        //we move two bytes now
        pc+=2;
    }

    //and
    case((instruction) == 0b11100110){
        int nextbyte = instruction >> 8;
        opcodes.andnton(6, 0, nextbyte);
        //we move two bytes now
        pc+=2;
    }

    //xor
    case((instruction) == 0b11101110){
        int nextbyte = instruction >> 8;
        opcodes.xornton(6, 0, nextbyte);
        //we move two bytes now
        pc+=2;
    }
    
    //or
    case((instruction) == 0b11110110){
        int nextbyte = instruction >> 8;
        opcodes.xornton(6, 0, nextbyte);
        //we move two bytes now
        pc+=2;
    }
    //cp
    case((instruction) == 0b11111110){
        int nextbyte = instruction >> 8;
        opcodes.cpnton(6, 0, nextbyte);
        //we move two bytes now
        pc+=2;
    }

    //ret conds
    //0 nz
    //1 z
    //2 nc
    //3 c
   case((instruction) == 0b11000000){
        if !(register.flagsregister.zero){
            globals.pop();
        }
        pc += 1
    }

    //z
    case((instruction) == 0b11001000){
        if (register.flagsregister.zero){
            globals.pop();
        }
        pc+=1;
    }

    //nc
    case((instruction) == 0b11010000){
        if !(register.flagsregister.carry){
            globals.pop();
        }
        pc+=1;
    }

    //c
    case((instruction) == 0b11011000){
        if !(register.flagsregister.carry){
            globals.pop();
        }
        pc+=1;
    }


    //ret
    case((instruction) == 0b011001001){
        globals.pop();
        pc+=1;
    }

    //reti
    //look at this one again 
    case((instruction) == 0b011001001){
        globals.pop();
        pc+=1;
    }



    //jp cond, imm16

    //0 nz
    case((instruction) == 0b11000010){
        uint16_t nextInstruction = ROM[pc + 1];
        if !(register.flagsregister.zero){
            opcodes.jump(nextInstruction);
        }
    }

    //1 z
    case((instruction) == 0b11001010){
        uint16_t nextInstruction = ROM[pc + 1];
        if (register.flagsregister.zero){
            opcodes.jump(nextInstruction);
        }
        pc += 2;
    }

    //2 nc
    case((instruction) == 0b11010010){
          uint16_t nextInstruction = ROM[pc + 1];
        if !(register.flagsregister.carry){
            opcodes.jump(nextInstruction);
        }
        pc += 2;
    }

    //3 c
    case((instruction) == 0b11011010){
          uint16_t nextInstruction = ROM[pc + 1];
        if (register.flagsregister.carry){
            opcodes.jump(nextInstruction);
        }
        pc+=2;
    }

    //reti 
    case((instruction) == 0b11011001){
        globals.pop();
        register.flagsregister.masterinterruptenable = true;
    }


    //jp hl 
    case((instruction) = 0b11101001){
        opcodes.jump(register.doubleRegistersArr[2]);
    }
    


    //case call cond imm16
  // 0 nz
case ((instruction) == 0b11000100): {
    uint8_t nextInstruction = ROM[pc + 1];
    if (!(register.flagsregister.zero)) {
        stack.push(nextInstruction); // Push address of the next instruction (after the CALL instruction)
    }
    pc += 2; // Move past the immediate 16-bit value
}

// 1 z
case ((instruction) == 0b11001100): {
    uint8_t nextInstruction = ROM[pc + 1];
    if (register.flagsregister.zero) {
        stack.push(nextInstruction); // Push address of the next instruction (after the CALL instruction)
    }
    pc += 2; // Move past the immediate 16-bit value
}

// 2 nc
case ((instruction) == 0b11010100): {
    uint16_t nextInstruction = ROM[pc + 1];
    if (!(register.flagsregister.carry)) {
        stack.push(nextInstruction); // Push address of the next instruction (after the CALL instruction)
    }
    pc += 2; // Move past the immediate 16-bit valued 
}

// 3 c
case ((instruction) == 0b11011100): {
    uint16_t nextInstruction = ROM[pc + 1];
    if (register.flagsregister.carry) {
        stack.push(nextInstruction); // Push address of the next instruction (after the CALL instruction)
    }
    pc += 2; // Move past the immediate 16-bit value
}


//call imm16
case ((instruction) == 0b11001101){
  uint16_t nextInstruction = ROM[pc + 1];
  stack.push(pc + 1);
  pc += 2;
}



//rst tgt3 


//pop r16stk
case ((instruction) == 0b11000001){
  register.registersArr[0] = stack.pop();
  pc += 1;
}

//1
case ((instruction) == 0b11010001){
  register.registersArr[1] = stack.pop();
  pc += 1;
}

//2
case ((instruction) == 0b11100001){
  uint16_t nextInstruction = ROM[pc + 1];
  register.registersArr[2] = stack.pop();
  pc += 1;
}


//3
case ((instruction) == 0b11110001){
  uint16_t nextInstruction = ROM[pc + 1];
  register.registersArr[3] = stack.pop();
  pc += 1;
}


//push r16stk 

//0
case ((instruction) == 0b11000101){
  uint16_t nextInstruction = ROM[pc + 1];
  stack.push(register.registersArr[0]);
  pc += 1;
}

//1
case ((instruction) == 0b11010101){
  uint16_t nextInstruction = ROM[pc + 1];
  stack.push(register.registersArr[1]);
  pc += 1;
}

//2
case ((instruction) == 0b111100101){
  uint8_t nextInstruction = ROM[pc + 1];
  stack.push(register.registersArr[2]);
  pc += 1;
}


//3
case ((instruction) == 0b11110101){
  uint8_t nextInstruction = ROM[pc + 1];
  stack.push(register.registersArr[3]);
  pc += 1;
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




















}
