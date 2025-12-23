#include <singleton.h>
#include <SDL2.h>
#include "accumulator.h"

class audio {
    uint8_t* memory = Singleton.getInstance.getMemoryBus();
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
    Clock& clock = Singleton.getInstance.getClock();
    const std::unordered_map<uint8_t, std::array<uint8_t, 8>> dutyTable = {
        { 0b00, { 0,0,0,0,0,0,0,1 } }, // 12.5%
        { 0b01, { 0,0,0,0,0,0,1,1 } }, // 25%
        { 0b10, { 0,0,0,0,1,1,1,1 } }, // 50%
        { 0b11, { 1,1,1,1,1,1,0,0 } }  // 75%
        };

    // 1. Pulse Channel 1
    struct PulseChannel1State {
        int active;
        int step = 0;
        uint8_t dutyIndex;
        //this is the period divider
        Accumulator& phaseAccumulator = Clock.Accumulators['a'];
        uint16_t& shadowFrequency = Singleton.getChannel1ShadowFrequency();
        uint8_t sweepShift;
        uint8_t sweepDirection;
        Accumulator& sweepAccumulator = Clock.Accumulators['b'];
        Accumulator& envelopeSweepAccumulator = Clock.Accumulators['c'];
        Acuumulator& lengthAccumulator = Clock.Accumulators['d'];
        //initial volume
        uint8_t volume = (memory[0xFF12] >> 4) & 0xF;
        uint8_t envelopePeriod;
        uint32_t envelopeAccumulator;
        bool channelEnabled;
        uint8_t amplitude;
        uint8_t envelope;
        bool lengthEnable;
        int lengthTimer = 64;
        // Add shadow trigger for channel 1
        uint8_t& shadowTrigger = Singleton.getInstance().getShadowTriggerChannel1();
    };

    // 2. Pulse Channel 2
    struct PulseChannel2State {
        int step = 0;
        uint8_t dutyIndex;
        Acuumulator& lengthAccumulator = Clock.Accumulators['f'];
        Accumulator& phaseAccumulator = Clock.Accumulators['g'];
        uint16_t& shadowFrequency = Singleton.getInstance().getChannel2ShadowFrequency(); 
        //initial volume
        uint8_t volume = (memory[0xFF17] >> 4) & 0xF;
        uint8_t envelopePeriod;
        uint32_t envelopeAccumulator;
        bool channelEnabled;
        uint8_t amplitude;
        uint8_t envelope;
        Accumulator& lengthAccumulator = Clock.Accumulators['h'];
        // Add shadow trigger for channel 2
        uint8_t& shadowTrigger = Singleton.getInstance().getShadowTriggerChannel2();
    };

