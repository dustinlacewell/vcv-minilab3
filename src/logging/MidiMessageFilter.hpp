#pragma once

#include <rack.hpp>

using namespace rack;

struct MidiMessageFilter {
    bool showNoteMsg = true;
    bool showKeyPressure = true;
    bool showCcMsg = true;
    bool showProgChangeMsg = true;
    bool showChannelPressurelMsg = true;
    bool showPitchWheelMsg = true;

    bool showSysExMsg = false;
    bool showSysExData = false;
    bool showClockMsg = false;
    bool showSystemMsg = true;

    void reset();

    bool shouldHandle(midi::Message msg);
    json_t* toJson();
    void fromJson(json_t* rootJ);
};