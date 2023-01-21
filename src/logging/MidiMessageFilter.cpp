#include "MidiMessageFilter.hpp"
#include <rack.hpp>

using namespace rack;

void MidiMessageFilter::reset() {
    showNoteMsg = true;
    showKeyPressure = true;
    showCcMsg = true;
    showProgChangeMsg = true;
    showChannelPressurelMsg = true;
    showPitchWheelMsg = true;

    showSysExMsg = false;
    showSysExData = false;
    showClockMsg = false;
    showSystemMsg = true;
}

bool MidiMessageFilter::shouldHandle(midi::Message msg) {
    switch (msg.getStatus()) {
        case 0x8:  // Note Off
        case 0x9:  // Note On
            return true;
        case 0xa:  // Key Pressure
            return showKeyPressure;
        case 0xb:  // Control Change
            return showCcMsg;
        case 0xc:  // Program Change
            return showProgChangeMsg;
        case 0xd:  // Channel Pressure
            return showChannelPressurelMsg;
        case 0xe:  // Pitch Wheel
            return showPitchWheelMsg;
        case 0xf:  // System
            switch (msg.getChannel()) {
                case 0xF0:  // SysEx
                    return showSysExMsg;
                case 0xF1:  // MTC Quarter Frame
                case 0xF2:  // Song Position Pointer
                case 0xF3:  // Song Select
                case 0xF6:  // Tune Request
                    return showSystemMsg;
                case 0xF8:  // Timing Clock
                    return showClockMsg;
                case 0xFA:  // Start
                case 0xFB:  // Continue
                case 0xFC:  // Stop
                case 0xFE:  // Active Sensing
                case 0xFF:  // System Reset
                    return showSystemMsg;
            }
            break;
    }
    return false;
}

json_t* MidiMessageFilter::toJson() {
    json_t* rootJ = json_object();
    json_object_set_new(rootJ, "showNoteMsg", json_boolean(showNoteMsg));
    json_object_set_new(
        rootJ, "showKeyPressure", json_boolean(showKeyPressure)
    );
    json_object_set_new(rootJ, "showCcMsg", json_boolean(showCcMsg));
    json_object_set_new(
        rootJ, "showProgChangeMsg", json_boolean(showProgChangeMsg)
    );
    json_object_set_new(
        rootJ, "showChannelPressurelMsg", json_boolean(showChannelPressurelMsg)
    );
    json_object_set_new(
        rootJ, "showPitchWheelMsg", json_boolean(showPitchWheelMsg)
    );
    json_object_set_new(rootJ, "showSysExMsg", json_boolean(showSysExMsg));
    json_object_set_new(rootJ, "showSysExData", json_boolean(showSysExData));
    json_object_set_new(rootJ, "showClockMsg", json_boolean(showClockMsg));
    json_object_set_new(rootJ, "showSystemMsg", json_boolean(showSystemMsg));
    return rootJ;
}

void MidiMessageFilter::fromJson(json_t* rootJ) {
    json_t* showNoteMsgJ = json_object_get(rootJ, "showNoteMsg");
    if (showNoteMsgJ) {
        showNoteMsg = json_is_true(showNoteMsgJ);
    }

    json_t* showKeyPressureJ = json_object_get(rootJ, "showKeyPressure");
    if (showKeyPressureJ) {
        showKeyPressure = json_is_true(showKeyPressureJ);
    }

    json_t* showCcMsgJ = json_object_get(rootJ, "showCcMsg");
    if (showCcMsgJ) {
        showCcMsg = json_is_true(showCcMsgJ);
    }

    json_t* showProgChangeMsgJ = json_object_get(rootJ, "showProgChangeMsg");
    if (showProgChangeMsgJ) {
        showProgChangeMsg = json_is_true(showProgChangeMsgJ);
    }

    json_t* showChannelPressurelMsgJ =
        json_object_get(rootJ, "showChannelPressurelMsg");
    if (showChannelPressurelMsgJ) {
        showChannelPressurelMsg = json_is_true(showChannelPressurelMsgJ);
    }

    json_t* showPitchWheelMsgJ = json_object_get(rootJ, "showPitchWheelMsg");
    if (showPitchWheelMsgJ) {
        showPitchWheelMsg = json_is_true(showPitchWheelMsgJ);
    }

    json_t* showSysExMsgJ = json_object_get(rootJ, "showSysExMsg");
    if (showSysExMsgJ) {
        showSysExMsg = json_is_true(showSysExMsgJ);
    }

    json_t* showSysExDataJ = json_object_get(rootJ, "showSysExData");
    if (showSysExDataJ) {
        showSysExData = json_is_true(showSysExDataJ);
    }

    json_t* showClockMsgJ = json_object_get(rootJ, "showClockMsg");
    if (showClockMsgJ) {
        showClockMsg = json_is_true(showClockMsgJ);
    }

    json_t* showSystemMsgJ = json_object_get(rootJ, "showSystemMsg");
    if (showSystemMsgJ) {
        showSystemMsg = json_is_true(showSystemMsgJ);
    }
}