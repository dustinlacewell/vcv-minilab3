#pragma once

#include <rack.hpp>


using namespace rack;


struct MidiMessageFilter {
    /** [Stored to JSON] */
    bool showNoteMsg = true;
    /** [Stored to JSON] */
    bool showKeyPressure = true;
    /** [Stored to JSON] */
    bool showCcMsg = true;
    /** [Stored to JSON] */
    bool showProgChangeMsg = true;
    /** [Stored to JSON] */
    bool showChannelPressurelMsg = true;
    /** [Stored to JSON] */
    bool showPitchWheelMsg = true;

    /** [Stored to JSON] */
    bool showSysExMsg = false;
    /** [Stored to JSON] */
    bool showSysExData = false;
    /** [Stored to JSON] */
    bool showClockMsg = false;
    /** [Stored to JSON] */
    bool showSystemMsg = true;

    void reset();

    bool shouldHandle(midi::Message msg);
    json_t *toJson();
    void fromJson(json_t *rootJ);
};