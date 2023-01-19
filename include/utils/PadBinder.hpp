#pragma once

#include <G8Pad.hpp>
#include <experimental/memory>
#include <rack.hpp>

using namespace std::experimental;
using namespace rack;
using namespace rack::midi;

struct PadBinder {
    InputQueue* midiInput;

    PadBinder(InputQueue* midiInput) : midiInput(midiInput) {}

    int process(Module* module);
    void attachMidi(InputQueue& midiInput);
    int unbind();
    int bindToMiniLab3(Module* module);
    int bindToG8Pad(Module* module);
};