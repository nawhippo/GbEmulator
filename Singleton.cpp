#include <cstdint>
#include <algorithm>

class Singleton {
Register register;
uint8_t ROM[256];
uint8_t RAM[256];

public:


Singleton(Singleton const&) = delete;
void operator=(Singleton const&) = delete;

static Singleton& getInstance(){
    static Singleton instance;
    return instance;
}




private:

Singleton(Register newregister, uint8_t ram[256], uint8_t rom[256]){
    std::copy(rom, rom + 256, ROM);
    register->newregister; 
    std::copy(ram, ram + 256, RAM);
    }

Singleton(){}



};