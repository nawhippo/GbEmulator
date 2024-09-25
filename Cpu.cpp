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




int executeLoop(){
while(true){
uint8_t instruction = ROM[pc];
//flags first
switch(instruction){
    int opcode = (instruction & 0xF000);

    //1 byte instructional sets
    case (opcode == 0x00){
        opCodes::opcodeNop();
        pc+=1;
        cycle+=1;
        break;
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
