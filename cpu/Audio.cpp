#include <singleton.h>
#include <SDL2.h>

class audio {
       uint8_t* channel1ShadowPeriod = singleton.getInstance().getChannel1ShadowPeriod();
       uint8_t* memory = singleton.getInstance().getMemoryBus();
       uint8_t* audioMaster = &memory[0xFF26];
       uint8_t* soundPanning = &memory[0XFF25];
       uint8_t* MasterVolumeVIN = &memory[0xFF24];
       uint8_t* Channel1Sweep = &memory[0xFF10];
       uint8_t* Channel1LengthDutyTimer = &memory[0xFF11];
       uint8_t* Channel1VolumeEnvelope = &memory[0xFF12];

       //WRITE ONLY
       uint8_t* Channel1PeriodLow = &memory[0xFF13];
       uint8_t* Channel1PeriodHigh = &memory[0xFF14];


       uint8_t* Channel3DACEnable = &memory[0xFF1A];
       uint8_t* Channel3LengthTimer = &memory[0xFF1B];
       uint8_t* Channel3OutputLevel = &memory[0xFF1C];
       uint8_t* Channel3PeriodLow = &memory[0xFF1D];
       uint8_t* Channel3PeriodHigh = &memory[0xFF1E];

       uint8_t* Channel4LengthTimer = &memory[0xFF20];
       uint8_t* Channel4VolumeEnvelope = &memory[0xFF21];
       uint8_t* Channel4Frequency = &memory[0xFF22];
       uint8_t* Channel4Control = &memory[0xFF23];
       //16 bytes - each byte holds 2 samples - upper nibble first FF30 Upper -> FF30 Lower -> FF31 Upper

       uint8_t* WavePatternRAM = &memory[0xFF30];

       
        //PUT DATA OUTPUT DATA 32 4 bit samples first 3 input, last 3 output 

       uint16_t periodValue1 = getPeriodValueChannel1();
       uint16_t periodValue2 = getPeriodValueChannel2();
       uint16_t periodValue3 = getPeriodValueChannel3();
       //what frequency is the output supposesd to be 

       //period value is decremented and when its zero move onto the next sample
       //period value is just an interval tracker 
       //pulse channel 3 freq 


       uint16_t sampleRate1 = (1048576/(2048 - periodValue1));
       uint16_t sampleRate2 = (1048576/(2048 - periodValue2));
       uint16_t sampleRate3 = (1048576/(2048 - periodValue3));

       
       uint16_t freq1 = (131072/(2048 - periodValue1));
       uint16_t freq2 = (131072/(2048 - periodValue2));
       uint16_t freq3 = (131072/(2048 - periodValue2));

       //audio is clamped to maximum 32 unsigned 4,294,967,295
       SDL_AudioStream *channel1Stream = SDL_NewAudioStream(AUDIO_u32, 1, sampleRate1, AUDIO_u32, 2, sampleRate1);
       SDL_AudioStream *channel2Stream = SDL_NewAudioStream(AUDIO_u32, 1, sampleRate2, AUDIO_u32, 2, sampleRate2);
       SDL_AudioStream *channel3Stream = SDL_NewAudioStream(AUDIO_u32, 1, sampleRate3, AUDIO_u32, 2, sampleRate3);
       SDL_AudioStream *channel4Stream = SDL_NewAudioStream(AUDIO_u32, 1, sampleRate4, AUDIO_u32, 2, sampleRate4);
       //how to write to buffer;

       uint16_t buffer;
       //whats the sample rate
       uint8_t *output;
       static int out_bytes;       
       //evidentally these streams should all be put into one pcm buffer - but to do that we have to handle timing stuff....
       //mixing samples means adding them together.

       //SDL_PutAudioStreamData - this is where we pull data from the our analog sources
       //sdl_getaudiostreamdata - this is where we get the formatted data to combine in our mixed audio
       
    




