#pragma once

enum MidiStatus {
    NoteOff = 0x8,
    NoteOn = 0x9,
    KeyPressure = 0xa,
    ControlChange = 0xb,
    ProgramChange = 0xc,
    ChannelPressure = 0xd,
    PitchBend = 0xe,
    System = 0xf,
};

enum SystemChannel {
    SysEx = 0x0,
    MTCQuarterFrame = 0x1,
    SongPositionPointer = 0x2,
    SongSelect = 0x3,
    TuneRequest = 0x6,
    TimingClock = 0x8,
    Start = 0xA,
    Continue = 0xB,
    Stop = 0xC,
    ActiveSensing = 0xE,
    SystemReset = 0xFF,
};

constexpr int ModWheel = 1;
constexpr int ControlChannel = 15;
constexpr int FirstSlider = 80;
constexpr int FirstKnob = 102;

inline bool isKnob(int note) {
    return note >= FirstKnob && note <= FirstKnob + 7;
}

inline int knobIndex(int note) {
    return note - FirstKnob;
}

inline bool isSlider(int note) {
    return note >= FirstSlider && note <= FirstSlider + 3;
}

inline int sliderIndex(int note) {
    return note - FirstSlider;
}