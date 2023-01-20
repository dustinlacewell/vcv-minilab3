#include "PadMidi.hpp"
#include "utils/padNotes.hpp"

PadMidi::PadMidi(int padId) {
    this->padId = padId;
    this->gateOpen = false;
}

PadMidi::~PadMidi() {
    this->gateOpenCallbacks.clear();
    this->gateCloseCallbacks.clear();
    this->aftertouchCallbacks.clear();
    this->pitchBendCallbacks.clear();
    this->modWheelCallbacks.clear();
    this->knobCallbacks.clear();
}

json_t* PadMidi::toJson() {
    json_t* rootJ = json_object();
    json_object_set_new(rootJ, "padId", json_integer(this->padId));
    json_object_set_new(rootJ, "gateOpen", json_boolean(this->gateOpen));
    return rootJ;
}

void PadMidi::fromJson(json_t* rootJ) {
    json_t* padIdJ = json_object_get(rootJ, "padId");
    if (padIdJ) {
        this->padId = json_integer_value(padIdJ);
    }
    json_t* gateOpenJ = json_object_get(rootJ, "gateOpen");
    if (gateOpenJ) {
        this->gateOpen = json_boolean_value(gateOpenJ);
    }
}

void PadMidi::onGateOpen(std::function<void()> callback) {
    this->gateOpenCallbacks.push_back(callback);
}

void PadMidi::onGateClose(std::function<void()> callback) {
    this->gateCloseCallbacks.push_back(callback);
}

void PadMidi::onAftertouch(std::function<void(AftertouchEvent)> callback) {
    this->aftertouchCallbacks.push_back(callback);
}

void PadMidi::onPitchBend(std::function<void(PitchBendEvent)> callback) {
    this->pitchBendCallbacks.push_back(callback);
}

void PadMidi::onModWheel(std::function<void(ModWheelEvent)> callback) {
    this->modWheelCallbacks.push_back(callback);
}

void PadMidi::onKnob(std::function<void(KnobEvent)> callback) {
    this->knobCallbacks.push_back(callback);
}

void PadMidi::processNoteOn(midi::Message& msg) {
    int note = msg.getNote();
    int velocity = msg.getValue();
    if (note == noteForPad(this->padId)) {
        this->gateOpen = true;
        for (auto callback : this->gateOpenCallbacks) {
            callback();
        }
    }
}

void PadMidi::processNoteOff(midi::Message& msg) {
    int note = msg.getNote();
    if (note == noteForPad(this->padId)) {
        this->gateOpen = false;
        for (auto callback : this->gateCloseCallbacks) {
            callback();
        }
    }
}

void PadMidi::processAftertouch(midi::Message& msg) {
    if (!this->gateOpen) {
        return;
    }
    int aftertouch = msg.getValue();
    AftertouchEvent event;
    event.aftertouch = aftertouch;
    for (auto callback : this->aftertouchCallbacks) {
        callback(event);
    }
}

void PadMidi::processPitchBend(midi::Message& msg) {
    if (!this->gateOpen) {
        return;
    }
    int pitchBend = msg.getValue();
    PitchBendEvent event;
    event.pitchBend = pitchBend;
    for (auto callback : this->pitchBendCallbacks) {
        callback(event);
    }
}

void PadMidi::processModWheel(midi::Message& msg) {
    if (!this->gateOpen) {
        return;
    }
    int modWheel = msg.getValue();
    ModWheelEvent event;
    event.modWheel = modWheel;
    for (auto callback : this->modWheelCallbacks) {
        callback(event);
    }
}

void PadMidi::processKnob(midi::Message& msg) {
    if (!this->gateOpen) {
        return;
    }
    int knob = msg.getNote() - 102;
    int value = msg.getValue();
    KnobEvent event = {knob, value};
    for (auto callback : this->knobCallbacks) {
        callback(event);
    }
}

void PadMidi::processControlChange(midi::Message& msg) {
    int note = msg.getNote();
    switch (note) {
        case 0x01:
            processModWheel(msg);
            break;
        case 102:
        case 103:
        case 104:
        case 105:
        case 106:
        case 107:
        case 108:
        case 109:
            processKnob(msg);
            break;
    }
}

void PadMidi::processMessage(midi::Message& msg) {
    switch (msg.getStatus()) {
        case 0x9:
            processNoteOn(msg);
            break;
        case 0x8:
            processNoteOff(msg);
            break;
        case 0xa:
            processAftertouch(msg);
            break;
        case 0xe:
            processPitchBend(msg);
            break;
        case 0xb:
            processControlChange(msg);
            break;
    }
}