#include <singleton.h>
#include <SDL2.h>
#include "accumulator.h"


class audio {

    struct PulseChannel1State {
        bool channelEnabled;
        int step = 0;
        uint8_t dutyIndex;
        int lengthTimer = 64;
        bool lengthEnable;
        Accumulator& phaseAccumulator = Clock.Accumulators['a'];
        Accumulator& sweepAccumulator = Clock.Accumulators['b'];
        Accumulator& envelopeSweepAccumulator = Clock.Accumulators['c'];
        Accumulator& lengthAccumulator = Clock.Accumulators['d'];
        uint16_t& shadowFrequency = Singleton.getChannel1ShadowFrequency();
        uint8_t sweepShift;
        uint8_t sweepDirection;
        uint8_t volume = (memory[0xFF12] >> 4) & 0xF;
        uint8_t amplitude;
        uint8_t envelope;
        uint8_t envelopePeriod;
        uint32_t envelopeAccumulator;
        uint8_t& shadowTrigger = Singleton.getInstance().getShadowTriggerChannel1();
    };

    struct PulseChannel2State {
        bool channelEnabled;
        int step = 0;
        uint8_t dutyIndex;
        uint8_t lengthTimer = Singleton.getInstance().getChannel2LengthTimer();
        Accumulator& phaseAccumulator = Clock.Accumulators['g'];
        Accumulator& lengthAccumulator = Clock.Accumulators['f'];
        Accumulator& lengthAccumulator = Clock.Accumulators['h'];
        uint16_t& shadowFrequency = Singleton.getInstance().getChannel2ShadowFrequency();
        uint8_t volume = (memory[0xFF17] >> 4) & 0xF;
        uint8_t amplitude;
        uint8_t envelope;
        uint8_t envelopePeriod;
        uint32_t envelopeAccumulator;
        uint8_t& shadowTrigger = Singleton.getInstance().getShadowTriggerChannel2();
    };

    struct VoluntaryChannel3State {
        bool active = memory[0xFFA1] >> 6 & 0b1;
        int step = 0;
        uint8_t& lengthTimer = Singleton.getInstance().getChannel3LengthTimer();
        uint8_t length;
        Accumulator& phaseAccumulator = Clock.Accumulators['j'];
        Accumulator& lengthAccumulator = Clock.Accumulators['k'];
        uint16_t& shadowFrequency = Singleton.getInstance().getChannel3ShadowFrequency();
        int volume = (memory[0xFF1C] >> 4) & 0b11;
        uint8_t* amplitude;
        uint8_t& shadowTrigger = Singleton.getInstance().getShadowTriggerChannel3();
    };

    struct NoiseChannel4State {
        int active;
        bool lengthEnable = memory[0xFF23] >> 7 & 0b1;
        bool lengthEnable;
        uint16_t lsfr;
        int step = 0;
        uint8_t& lengthTimer = Singleton.getInstance().getChannel4LengthTimer();
        Accumulator& randomAccumulator = Clock.Accumulators['l'];
        Accumulator& lengthAccumulator = Clock.Accumulators['m'];
        Accumulator& envelopeAccumulator = Clock.Accumulators['n'];
        uint16_t& shadowFrequency = Singleton.getInstance().getChannel4ShadowFrequency();
        double clockDivider = memory[0xFF22] & 0b111;
        uint8_t clockShift = (memory[0xFF22] >> 4) & 0b1111;
        uint8_t lsfrWidth = (memory[0xFF22] >> 3) & 0b1;
        uint8_t trigger = Singleton.getInstance().getChannel4ShadowTrigger();
        uint8_t volume; 
        int16_t amplitude;        
        if (clockDivider == 0){
            clockDivider = 0.5;
        }
    };

