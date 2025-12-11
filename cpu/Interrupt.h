#include <singleton.h>
#define INTERRUPT

#ifndef
class Interrupt {
Singleton singleton = Singleton.getInstance();
uint8_t& memory = singleton.getMemoryBus();
uint8_t& interruptEnable = singleton.getInterruptEnable();
bool IME = singleton.getIME();
uint8_t& IF = singleton.getIF();
uint8_t& IE = singleton.getIE();



int checkInterruptTrigger(){
    if (IME){
        uint8_t interrupt = IF & IE;
        if (interrupt & 0x01){
            triggerVBlankInterrupt();
        }   
        if (interrupt & 0x02) {
            triggerLCDInterrupt();
        }
        if (interrupt & 0x04){
            triggerTimerInterrupt();
        }
        if (interrupt & 0x08){
            triggerSerialInterrupt();
        }
        if (interrupt & 0x10){
            triggerJoypadInterrupt();
        }
    }


int triggerVBlankInterrupt(){
bool enabled = ((interruptEnable >> 7) & 0b1);
if (!enabled){
    cout << "VBLANK INTERRUPT NOT ENABLED";
    return -1;
}


}

int triggerLCDInterrupt(){
bool enabled = ((*interruptEnable >> 6) & 0b1);
if (!enabled){
    cout << "LCD INTERRUPT NOT ENABLED";
    return -1;
}
}

int triggerTimerInterrupt(){
bool enabled = ((*interruptEnable >> 5) & 0b1);
if (!enabled){
    cout << "TIMER INTERRUPT NOT ENABLED";
    return -1;
}
}


int triggerSerialInterrupt(){
bool enabled = ((*interruptEnable >> 4) & 0b1); 
if (!enabled){
    cout << "SERIAL INTERRUPT NOT ENABLED";
    return -1;
}
}


int triggerSTATInterrupt(){
if (!enabled){
    cout << "STAT INTERRUPT NOT ENABLED";
    return -1;
}
}




int triggerJoypadInterrupt(){
}
}
}
#endif