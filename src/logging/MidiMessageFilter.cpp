#include "MidiMessageFilter.hpp"
#include <rack.hpp>

#include "consts/midi.hpp"

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
        case NoteOff:
        case NoteOn:
            return true;
        case KeyPressure:
            return showKeyPressure;
        case ControlChange:
            return showCcMsg;
        case ProgramChange:
            return showProgChangeMsg;
        case ChannelPressure:
            return showChannelPressurelMsg;
        case PitchBend:
            return showPitchWheelMsg;
        case System:
            switch (msg.getChannel()) {
                case SysEx:
                    return showSysExMsg;
                case MTCQuarterFrame:
                case SongPositionPointer:
                case SongSelect:
                case TuneRequest:
                    return showSystemMsg;
                case TimingClock:
                    return showClockMsg;
                case Start:
                case Continue:
                case Stop:
                case ActiveSensing:
                case SystemReset:
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