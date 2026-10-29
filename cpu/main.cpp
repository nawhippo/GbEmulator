#include "Audio.h"
#include "Clock.h"
#include "Singleton.h"
#include "Registers.h"
#include "OpcodeHandler.h"

int main() {
    Singleton singleton = new singleton();
    //load rom i am assuming
    
    while(true){
        OpcodeHandler::executeInstruction();
    }
}