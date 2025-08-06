#include <singleton.h>

class MemoryBus {
Singleton& singleton = singleton::getInstance();


//CPU Memory Bus
    struct MemoryBus {
        uint8_t memory[0XFFFF];
    } memorybus;


//addys for grahpics 

//$8000 - 97FF is VRAM each tile takes up 16 bytes

//vram 0x8000 - 0x9FFF
//$8000 addressing 0 - 127 in block 0 and 128 - 255 in block 1
//$8800 - $9000 is base pointer, signed -128 to -1, 0 - 127

//tile maps, are references to tiles, not directly written

//window is a secondary background layer, only the top left pixel is controllable
//it is often used for fixed ui elements like status bars 

//tiles are 8 by 8 blocks
//object - an object attribute memory, composed of one or two tiles and can move independently


//first byte is least sig, second is most for color coding
//2 bit colors means 4 possible colors
//00 - white, 01 - light gray, 10 - dark gray, 11 - black


uint8_t& getROM(uint8_t address) {
    return memorybus.memory[address];
}

uint8_t& getRAM(uint8_t address) {
    return memorybus.memory[address];
}

uint8_t& getVRAM(uint8_t address) {
    return memorybus.memory[address];
}


}