#ifndef CLOCK
#define CLOCK
#include "Accumulator.h"
#include "Audio.h"
#include "Interrupt.h"
class Clock {
    public: 
        static double machineCycle = 0;
        static double timer = 0;
        const double CLOCK_FREQ = 4194304.0;
        const double FRAME_RATE = 59.7275;
        const int CYCLES_PER_FRAME = CLOCK_FREQ / FRAME_RATE;
        static int CYCLES_ELAPSED;
        const double MACHINE_CYCLE = 1048574.0;
        Singleton& singleton = Singleton::getInstance();
        uint8_t* memory = singleton.getMemoryBus();
        std::unordered_map<char, Accumulator*> Accumulators; 


        Accumulator Pulse1Accumulator* = Accumulator('a', 0);
        Accumulator Pulse1SweepAccumulator* = Accumulator('b', 0);
        Accumulator Pulse1EnvelopeSweepAccumulator* = Accumulator('c', 0);
        Accumulator Pulse1LengthTimer* = Accumulator('d', 0);
        Accumulator Voluntary3LengthTimer* = Accumulator('e', 0);
        //TIMA
        Accumulator TimaAccummulator* = Accumulator("f", 0);
        Accumulator DividerAccummulator* = Accumulator("f", 0);
        Accumulator PPUCycleAccumulator* = Accumulator("h", 0);
        PPUCycleAccumulator.threshold = 456;  
        Pulse1LengthTimer.threshold = (CLOCK_FREQ/256);
        //instruction cycles take 1 - 5 machine cycles

        //SAME as clock
        const int APU_MASTER_CLOCK = 4194304.0;
        const double APU_FRAME_SEQUENCER = 512.0;
        const double ENVELOPE_STEP = 64.0;
        const double APU_LENGTH_COUNTER = 256.0;
        const double SQUARE_CHANNEL_SWEEP = 128.0;


        //SAME as clock
        const double LCD_CLOCK = 4194304.0;
        const double LCD_LINE = 9198.0;
        const double LCD_FULL_FRAME = 59.7275;
        
        double tick(double inc){
        timer+=inc;
        if (timer % 4 == 0){
            machineCycle+=1;
        }
        for (const auto& pair : Accumulators){
            //TIMA ACC
            if pair.first == 't':
                updateTimaAccumulator();
            pair.second.tick(inc); 
        }
        return timer;
        }
        double getTimer(){
            return timer;
        }

        auto updateTimaAccumulator(){
            auto clockSelect = memory[0xFF07] & 0b11;
            if (clockSelect == 0b00){
            auto accTicks = (CLOCK_FREQ / 268400);
            }
            if (clockSelect == 0b01){
            auto accTicks = (CLOCK_FREQ / 67110);
            }
            if (clockSelect == 0b10){
            auto accTicks = (CLOCK_FREQ / 16780);
            }
            if (clockSelect == 0b11){
            auto accTicks = (CLOCK_FREQ / 4194);
        }

        auto updatePPUAccumulator(){
            int dots = (int)PPUCycleAccumulator.value % 456;
            int scanline = (int)(PPUCycleAccumulator.value / 456) % 154;
            uint8_t mode = 0;
            
            if (scanline >= 144) {
                mode = 1; // VBlank
            } else if (dots < 80) {
                mode = 2; // OAM search
            } else if (dots < 252) { // 80 + 172
                mode = 3; // Transfer to LCD
            } else {
                mode = 0; // HBlank
            }
            
            uint8_t STAT = memory[0xFF41];
            STAT = (STAT & 0xFC) | (mode & 0x03); // Clear bits 0-1, set new mode
            memory[0xFF41] = STAT;
        }

    private:
        Clock() {
            timer = 0;
        }


};
}
#endif