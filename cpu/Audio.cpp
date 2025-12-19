#include <singleton.h>
#include <SDL2.h>
#include "accumulator.h"

class audio {
    static const int AUDIO_BUFFER_SIZE = 1024;
    int16_t bufferChannel1[AUDIO_BUFFER_SIZE] = {0};
    int16_t bufferChannel2[AUDIO_BUFFER_SIZE] = {0};
    int16_t bufferChannel3[AUDIO_BUFFER_SIZE] = {0};
    int16_t bufferChannel4[AUDIO_BUFFER_SIZE] = {0};
    int bufferIndexChannel1 = 0;
    int bufferIndexChannel2 = 0;
    int bufferIndexChannel3 = 0;
    int bufferIndexChannel4 = 0;

    int phaseStep = 0;







    struct PulseChannel1State {
        int step;
        uint8_t dutyIndex;
        uint8_t dutyTable[8];
        Accumulator& phaseAccumulator = Clock.Accumulators['a'];
        uint16_t& shadowFrequency = Singleton.getChannel1ShadowFrequency(); // changed from reference to plain uint16_t
        uint8_t sweepShift;
        uint8_t sweepDirection;
        Accumulator& sweepAccumulator = Clock.Accumulators['b'];
        uint8_t volume;
        uint8_t envelopePeriod;
        uint32_t envelopeAccumulator;
        bool channelEnabled;

    };



      




















       const int SDL_SAMPLE_RATE = 44100;
       int* channel1PhaseStep = 0;
       uint8_t* channel1Frequency = singleton.getInstance().getChannel1Frequency();
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

       uint16_t periodValue2 = getPeriodValueChannel2();
       uint16_t periodValue3 = getPeriodValueChannel3();
       //what frequency is the output supposesd to be 

       //period value is decremented and when its zero move onto the next sample
       //period value is just an interval tracker 
       //pulse channel 3 freq 


       uint16_t sampleRate2 = (1048576/(2048 - periodValue2));
       uint16_t sampleRate3 = (1048576/(2048 - periodValue3));

       
    //    uint16_t freq1 = (131072/(2048 - periodValue1));
       uint16_t freq2 = (131072/(2048 - periodValue2));
       uint16_t freq3 = (131072/(2048 - periodValue2));

       //audio is clamped to maximum 32 unsigned 4,294,967,295
       //sample rates in the system vary, so we have to have constant sdl sample rates.
       //this is for converting audio formats automatically - rather than directly writing to a buffer - not without latency unfortunately.
        SDL_AudioStream *channel1Stream = SDL_NewAudioStream(AUDIO_u32, 1, SDL_SAMPLE_RATE, AUDIO_u32, 2, SDL_SAMPLE_RATE);
    //    SDL_AudioStream *channel2Stream = SDL_NewAudioStream(AUDIO_u32, 1, SDL_SAMPLE_RATE, AUDIO_u32, 2, SDL_SAMPLE_RATE);
    //    SDL_AudioStream *channel3Stream = SDL_NewAudioStream(AUDIO_u32, 1, SDL_SAMPLE_RATE, AUDIO_u32, 2, SDL_SAMPLE_RATE);
    //    SDL_AudioStream *channel4Stream = SDL_NewAudioStream(AUDIO_u32, 1, SDL_SAMPLE_RATE, AUDIO_u32, 2, SDL_SAMPLE_RATE);
       //how to write to buffer;

       uint16_t buffer;
       //whats the sample rate
       uint8_t *output;
       static int out_bytes;       
       //evidentally these streams should all be put into one pcm buffer - but to do that we have to handle timing stuff....
       //mixing samples means adding them together.

       //SDL_PutAudioStreamData - this is where we pull data from the our analog sources
       //sdl_getaudiostreamdata - this is where we get the formatted data to combine in our mixed audio
    