    uint8_t* memory = Singleton.getInstance.getMemoryBus();
    static const int AUDIO_BUFFER_SIZE = 1024;
    int16_t bufferChannel1[AUDIO_BUFFER_SIZE] = {0};
    int16_t bufferChannel2[AUDIO_BUFFER_SIZE] = {0};
    int16_t bufferChannel3[AUDIO_BUFFER_SIZE] = {0};
    int16_t bufferChannel4[AUDIO_BUFFER_SIZE] = {0};
    extern PulseChanne1lState;
    extern PulseChannel2State;
    extern VoluntaryChannel3State;
    extern NoiseChannel4State; 
    int bufferIndexChannel1 = 0;
    int bufferIndexChannel2 = 0;
    int bufferIndexChannel3 = 0;
    int bufferIndexChannel4 = 0;
    int phaseStep = 0;
    Clock& clock = Singleton.getInstance.getClock();
    const std::unordered_map<uint8_t, std::array<uint8_t, 8>> dutyTable = {
        { 0b00, { 0,0,0,0,0,0,0,1 } },
        { 0b01, { 0,0,0,0,0,0,1,1 } },
        { 0b10, { 0,0,0,0,1,1,1,1 } },
        { 0b11, { 1,1,1,1,1,1,0,0 } }
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
    uint8_t* WavePatternRAM = &memory[0xFF30];

    int16_t generatePulseWaveChannel1() {
        uint8_t duty = (memory[0xFF11] >> 6) & 0b11;
        bool lengthTrigger = (memory[0xFF14] >> 6) & 0b1;
        bool lengthEnable = (memory[0xFF14] >> 5) & 0b1;
        if (PulseChannel1State.shadowTrigger) {
            PulseChannel1State.active = 1;
            PulseChannel1State.lengthTimer = 1;
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
            if (newFreq > 2047) {
                PulseChannel1State.active = 0;
            } else {
                PulseChannel1State.shadowFrequency = newFreq;
            }
            PulseChannel1State.phaseAccumulator.threshold = (2048 - newfreq) * 32;
            PulseChannel1State.volume = (memory[0xFF12] >> 4) & 0xF;
            PulseChannel1State.envelopeSweepAccumulator.reset();
            PulseChannel1State.shadowFrequency = getPeriodChannel1();
            PulseChannel1State.shadowTrigger = 0;
        }
        if (lengthEnable) {
            if (PulseChannel1State.lengthAccumulator.trigger && PulseChannel1State.lengthTimer > 0) {
                PulseChannel1State.lengthTimer -= 1;
                if (PulseChannel1State.lengthTimer == 0) {
                    PulseChannel1State.active = 0;
                }
            }
        }
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
            if (newFreq > 2047) {
                PulseChannel1State.active = 0;
            } else {
                PulseChannel1State.shadowFrequency = newFreq;
            }
            PulseChannel1State.sweepAccumulator.threshold = sweepPeriod * 32768;
            PulseChannel1State.sweepAccumulator.trigger = false;
        }

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
            PulseChannel1State.envelopeSweepAccumulator.threshold = envelopePeriod * 32768;
            PulseChannel1State.envelopeSweepAccumulator.trigger = false;
        } else {
            PulseChannel1State.amplitude = (int16_t)(PulseChannel1State.volume * 32767);
        }

