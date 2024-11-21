#include <cstdint>
#include <string>
#include <unordered_map>
#include <opCodes.cpp>

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
    case((instruction) == 011001001){
        globals.pop();
        pc+=1;
    }

    //reti
    //look at this one again 
    case((instruction) == 011001001){
        globals.pop();
        pc+=1;
    }



    //jp cond 

    //0 nz
    case((instruction) == 11000010){
        if !(register.flagsregister.zero){
            opcodes.jump();
        }
    }

    //1 z
    case((instruction) == 11001010){
        if (register.flagsregister.zero){
            opcodes.jump();
        }
    }

    //2 nc
    case((instruction) == 11010010){
        if !(register.flagsregister.carry){
            opcodes.jump();
        }
    }

    //3 c
    case((instruction) == 11011010){
        if (register.flagsregister.carry){
            opcodes.jump();
        }
    }

    //reti 
    case((instruction) == 11011001){
        globals.pop();
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
