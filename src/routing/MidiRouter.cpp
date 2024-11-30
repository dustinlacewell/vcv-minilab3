#include "MidiRouter.hpp"
#include "consts/midi.hpp"
#include "padNotes.hpp"

// No mutex needed - we assume setup happens in a single thread
// and runtime only reads the callbacks

MidiRouter::MidiRouter(int padId) {
    this->padId = padId;
    this->gateOpen = padId == -1;
    this->otherGates = 0;

    // Pre-allocate space for typical usage - reduces runtime allocations
    this->gateOpenCallbacks.reserve(4);
    this->gateCloseCallbacks.reserve(4);
    this->noteOnCallbacks.reserve(4);
    this->noteOffCallbacks.reserve(4);
    this->aftertouchCallbacks.reserve(4);
    this->pitchBendCallbacks.reserve(4);
    this->modWheelCallbacks.reserve(4);
    this->knobCallbacks.reserve(4);
    this->sliderCallbacks.reserve(4);
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

void MidiRouter::processNoteOn(midi::Message& msg) {
    int note = msg.getNote();
    int velocity = msg.getValue();
    int channel = msg.getChannel();

    if (channel == ControlChannel) {
        processGateNoteOn(msg);
        return;
    }

    if (padId == -1) {
        NoteOnEvent event{note, velocity};
        // Direct iteration - no locks or copies needed
        for (const auto& callback : noteOnCallbacks) {
            callback(event);
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
        NoteOffEvent event{note};
        for (const auto& callback : noteOffCallbacks) {
            callback(event);
        }
    }
}

void MidiRouter::processGateNoteOn(midi::Message& msg) {
    auto note = msg.getNote();

    if (padId == -1) {
        otherGates++;
        close();
        return;
    }

    if (note == noteForPad(padId)) {
        open();
        auto velocity = msg.getValue();
        NoteOnEvent event{note, velocity};
        for (const auto& callback : noteOnCallbacks) {
            callback(event);
        }
    }
}

void MidiRouter::processGateNoteOff(midi::Message& msg) {
    auto note = msg.getNote();

    if (padId == -1) {
        otherGates = std::max(0, otherGates - 1);
        if (otherGates == 0) {
            open();
        }
        return;
    }

    if (note == noteForPad(padId)) {
        close();
        NoteOffEvent event{note};
        for (const auto& callback : noteOffCallbacks) {
            callback(event);
        }
    }
}

void MidiRouter::processAftertouch(midi::Message& msg) {
    if (msg.getChannel() != ControlChannel) return;
    if (msg.getNote() != noteForPad(padId)) return;

    AftertouchEvent event{msg.getValue()};
    for (const auto& callback : aftertouchCallbacks) {
        callback(event);
    }
}

void MidiRouter::processPitchBend(midi::Message& msg) {
    if (!gateOpen) return;  // Direct access - no lock needed

    PitchBendEvent event{msg.getValue()};
    for (const auto& callback : pitchBendCallbacks) {
        callback(event);
    }
}

void MidiRouter::processModWheel(midi::Message& msg) {
    if (!gateOpen) return;

    ModWheelEvent event{msg.getValue()};
    for (const auto& callback : modWheelCallbacks) {
        callback(event);
    }
}

void MidiRouter::processKnob(midi::Message& msg) {
    if (!gateOpen) return;

    int knob = msg.getNote() - FirstKnob;
    KnobEvent event{knob, msg.getValue()};
    for (const auto& callback : knobCallbacks) {
        callback(event);
    }
}

void MidiRouter::processSlider(midi::Message& msg) {
    int slider = msg.getNote() - FirstSlider;
    SliderEvent event{slider, msg.getValue()};
    for (const auto& callback : sliderCallbacks) {
        callback(event);
    }
}

void MidiRouter::processControlChange(midi::Message& msg) {
    int note = msg.getNote();
    
    if (note == ModWheel) {
        processModWheel(msg);
    }
    else if (note >= FirstSlider && note <= FirstSlider + 3) {
        processSlider(msg);
    }
    else if (note >= FirstKnob && note <= FirstKnob + 7) {
        processKnob(msg);
    }
}

// Setup methods - these are called during initialization, not during real-time processing
void MidiRouter::onGateOpen(std::function<void()> callback) {
    if (callback) gateOpenCallbacks.push_back(std::move(callback));
}

void MidiRouter::onGateClose(std::function<void()> callback) {
    if (callback) gateCloseCallbacks.push_back(std::move(callback));
}

void MidiRouter::onNoteOn(std::function<void(NoteOnEvent)> callback) {
    if (callback) noteOnCallbacks.push_back(std::move(callback));
}

void MidiRouter::onNoteOff(std::function<void(NoteOffEvent)> callback) {
    if (callback) noteOffCallbacks.push_back(std::move(callback));
}

void MidiRouter::onAftertouch(std::function<void(AftertouchEvent)> callback) {
    if (callback) aftertouchCallbacks.push_back(std::move(callback));
}

void MidiRouter::onPitchBend(std::function<void(PitchBendEvent)> callback) {
    if (callback) pitchBendCallbacks.push_back(std::move(callback));
}

void MidiRouter::onModWheel(std::function<void(ModWheelEvent)> callback) {
    if (callback) modWheelCallbacks.push_back(std::move(callback));
}

void MidiRouter::onKnob(std::function<void(KnobEvent)> callback) {
    if (callback) knobCallbacks.push_back(std::move(callback));
}

void MidiRouter::onSlider(std::function<void(SliderEvent)> callback) {
    if (callback) sliderCallbacks.push_back(std::move(callback));
}

void MidiRouter::open() {
    if (!gateOpen) {
        gateOpen = true;
        for (const auto& callback : gateOpenCallbacks) {
            callback();
        }
    }
}

void MidiRouter::close() {
    if (gateOpen) {
        gateOpen = false;
        for (const auto& callback : gateCloseCallbacks) {
            callback();
        }
    }
}

json_t* MidiRouter::toJson() {
    json_t* rootJ = json_object();
    json_object_set_new(rootJ, "padId", json_integer(padId));
    json_object_set_new(rootJ, "gateOpen", json_boolean(gateOpen));
    return rootJ;
}

void MidiRouter::fromJson(json_t* rootJ) {
    if (!rootJ) return;

    json_t* padIdJ = json_object_get(rootJ, "padId");
    if (padIdJ) {
        padId = json_integer_value(padIdJ);
    }
    json_t* gateOpenJ = json_object_get(rootJ, "gateOpen");
    if (gateOpenJ) {
        gateOpen = json_boolean_value(gateOpenJ);
    }
}