#include <cstdint>
#include <string>
#include <unordered_map>


//seperate opcode functions w/ parameters specified by rom


//seperate functions for every register operation?



//instructinos 4 to 8 bytes long



//max len of instructional set it 3 bytes
int pc = 0x100;
int cycle = 0;


class CPU {
Singleton globals = Singleton.getInstance();
Registers register = globals.getRegister();
int& programCounter = globals.getProgramCounter();
uint8_t ROM = globals.getROM();


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
    int instruction = ROM[pc];
    //1 byte instructional sets
    case (instruction == 0x00){
        pc += 1;
    }
    //HALT
    case (instruction == 0x76){
        pc+=1;
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
    
    case((instruction) == 0b11001110){
        int nextbyte = instruction >> 8;
        opcodes.addntocarry(6, 0, 0, nextbyte);
        //we move two bytes now
        pc+=2;
    }




    }
























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
