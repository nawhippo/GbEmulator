#include <singleton.h>
#include <SDL2.h>

class audio {
      int8_t* memory = singleton.getInstance().getMemoryBus;
       int8_t* audioMaster = &memory[0xFF26];
       int8_t* soundPanning = &memory[0XFF25];
       int8_t* MasterVolumeVIN = &memory[0xFF24];
       int8_t* Channel1Sweep = &memory[0xFF10];
       int8_t* Channel1LengthDutyTimer = &memory[0xFF11];
       int8_t* Channel1VolumeEnvelope = &memory[0xFF12];
       int8_t* Channel1PeriodLow = &memory[0xFF13];
       int8_t* Channel1PeriodHigh = &memory[0xFF14];


       int8_t* Channel3DACEnable = &memory[0xFF1A];
       int8_t* Channel3LengthTimer = &memory[0xFF1B];
       int8_t* Channel3OutputLevel = &memory[0xFF1C];
       int8_t* Channel3PeriodLow = &memory[0xFF1D];
       int8_t* Channel3PeriodHigh = &memory[0xFF1E];

       int8_t* Channel4LengthTimer = &memory[0xFF20];
       int8_t* Channel4VolumeEnvelope = &memory[0xFF21];
       int8_t* Channel4Frequency = &memory[0xFF22];
       int8_t* Channel4Control = &memory[0xFF23];
       //16 bytes - each byte holds 2 samples - upper nibble first FF30 Upper -> FF30 Lower -> FF31 Upper

       int8_t* WavePatterRAM = &memory[0xFF30];

       
        //PUT DATA OUTPUT DATA 32 4 bit samples first 3 input, last 3 output 
       uint_16 SAMPLE_RATE = getPeriodValue();
       //what frequency is the output supposesd to be 

       SDL_AudioStream *channel3stream = SDL_NewAudioStream(AUDIO_u32, 1, SAMPLE_RATE, 22050, AUDIOF32,2, 48000);
    
 

    int getPeriodValue(){
        uint16_t periodValue =  ((((memory[FF1E] << 5) & 0b11100000) & (memory[FF1D] & 0xFF)) & 0b0000011111111111);
    }
}  