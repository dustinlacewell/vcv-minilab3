#pragma once

struct NoteOnEvent {
    int note;
    int velocity;
};

struct NoteOffEvent {
    int note;
};

struct AftertouchEvent {
    int aftertouch;
};

struct PitchBendEvent {
    int pitchBend;
};

struct ModWheelEvent {
    int modWheel;
};

struct KnobEvent {
    int knob;
    int value;
};

struct SliderEvent {
    int slider;
    int value;
};