    // 3. Voluntary Channel 3
    struct VoluntaryChannel3State {
        //affected by output level 0xFF1C
        int active;
        //addressing ram
        int step = 0;
        Accumulator& phaseAccumulator = Clock.Accumulators['j']; 
        Accumulator& lengthAccumulator = Clock.Accumulators['k'];
        uint16_t& shadowFrequency = Singleton.getInstance().getChannel3ShadowFrequency(); 
        int volume = (memory[0xFF1C] >> 4) & 0b11;
        bool on = memory[0xFFA1] >> 6 & 0b1; 
        uint8_t& lengthTimer = Singleton.getInstance().getChannel3LengthTimer();
        uint8_t length;
        uint8_t& shadowTrigger = Singleton.getInstance().getShadowTriggerChannel3();
        uint8_t* amplitude;
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
    int16_t generatePulseWaveChannel1() {
        // Duty cycle from FF11 (bits 6-7)
        uint8_t duty = (memory[0xFF11] >> 6) & 0b11;
        // Length trigger and enable from FF14
        bool lengthTrigger = (memory[0xFF14] >> 6) & 0b1;
        bool lengthEnable = (memory[0xFF14] >> 5) & 0b1;
    
        // --- Begin trigger logic for channel 1 ---
        //this activates when writing to the trigger channel
        if (PulseChannel1State.shadowTrigger) {
            PulseChannel1State.active = 1;
            PulseChannel1State.lengthTimer = 1;
            // Set enabled flag or clear it
            if (PulseChannel1State.active == 0) {
                PulseChannel1State.active = 1;
            } else {
                PulseChannel1State.active = 0;
            }
            uint16_t freq = getPeriodChannel1();
            uint8_t sweepPeriod = (*Channel1Sweep >> 4) & 0b111;
            uint8_t sweepShift = (*Channel1Sweep) & 0b111;
            uint8_t sweepDirection = (*Channel1Sweep >> 3) & 0b1;
            uint16_t sweepDelta = freq >> sweepShift;
            uint16_t newFreq;
            if (sweepDirection) {
                newFreq = freq - sweepDelta;
            } else {
                newFreq = freq + sweepDelta;
            }
            // overflow check
            if (newFreq > 2047) {
                PulseChannel1State.active = 0;
            } else {
                PulseChannel1State.shadowFrequency = newFreq;
            }
            PulseChannel1State.phaseAccumulator.threshold = (2048 - newfreq) * 4;
            PulseChannel1State.volume = (memory[0xFF12] >> 4) & 0xF;
            PulseChannel1State.envelopeSweepAccumulator.reset();
            PulseChannel1State.shadowFrequency = getPeriodChannel1();
            PulseChannel1State.shadowTrigger = 0; // Reset shadow trigger after handling
        }
        if (lengthEnable) {
            if (PulseChannel1State.lengthAccumulator.trigger && PulseChannel1State.lengthTimer > 0) {
                PulseChannel1State.lengthTimer -= 1;
                if (PulseChannel1State.lengthTimer == 0) {
                    PulseChannel1State.active = 0;
                }
            }
        }
        // Envelope logic
        uint8_t envelope = memory[0xFF12];
        uint8_t initialVolume = (envelope >> 4) & 0xF;
        bool envelopeDirection = (envelope >> 3) & 0x1;
        uint8_t envelopePeriod = envelope & 0x7;
        if (PulseChannel1State.step == 0) {
            PulseChannel1State.volume = initialVolume;
        }

        if (PulseChannel1State.sweepAccumulator.trigger) {
            uint8_t sweepPeriod = (*Channel1Sweep >> 4) & 0b111;
            uint8_t sweepShift = (*Channel1Sweep) & 0b111;
            uint8_t sweepDirection = (*Channel1Sweep >> 3) & 0b1;
            uint16_t freq = PulseChannel1State.shadowFrequency;
            uint16_t sweepDelta = freq >> sweepShift;
            uint16_t newFreq;
            if (sweepDirection) {
                newFreq = freq - sweepDelta;
            } else {
                newFreq = freq + sweepDelta;
            }
            // overflow check
            if (newFreq > 2047) {
                PulseChannel1State.active = 0;
            } else {
                PulseChannel1State.shadowFrequency = newFreq;
            }
            // Set threshold for next sweep step
            PulseChannel1State.sweepAccumulator.threshold = sweepPeriod * 65536;
            PulseChannel1State.sweepAccumulator.trigger = false;
        }

        // Envelope sweep accumulator logic
        if (PulseChannel1State.envelopeSweepAccumulator.trigger) {
            if (envelopePeriod > 0) {
                if (envelopeDirection) {
                    if (PulseChannel1State.volume < 15) {
                        PulseChannel1State.volume += 1;
                    }
                } else {
                    if (PulseChannel1State.volume > 0) {
                        PulseChannel1State.volume -= 1;
                        if (PulseChannel1State.volume == 0) {
                            PulseChannel1State.active = 0;
                        }
                    }
                }
            }
            PulseChannel1State.amplitude = (int16_t)(PulseChannel1State.volume * 32767);
            PulseChannel1State.envelopeSweepAccumulator.threshold = envelopePeriod * 65536;
            PulseChannel1State.envelopeSweepAccumulator.trigger = false;
        } else {
            PulseChannel1State.amplitude = (int16_t)(PulseChannel1State.volume * 32767);
        }

        uint16_t frequency = getPeriodChannel1();
        if (PulseChannel1State.phaseAccumulator.trigger) {
            PulseChannel1State.dutyIndex = PulseChannel1State.step % 8;
            PulseChannel1State.step = (PulseChannel1State.step + 1) % 8;
            Pulse1Channel1State.phaseAccumulator.reset();
            //11 bit max number is 2048 - we count up to 2048 and multiply by four since the audio is a forth the speed of the cpu (we are basesd off cpu instructions)
            Pulse1Channel1State.phaseAccumulator.threshold = (2048 - *channel1ShadowFrequency) * 4;
        }
        // Only update sample if active
        bool on = dutyTable.at(duty)[PulseChannel1State.dutyIndex];
        int16_t sample = (PulseChannel1State.active && on) ? PulseChannel1State.amplitude : 0;
        return sample;
    }