    //this needs to be called every time the sdl callback is called - so the fixed SDL sample rate. return the current phase step every time sdl pings it
    uint16_t generatePulseWaveChannel1(){
        uint8_t* duty = &memory[0xFF11];
        int dutytableindex = (duty >> 5) & 0b11;
        //this is because of the 8 steps - 1 step is a frequency timer period
        const std::unordered_map<uint8_t, std::array<uint8_t, 8>> dutyTable = {
        { 0b00, { 0,0,0,0,0,0,0,1 } }, // 12.5%
        { 0b01, { 0,0,0,0,0,0,1,1 } }, // 25%
        { 0b10, { 0,0,0,0,1,1,1,1 } }, // 50%
        { 0b11, { 1,1,1,1,1,1,0,0 } }  // 75%
        };
        uint8_t* channel1Behavior = memory[0xFF10];
        uint16_t frequency = getFrequencyChannel1();
        // Assign to PulseChannel1State fields
        pulseState.dutyIndex = step % 8;
        // Copy the duty table for the current duty cycle
        // envelopeAccumulator and channelEnabled would be updated elsewhere as needed
        bool on = pulseState.dutyTable[pulseState.dutyIndex];
        auto sample = on * amplitude;

        if (pulseState.sweepAccumulator.trigger){
            uint8_t sweepPeriod = (channel1Behavior >> 4) & 0b111;
            if (sweepPeriod == 0){
                sweepPeriod = 8;
            }
            uint16_t frequency = getFrequencyChannel1();
            uint8_t direction = (channel1Behavior >> 3) & 0b1;
            uint8_t sweepshift = (channel1Behavior) & 0b111;
            uint16_t alter = frequency >> sweepshift;
            uint16_t frequencyupdate;
            if (direction){
                frequencyupdate = frequency - alter;
            } else {
                frequencyupdate = frequency + alter;
            }
            // Assign all relevant values to pulseState
            pulseState.sweepPeriod = sweepPeriod;
            pulseState.sweepShift = sweepshift;
            pulseState.sweepDirection = direction;
            pulseState.frequencyUpdate = frequencyupdate;
            pulseState.shadowFrequency = frequencyupdate;
            pulseState.sweepAccumulator.threshold = sweepPeriod * 32768;
            pulseState.sweepAccumulator.trigger = false; // Reset trigger after handling
            pulseState.audioOutputFrequency = 131072 / (2048 - frequencyupdate);
        }
        if (clock.phaseAccumulator.trigger){
            phaseStep = (phaseStep + 1) % 8;
            clock.phaseAccumulator.threshold = (2048 - frequency) * 4;
            clock.phaseAccumulator.trigger.reset(); // Reset trigger after handling
        }
        //sound should change evey ~ 7.8ms
        //values are copied from shadow memory (considering we cannot create hardware that does pulses and the registers are read only)
        //ff10 -> phase sweep pace 654 (timer for phases)
        //direction - 0 is add 1 is sub
        //sweep step - shift right before adding/subtracting delta
        bool on = pulseState.dutyTable[pulseState.dutyIndex];
        auto sample = on * amplitude;
        return sample;
    }

    //keep in mind this IS signhed
    uint8_t audioCallbackChannel1(void* userdata, uint8_t* audioStream, int len) {
        int16_t* out = (int16_t*)stream;
        int num_samples = len / sizeof(int16_t);
        for (int i = 0; i < num_samples; ++i) {
            out[i] = generatePulseWaveChannel1();
        }
}   



    //channel2
    uint16_t getFrequencyValueChannel2(){
        //upper 3 bits of high, and all of low - eleven bit value
        uint16_t periodValue = (((memory[0xFF19] & 0b00000111) << 8) | (memory[0xFF18] & 0xFF)) | 0b0000011111111111;
        return periodValue;
    }