    //channel 1
    uint16_t getPeriodValueChannel1(){
        //upper 3 bits of high, and all of low - eleven bit value
        //these are write only
        uint16_t periodValue = (((memory[0xFF14] & 0b00000111) << 8) | (memory[0xFF13] & 0xFF)) & 0b0000011111111111;
        return periodValue;
    }

    uint16_t generatePulseWaveChannel1(){
    //values are copied from shadow memory (considering we cannot create hardware that does pulses and the registers are read only)
    //ff10 -> Pace 654 - Direction 3 - Individual Step 2 1 0
    //pace is uinits of 128 hz 
    //direction - 0 is add 1 is sub
    //sweep step - shift right before adding/subtracting
    //sweep timer counter
    //enabled
    //step calc
    uint8_t initialPeriod = 0;
    uint8_t* channel1Behavior = memory[0xFF10];
    int offset = 0;
    int step = (channel1Behavior & 0b111);
    uint16_t period = channel1ShadowPeriod;
    uint16_t direction = (channel1Behavior >> 3) & 0b1;
    uint16_t periodUpdate = 0;
    if (direction == 1){
        periodUpdate = period - (period / (0b1 << step));
    } else {
        periodUpdate = period + (period / (0b1 << step));
    }
    //period in NR13 AND NR14 are modified and rewritten - cant read em tho
    uint8_t periodLow = periodUpdate & (0xFF);
    uint8_t periodHigh = ((periodUpdate >> 8) & 0b111);
    *Channel1PeriodHigh = ((Channel1PeriodHigh & 0b11111000) | periodHigh);
    *Channel1PeriodLow = static_cast<uint8_t>(periodUpdate);
    channel1ShadowPeriod = periodUpdate;

    int len;
    //how often to grab info from data? sample rate
    //clock / 4 tick = 1048576 hz subdivision length 
    //when timer reaches zero (subdiv - period) -> next step in 8 step duty waveform is outputted sample rate is effectively
    //the resulting tone frequency - 131072 / (2048 - period_value)
    //i need raw audio info to put in the stream and modify.
    //SDL_AudioStreamPut(channel1Stream);

    //duty cycle clocking is delayed until the first trigger.
    userdata;


    SDL_AudioCallback();
    }


    uint8_t pulseCallBack(void* userdata, uint8_t* audioStream, int len){
        SDL_AudioStream* stream = <static_cast>(SDL_AudioStream*)userdata;
        int data = SDL_GetAudioStreamData(s, stream, len);
        if (data < len){
            memset(stream + data, 0, len - data);
    }
    }

    //channel2
    uint16_t getPeriodValueChannel2(){
        //upper 3 bits of high, and all of low - eleven bit value
        uint16_t periodValue = (((memory[0xFF19] & 0b00000111) << 8) | (memory[0xFF18] & 0xFF)) | 0b0000011111111111;
        return periodValue;
    }

    //channel 3
    uint16_t getPeriodValueChannel3(){
        uint16_t periodValue = ((((memory[0xFF1E] << 5) & 0b11100000) & (memory[0xFF1D] & 0xFF)) & 0b0000011111111111);
        return periodValue;
    }




    uint16_t handleAudioMixing(){
        uint8_t bufferlen = 4096;
        //128mhz sweep maybe incrementing every tick
        //check if channels are on 
        bool channel1On = (audioMaster | 0b1);
        bool channel2On = ((audioMaster >> 1)| 0b1);
        bool channel3On = ((audioMaster >> 2)| 0b1);
        bool channel4On = ((audioMaster >> 3)| 0b1);
        uint8_t *outputbuffer;
        if (channel1On){
            outputbuffer*+=channel1stream;
        }
        if (channel2On){
            outputbuffer*+=channel2stream;
        }
        if (channel3On){
            outputbuffer*+=channel3stream;
        }
        if (channel4On){
            outputbuffer*+=channel4stream;
        }
        
        SDL_AudioStreamPut(outputbuffer, bufferlen);

        














    }


    //64hz envelope sweep rate of 8
    //256hz sound length rate of 2 
    //128 hz - ch1 freq rate of 4 
    int APUStep(){
        //audio sync? universal step timer
        
    }
}  