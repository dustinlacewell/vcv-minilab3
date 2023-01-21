#pragma once

#include <experimental/memory>
#include <rack.hpp>

using namespace std::experimental;
using namespace rack;
using namespace rack::midi;

struct PadBinder {
    InputQueue* midiInput;

    PadBinder(InputQueue* midiInput) : midiInput(midiInput) {
        midiInput->reset();
    }

    int process(Module* module);
    void attachMidi(InputQueue& newQueue);
    int unbind();
    int bindToMiniLab3(Module* module);
    int bindToG8Pad(Module* module);
};