    //channel 3
    uint16_t getFrequencyValueChannel3(){
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


    void openChannel1AudioDevice(){
        SDL_AudioSpec want, have;
        SDL_zero(want);
        want.freq = 44100;                    // sample rate
        want.format = AUDIO_S16SYS;           // signed 16-bit samples
        want.channels = 1;                    // mono
        want.samples = 1024;                  // buffer size in frames
        want.callback = audio_callback;       // your function
        want.userdata = &PulseChannel1State;       // pass a pointer to your state
        SDL_AudioDeviceID channel1dev = SDL_OpenAudioDevice(
            nullptr,       // default device
            0,             // 0 = playback
            &want,
            &have,
            0              // allow format changes = 0 means "no"
        );

        if (dev == 0) {
            SDL_Log("Failed to open audio: %s", SDL_GetError());
            return;
        }
    }


    void openChannel2AudioDevice(){
        SDL_AudioSpec want, have;
        SDL_zero(want);
        want.freq = 44100;                    // sample rate
        want.format = AUDIO_S16SYS;           // signed 16-bit samples
        want.channels = 1;                    // mono
        want.samples = 1024;                  // buffer size in frames
        want.callback = audio_callback;       // your function
        want.userdata = &PulseChannel2State;       // pass a pointer to your state
        SDL_AudioDeviceID devchannel2dev = SDL_OpenAudioDevice(
            nullptr,       // default device
            0,             // 0 = playback
            &want,
            &have,
            0              // allow format changes = 0 means "no"
        );

        if (dev == 0) {
            SDL_Log("Failed to open audio: %s", SDL_GetError());
            return;
        }
    }

        void openChannel3AudioDevice(){
        SDL_AudioSpec want, have;
        SDL_zero(want);
        want.freq = 44100;                    // sample rate
        want.format = AUDIO_S16SYS;           // signed 16-bit samples
        want.channels = 1;                    // mono
        want.samples = 1024;                  // buffer size in frames
        want.callback = audio_callback;       // your function
        want.userdata = &PulseChanne13State;       // pass a pointer to your state
        SDL_AudioDeviceID devchannel3dev = SDL_OpenAudioDevice(
            nullptr,       // default device
            0,             // 0 = playback
            &want,
            &have,
            0              // allow format changes = 0 means "no"
        );

        if (dev == 0) {
            SDL_Log("Failed to open audio: %s", SDL_GetError());
            return;
        }
    }

        void openChannel4AudioDevice(){
        SDL_AudioSpec want, have;
        SDL_zero(want);
        want.freq = 44100;                    // sample rate
        want.format = AUDIO_S16SYS;           // signed 16-bit samples
        want.channels = 1;                    // mono
        want.samples = 1024;                  // buffer size in frames
        want.callback = audio_callback;       // your function
        want.userdata = &PulseChannel4State;       // pass a pointer to your state
        SDL_AudioDeviceID devchannel4dev = SDL_OpenAudioDevice(
            nullptr,       // default device
            0,             // 0 = playback
            &want,
            &have,
            0              // allow format changes = 0 means "no"
        );

        if (dev == 0) {
            SDL_Log("Failed to open audio: %s", SDL_GetError());
            return;
        }
    }

        void openChannel5AudioDevice(){
        SDL_AudioSpec want, have;
        SDL_zero(want);
        want.freq = 44100;                    // sample rate
        want.format = AUDIO_S16SYS;           // signed 16-bit samples
        want.channels = 1;                    // mono
        want.samples = 1024;                  // buffer size in frames
        want.callback = audio_callback;       // your function
        want.userdata = &PulseChannel1State;       // pass a pointer to your state
        SDL_AudioDeviceID devchannel5dev = SDL_OpenAudioDevice(
            nullptr,       // default device
            0,             // 0 = playback
            &want,
            &have,
            0              // allow format changes = 0 means "no"
        );

        if (dev == 0) {
            SDL_Log("Failed to open audio: %s", SDL_GetError());
            return;
        }
    }
    //64hz envelope sweep rate of 8
    //256hz sound length rate of 2 
    //128 hz - ch1 freq rate of 4 
    int APUStep(){
        //audio sync? universal step timer
        clock.Accumulators['a'].tick(1); // or whatever increment is appropriate
        clock.Accumulators['b'].tick(1);
    }
}