    //the frequency is the difference between calls happening very fast.
    uint16_t generatePulseWaveChannel2() {
        // Duty cycle from FF16 (bits 6-7)
        uint8_t duty = memory[0xFF16];
        int dutytableindex = (duty >> 6) & 0b11;
        // Envelope from FF17
        uint8_t envelope = memory[0xFF17];
        // Frequency: upper 3 bits of FF19, all of FF18
        uint16_t frequency = getPeriodChannel2();

        // --- Begin trigger logic for channel 2 ---
        if (PulseChannel2State.shadowTrigger) {
            PulseChannel2State.active = 1;
            PulseChannel2State.lengthTimer = 1;
            // Set enabled flag or clear it
            if (PulseChannel2State.active == 0) {
                PulseChannel2State.active = 1;
            } else {
                PulseChannel2State.active = 0;
            }
            PulseChannel2State.volume = (memory[0xFF17] >> 4) & 0xF;
            PulseChannel2State.envelopeSweepAccumulator.reset();
            PulseChannel2State.shadowFrequency = frequency;
            PulseChannel2State.shadowTrigger = 0; 
        }
        uint8_t lengthEnable = (memory[0xFF19] >> 6) & 0b1;
        // --- Length logic for channel 2 (like channel 1) ---
        if (lengthEnable) {
            if (PulseChannel2State.lengthAccumulator.trigger && PulseChannel2State.lengthTimer > 0) {
                PulseChannel2State.lengthTimer -= 1;
                if (PulseChannel2State.lengthTimer == 0) {
                    PulseChannel2State.active = 0;
                }
            }
        }
        // Envelope logic (with sweep timing, similar to channel 1)
        uint8_t initialVolume = (envelope >> 4) & 0xF;
        bool envelopeDirection = (envelope >> 3) & 0x1;
        uint8_t envelopePeriod = envelope & 0x7;
        if (PulseChannel2State.step == 0) {
            PulseChannel2State.volume = initialVolume;
        }

        // Envelope sweep accumulator logic
        if (PulseChannel2State.envelopeSweepAccumulator.trigger) {
            if (envelopePeriod > 0) {
                if (envelopeDirection) {
                    if (PulseChannel2State.volume < 15) {
                        PulseChannel2State.volume += 1;
                    }
                } else {
                    if (PulseChannel2State.volume > 0) {
                        PulseChannel2State.volume -= 1;
                        if (PulseChannel2State.volume == 0) {
                            PulseChannel2State.active = 0;
                        }
                    }
                }
            }
            PulseChannel2State.amplitude = (int16_t)(PulseChannel2State.volume * 32767);
            // Set threshold for next envelope step (same as channel 1, but for channel 2)
            PulseChannel2State.envelopeSweepAccumulator.threshold = envelopePeriod * 65536;
            PulseChannel2State.envelopeSweepAccumulator.reset();
        } else {
            PulseChannel2State.amplitude = (int16_t)(PulseChannel2State.volume * 32767);
        }

        // --- Tie waveform step to phase accumulator trigger ---
        if (PulseChannel2State.phaseAccumulator.trigger) {
            PulseChannel2State.dutyIndex = PulseChannel2State.step % 8;
            PulseChannel2State.step = (PulseChannel2State.step + 1) % 8;
            PulseChannel2State.reset();
        }
        bool on = dutyTable.at(dutytableindex)[PulseChannel2State.dutyIndex];
        auto sample = (PulseChannel2State.active && on) ? PulseChannel2State.amplitude : 0;
        return sample;
    }


    uint16_t generateVoluntaryWaveChannel3(){
        std::unordered_map<int, int> audioConversion = {
            {0b00, 0}
            {0b01, 15 * 32767}
            {0b10, 7 * 32767}
            {0b11, 3 * 32767}
        };
        VoluntaryChannel3State.volume = memory[0xFF1C] >> 4;
        VoluntaryChannel3State.amplitude = (int16_t) audioConversion[VoluntaryChannel3State.Volume] * 32767; 
        //length timer, time until the channel shuts itself off.
        VoluntaryChannel3State.length = memory[0xFF1E] >> 5;
        //its write only so reading it is pointless
        VoluntaryChannel3State.trigger = memory[0xFF1E] >> 6;
        VoluntaryChannel3State.volume = memory[0xFF1C] >> 4;
        bool lengthEnable = ((memory[0xFF1E] >> 6) & 0b1);
        // --- Length logic for channel 3 (like channel 1) ---
        if (lengthEnable) {
            if (VoluntaryChannel3State.lengthAccumulator.trigger && VoluntaryChannel3State.lengthTimer > 0) {
                VoluntaryChannel3State.lengthTimer -= 1;
                if (VoluntaryChannel3State.lengthTimer == 0) {
                    VoluntaryChannel3State.on = false;
                }
            }
        }
        if (VoluntaryChannel3State.shadowTrigger){
           VoluntaryChannel3State.active = 0;
           VoluntaryChannel3State.LengthAccumulator.reset();
           VoluntaryChannel3State.Volume = audioConversion[((memory[0xFF1C] >> 4) & 0b11)];
           //TODO: reset RAM
            VoluntaryChannel3State.step = 0;
        } 
        //length enable handled during instructions.
        VoluntaryChannel3State.audioOutputFrequency = 2097152 / (2048 - period);
        //read one sample
        if (VoluntaryChannel3State.phase_accumlator.trigger){
            VoluntaryChannel3State.phase_accumlator.reset()
            //only 32 bit samples long.
            VoluntaryChannel3State.step = VoluntaryChannel3State.step % 32;   
        }
        uint8_t sampleByte = memory[0xFF30 + (VoluntaryChannel3State.step / 2)];
        //reverse nibble order.
        uint8_t sample4bit = (VoluntaryChannel3State.step % 2 == 0) ?
        (sampleByte >> 4) : (sampleByte & 0x0F);
        uint16_t sample16bit = sample4bit * 4369; 
        return sample16bit;
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
        want.userdata = &VoluntaryChanne13State;       // pass a pointer to your state
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
}

