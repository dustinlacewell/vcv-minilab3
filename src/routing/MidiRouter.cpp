#include "MidiRouter.hpp"
#include "consts/midi.hpp"
#include "padNotes.hpp"

MidiRouter::MidiRouter(int padId) {
    this->padId = padId;
    this->gateOpen = padId == -1;
}

MidiRouter::~MidiRouter() {
    this->gateOpenCallbacks.clear();
    this->gateCloseCallbacks.clear();
    this->noteOnCallbacks.clear();
    this->noteOffCallbacks.clear();
    this->aftertouchCallbacks.clear();
    this->pitchBendCallbacks.clear();
    this->modWheelCallbacks.clear();
    this->knobCallbacks.clear();
    this->sliderCallbacks.clear();
}

json_t* MidiRouter::toJson() {
    json_t* rootJ = json_object();
    json_object_set_new(rootJ, "padId", json_integer(this->padId));
    json_object_set_new(rootJ, "gateOpen", json_boolean(this->gateOpen));
    return rootJ;
}

void MidiRouter::fromJson(json_t* rootJ) {
    json_t* padIdJ = json_object_get(rootJ, "padId");
    if (padIdJ) {
        this->padId = json_integer_value(padIdJ);
    }
    json_t* gateOpenJ = json_object_get(rootJ, "gateOpen");
    if (gateOpenJ) {
        this->gateOpen = json_boolean_value(gateOpenJ);
    }
}

void MidiRouter::onGateOpen(std::function<void()> callback) {
    this->gateOpenCallbacks.push_back(callback);
}

void MidiRouter::onGateClose(std::function<void()> callback) {
    this->gateCloseCallbacks.push_back(callback);
}

void MidiRouter::onNoteOn(std::function<void(NoteOnEvent)> callback) {
    this->noteOnCallbacks.push_back(callback);
}

void MidiRouter::onNoteOff(std::function<void(NoteOffEvent)> callback) {
    this->noteOffCallbacks.push_back(callback);
}

void MidiRouter::onAftertouch(std::function<void(AftertouchEvent)> callback) {
    this->aftertouchCallbacks.push_back(callback);
}

void MidiRouter::onPitchBend(std::function<void(PitchBendEvent)> callback) {
    this->pitchBendCallbacks.push_back(callback);
}

void MidiRouter::onModWheel(std::function<void(ModWheelEvent)> callback) {
    this->modWheelCallbacks.push_back(callback);
}

void MidiRouter::onKnob(std::function<void(KnobEvent)> callback) {
    this->knobCallbacks.push_back(callback);
}

void MidiRouter::onSlider(std::function<void(SliderEvent)> callback) {
    this->sliderCallbacks.push_back(callback);
}

void MidiRouter::open() {
    this->gateOpen = true;
    for (auto callback : this->gateOpenCallbacks) {
        callback();
    }
}

void MidiRouter::close() {
    this->gateOpen = false;
    for (auto callback : this->gateCloseCallbacks) {
        callback();
    }
}

void MidiRouter::processGateNoteOn(midi::Message& msg) {
    auto note = msg.getNote();

    if (padId == -1) {
        otherGates++;
        close();
        return;
    }

    if (note == noteForPad(this->padId)) {
        open();
        auto velocity = msg.getValue();
        NoteOnEvent noteEvent = {note, velocity};
        for (const auto& noteOnCallback : this->noteOnCallbacks) {
            noteOnCallback(noteEvent);
        }
    }
}

void MidiRouter::processGateNoteOff(midi::Message& msg) {
    auto note = msg.getNote();

    if (padId == -1) {
        otherGates--;
        if (otherGates == 0) {
            open();
        }
        return;
    }

    if (note == noteForPad(this->padId)) {
        close();
        NoteOffEvent noteEvent = {note};
        for (const auto& noteOffCallback : this->noteOffCallbacks) {
            noteOffCallback(noteEvent);
        }
    }
}

void MidiRouter::processNoteOn(midi::Message& msg) {
    int note = msg.getNote();
    int velocity = msg.getValue();
    int channel = msg.getChannel();

    if (channel == ControlChannel) {
        processGateNoteOn(msg);
        return;
    }

    if (padId == -1) {
        NoteOnEvent noteEvent{note, velocity};

        for (const auto& noteOnCallback : this->noteOnCallbacks) {
            noteOnCallback(noteEvent);
        }
    }
}

void MidiRouter::processNoteOff(midi::Message& msg) {
    int note = msg.getNote();
    int channel = msg.getChannel();

    if (channel == ControlChannel) {
        processGateNoteOff(msg);
        return;
    }

    if (padId == -1) {
        for (const auto& noteOffCallback : this->noteOffCallbacks) {
            NoteOffEvent noteEvent{};
            noteEvent.note = note;
            noteOffCallback(noteEvent);
        }
    }
}

void MidiRouter::processAftertouch(midi::Message& msg) {
    auto note = msg.getNote();
    auto channel = msg.getChannel();

    if (channel != ControlChannel) {  // only pads have aftertouch
        return;
    }

    if (note != noteForPad(this->padId)) {  // separate aftertouch for each pad
        return;
    }

    int aftertouch = msg.getValue();
    AftertouchEvent event{aftertouch};
    for (const auto& callback : this->aftertouchCallbacks) {
        callback(event);
    }
}

void MidiRouter::processPitchBend(midi::Message& msg) {
    if (!this->gateOpen) {
        return;
    }
    int pitchBend = msg.getValue();
    PitchBendEvent event{pitchBend};
    for (const auto& callback : this->pitchBendCallbacks) {
        callback(event);
    }
}

void MidiRouter::processModWheel(midi::Message& msg) {
    if (!this->gateOpen) {
        return;
    }
    int modWheel = msg.getValue();
    ModWheelEvent event{modWheel};
    for (const auto& callback : this->modWheelCallbacks) {
        callback(event);
    }
}

void MidiRouter::processKnob(midi::Message& msg) {
    if (!this->gateOpen) {
        return;
    }
    int knob = msg.getNote() - FirstKnob;
    int value = msg.getValue();
    KnobEvent event = {knob, value};
    for (const auto& callback : this->knobCallbacks) {
        callback(event);
    }
}

void MidiRouter::processSlider(midi::Message& msg) {
    int slider = msg.getNote() - FirstSlider;
    int value = msg.getValue();
    SliderEvent event = {slider, value};
    for (const auto& callback : this->sliderCallbacks) {
        callback(event);
    }
}

void MidiRouter::processControlChange(midi::Message& msg) {
    int note = msg.getNote();
    switch (note) {
        case ModWheel:
            processModWheel(msg);
            break;
        case FirstSlider:
        case FirstSlider + 1:
        case FirstSlider + 2:
        case FirstSlider + 3:
            processSlider(msg);
            break;
        case FirstKnob:
        case FirstKnob + 1:
        case FirstKnob + 2:
        case FirstKnob + 3:
        case FirstKnob + 4:
        case FirstKnob + 5:
        case FirstKnob + 6:
        case FirstKnob + 7:
            processKnob(msg);
            break;
    }
}

void MidiRouter::processMessage(midi::Message& msg) {
    switch (msg.getStatus()) {
        case NoteOn:
            processNoteOn(msg);
            break;
        case NoteOff:
            processNoteOff(msg);
            break;
        case KeyPressure:
            processAftertouch(msg);
            break;
        case PitchBend:
            processPitchBend(msg);
            break;
        case ControlChange:
            processControlChange(msg);
            break;
    }
}