        uint16_t frequency = getPeriodChannel1();
        if (PulseChannel1State.phaseAccumulator.trigger) {
            PulseChannel1State.dutyIndex = PulseChannel1State.step % 8;
            PulseChannel1State.step = (PulseChannel1State.step + 1) % 8;
            Pulse1Channel1State.phaseAccumulator.reset();
            Pulse1Channel1State.phaseAccumulator.threshold = (2048 - *channel1ShadowFrequency) * 4;
        }
        bool on = dutyTable.at(duty)[PulseChannel1State.dutyIndex];
        int16_t sample = (PulseChannel1State.active && on) ? PulseChannel1State.amplitude : 0;
        return sample;
    }

    uint16_t generatePulseWaveChannel2() {
        uint8_t duty = memory[0xFF16];
        int dutytableindex = (duty >> 6) & 0b11;
        uint8_t envelope = memory[0xFF17];
        uint16_t frequency = getPeriodChannel2();

        if (PulseChannel2State.shadowTrigger) {
            PulseChannel2State.active = 1;
            PulseChannel2State.lengthTimer = 1;
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
        if (lengthEnable) {
            if (PulseChannel2State.lengthAccumulator.trigger && PulseChannel2State.lengthTimer > 0) {
                PulseChannel2State.lengthTimer -= 1;
                if (PulseChannel2State.lengthTimer == 0) {
                    PulseChannel2State.active = 0;
                }
            }
        }
        uint8_t initialVolume = (envelope >> 4) & 0xF;
        bool envelopeDirection = (envelope >> 3) & 0x1;
        uint8_t envelopePeriod = envelope & 0x7;
        if (PulseChannel2State.step == 0) {
            PulseChannel2State.volume = initialVolume;
        }

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
            PulseChannel2State.envelopeSweepAccumulator.threshold = envelopePeriod * 65536;
            PulseChannel2State.envelopeSweepAccumulator.reset();
        } else {
            PulseChannel2State.amplitude = (int16_t)(PulseChannel2State.volume * 32767);
        }

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
        VoluntaryChannel3State.length = memory[0xFF1E] >> 5;
        VoluntaryChannel3State.trigger = memory[0xFF1E] >> 6;
        VoluntaryChannel3State.volume = memory[0xFF1C] >> 4;
        bool lengthEnable = ((memory[0xFF1E] >> 6) & 0b1);
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
           VoluntaryChannel3State.step = 0;
        } 
        VoluntaryChannel3State.audioOutputFrequency = 2097152 / (2048 - period);
        if (VoluntaryChannel3State.phase_accumlator.trigger){
            VoluntaryChannel3State.phase_accumlator.reset()
            VoluntaryChannel3State.step = VoluntaryChannel3State.step % 32;   
        }
        uint8_t sampleByte = memory[0xFF30 + (VoluntaryChannel3State.step / 2)];
        uint8_t sample4bit = (VoluntaryChannel3State.step % 2 == 0) ?
        (sampleByte >> 4) : (sampleByte & 0x0F);
        uint16_t sample16bit = sample4bit * 4369; 
        return sample16bit;
    }

    uint16_t generateNoiseWaveChannel4(){
        if (NoiseChannel4State.trigger) {
            NoiseChannel4State.active = 1;
            NoiseChannel4State.lengthTimer = memory[0xFF20] & 0b111111;
            NoiseChannel4State.volume = (memory[0xFF21] >> 4) & 0xF;
            NoiseChannel4State.envelopeAccumulator.reset();
            NoiseChannel4State.randomAccumulator.reset();
            auto divider = memory[0xFF22] & 0b111;
            if (divider == 0) divider = 0.5;
            auto shift = (memory[0xFF22] >> 4) & 0b1111;
            auto freq = 262144 / (divider << (2 * shift));
            auto freqScaledtoCpu = (4194304.0 / freq);
            NoiseChannel4State.randomAccumulator.threshold = freqScaledtoCpu;
            NoiseChannel4State.trigger = 0;
            NoiseChannel4State.step = (NoiseChannel4State.step + 1) % 32768;
        }
        if (NoiseChannel4State.lengthEnable) {
            if (NoiseChannel4State.lengthAccumulator.trigger && NoiseChannel4State.lengthTimer > 0) {
                NoiseChannel4State.lengthTimer -= 1;
                if (NoiseChannel4State.lengthTimer == 0) {
                    NoiseChannel4State.active = 0;
                }
                NoiseChannel4State.lengthAccumulator.reset();
            }
        }
        uint8_t envelope = memory[0xFF21];
        uint8_t initialVolume = (envelope >> 4) & 0xF;
        bool envelopeDirection = (envelope >> 3) & 0x1;
        uint8_t envelopePeriod = envelope & 0x7;
        if (NoiseChannel4State.envelopeAccumulator.trigger) {
            if (envelopePeriod > 0) {
                if (envelopeDirection) {
                    if (NoiseChannel4State.volume < 15) {
                        NoiseChannel4State.volume += 1;
                    }
                } else {
                    if (NoiseChannel4State.volume > 0) {
                        NoiseChannel4State.volume -= 1;
                        if (NoiseChannel4State.volume == 0) {
                            NoiseChannel4State.active = 0;
                        }
                    }
                }
            }
            NoiseChannel4State.amplitude = (int16_t)(NoiseChannel4State.volume * 2184); 
            NoiseChannel4State.envelopeAccumulator.threshold = envelopePeriod * 65536;
            NoiseChannel4State.envelopeAccumulator.reset();
        } else {
            NoiseChannel4State.amplitude = (int16_t)(NoiseChannel4State.volume * 2184);
        }
        if (NoiseChannel4State.randomAccumulator.trigger){
            auto divider = memory[0xFF22] & 0b111;
            auto shift = (memory[0xFF22] >> 4) & 0b1111;
            auto freq = 262144 / (divider << shift); 
            auto freqScaledtoCpu = (4194304.0 / freq);
            NoiseChannel4State.randomAccumulator.reset();
            NoiseChannel4State.randomAccumulator.threshold = freqScaledtoCpu;
            auto prevNoise = NoiseChannel4State.lsfr;
            auto xorRes = (prevNoise >> 1 & 0b1) ^ (prevNoise & 0b1); 
            if (memory[0xFF22] >> 3 & 0b1){
                NoiseChannel4State.lsfr = (prevNoise & ~(1 << 6) | (xorRes << 6));
            } else {
                NoiseChannel4State.lsfr = (prevNoise & ~(1 << 14) | (xorRes << 14));
            }
        }
        int16_t sample = NoiseChannel4State.active ? NoiseChannel4State.amplitude : 0;
        return sample;
    }

    uint16_t handleAudioMixing(){
        uint8_t bufferlen = 4096;
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
        want.freq = 44100;
        want.format = AUDIO_S16SYS;
        want.channels = 1;
        want.samples = 1024;
        want.callback = audio_callback;
        want.userdata = &PulseChannel1State;
        SDL_AudioDeviceID channel1dev = SDL_OpenAudioDevice(
            nullptr,
            0,
            &want,
            &have,
            0
        );
        if (dev == 0) {
            SDL_Log("Failed to open audio: %s", SDL_GetError());
            return;
        }
    }

    void openChannel2AudioDevice(){
        SDL_AudioSpec want, have;
        SDL_zero(want);
        want.freq = 44100;
        want.format = AUDIO_S16SYS;
        want.channels = 1;
        want.samples = 1024;
        want.callback = audio_callback;
        want.userdata = &PulseChannel2State;
        SDL_AudioDeviceID devchannel2dev = SDL_OpenAudioDevice(
            nullptr,
            0,
            &want,
            &have,
            0
        );
        if (dev == 0) {
            SDL_Log("Failed to open audio: %s", SDL_GetError());
            return;
        }
    }

    void openChannel3AudioDevice(){
        SDL_AudioSpec want, have;
        SDL_zero(want);
        want.freq = 44100;
        want.format = AUDIO_S16SYS;
        want.channels = 1;
        want.samples = 1024;
        want.callback = audio_callback;
        want.userdata = &VoluntaryChanne13State;
        SDL_AudioDeviceID devchannel3dev = SDL_OpenAudioDevice(
            nullptr,
            0,
            &want,
            &have,
            0
        );
        if (dev == 0) {
            SDL_Log("Failed to open audio: %s", SDL_GetError());
            return;
        }
    }

    void openChannel4AudioDevice(){
        SDL_AudioSpec want, have;
        SDL_zero(want);
        want.freq = 44100;
        want.format = AUDIO_S16SYS;
        want.channels = 1;
        want.samples = 1024;
        want.callback = audio_callback;
        want.userdata = &PulseChannel4State;
        SDL_AudioDeviceID devchannel4dev = SDL_OpenAudioDevice(
            nullptr,
            0,
            &want,
            &have,
            0
        );
        if (dev == 0) {
            SDL_Log("Failed to open audio: %s", SDL_GetError());
            return;
        }
    }

    void openChannel5AudioDevice(){
        SDL_AudioSpec want, have;
        SDL_zero(want);
        want.freq = 44100;
        want.format = AUDIO_S16SYS;
        want.channels = 1;
        want.samples = 1024;
        want.callback = audio_callback;
        want.userdata = &PulseChannel1State;
        SDL_AudioDeviceID devchannel5dev = SDL_OpenAudioDevice(
            nullptr,
            0,
            &want,
            &have,
            0
        );
        if (dev == 0) {
            SDL_Log("Failed to open audio: %s", SDL_GetError());
            return;
        }
    }
}

