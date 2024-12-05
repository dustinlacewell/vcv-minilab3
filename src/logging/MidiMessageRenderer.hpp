#pragma once

#include <iomanip>
#include <rack.hpp>

using namespace rack;

struct MidiMessageRenderer {
    std::string padi(size_t n, int i);
    std::string pads(size_t n, std::string s);
    std::string renderSysexMessage(midi::Message &msg);
    std::string renderSongPointerMessage(midi::Message &msg);
    std::string renderSongSelectMessage(midi::Message &msg);
    std::string renderTimingClockMessage(midi::Message &msg);
    std::string renderStartMessage(midi::Message &msg);
    std::string renderContinueMessage(midi::Message &msg);
    std::string renderStopMessage(midi::Message &msg);
    std::string renderNoteOn(midi::Message &msg, std::string chan, std::string note);
    std::string renderNoteOff(midi::Message &msg, std::string chan, std::string note);
    std::string renderKeyPressure(midi::Message &msg, std::string chan, std::string note);
    std::string renderCC(midi::Message &msg, std::string chan, std::string note);
    std::string renderProgramChange(midi::Message &msg, std::string chan);
    std::string renderChannelPressure(midi::Message &msg, std::string chan);
    std::string renderPitchWheel(midi::Message &msg, std::string chan);
    std::string renderSystemMessage(midi::Message &msg);
    std::string render(midi::Message &msg);
};