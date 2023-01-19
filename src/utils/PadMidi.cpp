#include <utils/PadMidi.hpp>


int noteForPad(int padId) {
    switch (padId) {
        // C1
        case 1:return 36;
            // C2
        case 2:return 48;
            // C3
        case 3:return 60;
            // C4
        case 4:return 72;
            // C5
        case 5:return 84;
            // C6
        case 6:return 96;
            // C7
        case 7:return 108;
            // C8
        case 8:return 120;
            // D1
        case 9:return 38;
            // D2
        case 10:return 50;
            // D3
        case 11:return 62;
            // D4
        case 12:return 74;
            // D5
        case 13:return 86;
            // D6
        case 14:return 98;
            // D7
        case 15:return 110;
            // D8
        case 16:return 122;
        default:return -1;
    }
}

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

json_t *PadMidi::toJson() {
    json_t *rootJ = json_object();
    json_object_set_new(rootJ, "padId", json_integer(this->padId));
    json_object_set_new(rootJ, "gateOpen", json_boolean(this->gateOpen));
    return rootJ;
}

void PadMidi::fromJson(json_t *rootJ) {
    json_t *padIdJ = json_object_get(rootJ, "padId");
    if (padIdJ) {
        this->padId = json_integer_value(padIdJ);
    }
    json_t *gateOpenJ = json_object_get(rootJ, "gateOpen");
    if (gateOpenJ) {
        this->gateOpen = json_boolean_value(gateOpenJ);
    }
}

void PadMidi::onGateOpen(std::function<void(GateOpenEvent)> callback) {
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

void PadMidi::processNoteOn(midi::Message &msg) {
    INFO("PadMidi::processNoteOn %d : %d : %d", msg.getStatus(), msg.getNote(), msg.getValue());
    int note = msg.getNote();
    int velocity = msg.getValue();
    if (note == noteForPad(this->padId)) {
        this->gateOpen = true;
        GateOpenEvent event;
        event.velocity = velocity;
        for (auto callback : this->gateOpenCallbacks) {
            callback(event);
        }
    }
}

void PadMidi::processNoteOff(midi::Message &msg) {
    INFO("PadMidi::processNoteOff %d : %d : %d", msg.getStatus(), msg.getNote(), msg.getValue());
    int note = msg.getNote();
    if (note == noteForPad(this->padId)) {
        this->gateOpen = false;
        for (auto callback : this->gateCloseCallbacks) {
            callback();
        }
    }
}

void PadMidi::processAftertouch(midi::Message &msg) {
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

void PadMidi::processPitchBend(midi::Message &msg) {
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

void PadMidi::processModWheel(midi::Message &msg) {
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

void PadMidi::processKnob(midi::Message &msg) {
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

void PadMidi::processControlChange(midi::Message &msg) {
    int note = msg.getNote();
    switch (note) {
        case 0x01:processModWheel(msg);
            break;
        case 102:
        case 103:
        case 104:
        case 105:
        case 106:
        case 107:
        case 108:
        case 109:processKnob(msg);
            break;
    }
}

void PadMidi::processMessage(midi::Message &msg) {
    INFO("PadMidi::processMessage %d : %d : %d", msg.getStatus(), msg.getNote(), msg.getValue());

    switch (msg.getStatus()) {
        case 0x9:processNoteOn(msg);
            break;
        case 0x8:processNoteOff(msg);
            break;
        case 0xa:processAftertouch(msg);
            break;
        case 0xe:processPitchBend(msg);
            break;
        case 0xb:processControlChange(msg);
            break;
    }
}