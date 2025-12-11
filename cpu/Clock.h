
#ifndef CLOCK
#define CLOCK
class Clock {
    static double machineCycle = 0;
    static double timer = 0;
    const double CLOCK_FREQ = 4194304.0;
    const double FRAME_RATE = 59.7275;
    const int CYCLES_PER_FRAME = CLOCK_FREQ / FRAME_RATE;
    static int CYCLES_ELAPSED;
    const double MACHINE_CYCLE = 1048574.0;
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
        return timer;
    }
    double getTimer(){
        return timer;
    }

    private:
        Clock() {
            timer = 0;
        }
};

#endif