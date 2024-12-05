#include "MidiMessageRenderer.hpp"
#include <iomanip>
#include <rack.hpp>
#include "consts/midi.hpp"

using namespace rack;

std::string MidiMessageRenderer::padi(size_t n, int i) {
    std::string s = std::to_string(i);
    while (s.length() < n) {
        s = " " + s;
    }
    return s;
}

std::string MidiMessageRenderer::pads(size_t n, std::string s) {
    while (s.length() < n) {
        s = " " + s;
    }
    return s;
}

std::string MidiMessageRenderer::renderSysexMessage(midi::Message& msg) {
    std::string s = string::f("SYSEX (%i bytes)", msg.getSize());
    return s;
}

std::string MidiMessageRenderer::renderSongPointerMessage(midi::Message& msg) {
    uint16_t value = ((uint16_t)msg.getValue() << 7) | msg.getNote();
    std::string s = string::f("POINTER %i", value);
    return s;
}

std::string MidiMessageRenderer::renderSongSelectMessage(midi::Message& msg) {
    uint8_t song = msg.getNote();
    std::string s = string::f("SONG %i", song);
    return s;
}

std::string MidiMessageRenderer::renderTimingClockMessage(midi::Message& msg) {
    return "TICK";
}

std::string MidiMessageRenderer::renderStartMessage(midi::Message& msg) {
    return "START";
}

std::string MidiMessageRenderer::renderContinueMessage(midi::Message& msg) {
    return "CONTINUE";
}

std::string MidiMessageRenderer::renderStopMessage(midi::Message& msg) {
    return "STOP";
}

std::string MidiMessageRenderer::renderNoteOn(
    midi::Message& msg,
    std::string chan,
    std::string note
) {
    uint8_t vel = msg.getValue();
    std::string s = string::f(
        "%s %s %s | %i", chan.c_str(), pads(5, "ON").c_str(), note.c_str(), vel
    );
    return s;
}

std::string MidiMessageRenderer::renderNoteOff(
    midi::Message& msg,
    std::string chan,
    std::string note
) {
    uint8_t vel = msg.getValue();
    std::string s = string::f(
        "%s %s %s | %i", chan.c_str(), pads(5, "OFF").c_str(), note.c_str(), vel
    );
    return s;
}

std::string MidiMessageRenderer::renderKeyPressure(
    midi::Message& msg,
    std::string chan,
    std::string note
) {
    uint8_t value = msg.getValue();
    std::string s = string::f(
        "%s %s %s | %i",
        chan.c_str(),
        pads(5, "TOUCH").c_str(),
        note.c_str(),
        value
    );
    return s;
}

std::string MidiMessageRenderer::renderCC(
    midi::Message& msg,
    std::string chan,
    std::string note
) {
    int8_t value = msg.bytes[2];
    std::string s = string::f(
        "%s %s %s | %i",
        chan.c_str(),
        pads(5, "CC").c_str(),
        note.c_str(),
        value
    );
    return s;
}

std::string
MidiMessageRenderer::renderProgramChange(midi::Message& msg, std::string chan) {
    uint8_t prog = msg.getNote();
    std::string s =
        string::f("%s %s %i", chan.c_str(), pads(5, "PROG").c_str(), prog);
    return s;
}

std::string MidiMessageRenderer::renderChannelPressure(
    midi::Message& msg,
    std::string chan
) {
    uint8_t value = msg.getNote();
    std::string s = string::f("%s PRESS %i", chan.c_str(), value);
    return s;
}

std::string
MidiMessageRenderer::renderPitchWheel(midi::Message& msg, std::string chan) {
    uint8_t value = msg.getValue();
    std::string s = string::f("%s PITCH %i", chan.c_str(), value);
    return s;
}

std::string MidiMessageRenderer::renderSystemMessage(midi::Message& msg) {
    switch (msg.getChannel()) {
        case SysEx:  // sysex
            return renderSysexMessage(msg);
        case SongPositionPointer:  // song pointer
            return renderSongPointerMessage(msg);
        case SongSelect:  // song select
            return renderSongSelectMessage(msg);
        case TimingClock:  // timing clock
            return renderTimingClockMessage(msg);
        case Start:  // start
            return renderStartMessage(msg);
        case Continue:  // continue
            return renderContinueMessage(msg);
        case Stop:  // stop
            return renderStopMessage(msg);
        default:
            return "UNKOWN SYSTEM MSG";
    }
}

std::string MidiMessageRenderer::render(midi::Message& msg) {
    // render channel as two-digit number
    std::string chan = padi(2, msg.getChannel() + 1);
    // render note as a three-digit number, without leading zeros
    std::string note = padi(3, msg.getNote());

    switch (msg.getStatus()) {
        case NoteOn:
            return renderNoteOn(msg, chan, note);
        case NoteOff:
            return renderNoteOff(msg, chan, note);
        case KeyPressure:
            return renderKeyPressure(msg, chan, note);
        case ControlChange:
            return renderCC(msg, chan, note);
        case ProgramChange:
            return renderProgramChange(msg, chan);
        case ChannelPressure:
            return renderChannelPressure(msg, chan);
        case PitchBend:
            return renderPitchWheel(msg, chan);
        case System:
            return renderSystemMessage(msg);
        default:
            return "UNKOWN MSG";
    }
}
