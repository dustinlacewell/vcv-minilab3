#include <rack.hpp>
#include "utils/MidiEvents.hpp"

using namespace rack;

struct PadMidi {
    int padId = -1;
    bool gateOpen = false;

    // callbacks for gate open
    std::vector<std::function<void()>> gateOpenCallbacks;

    // callbacks for gate close
    std::vector<std::function<void()>> gateCloseCallbacks;

    // callbacks for aftertouch
    std::vector<std::function<void(AftertouchEvent)>> aftertouchCallbacks;

    // callbacks for pitch bend
    std::vector<std::function<void(PitchBendEvent)>> pitchBendCallbacks;

    // callbacks for mod wheel
    std::vector<std::function<void(ModWheelEvent)>> modWheelCallbacks;

    // callbacks for knobs
    std::vector<std::function<void(KnobEvent)>> knobCallbacks;

    PadMidi(int padId);
    ~PadMidi();

    json_t* toJson();
    void fromJson(json_t* rootJ);

    void processMessage(midi::Message& msg);
    void processNoteOn(midi::Message& msg);
    void processNoteOff(midi::Message& msg);
    void processAftertouch(midi::Message& msg);
    void processPitchBend(midi::Message& msg);
    void processModWheel(midi::Message& msg);
    void processKnob(midi::Message& msg);
    void processControlChange(midi::Message& msg);

    // callback register functions
    void onGateOpen(std::function<void()> callback);
    void onGateClose(std::function<void()> callback);
    void onAftertouch(std::function<void(AftertouchEvent)> callback);
    void onPitchBend(std::function<void(PitchBendEvent)> callback);
    void onModWheel(std::function<void(ModWheelEvent)> callback);
    void onKnob(std::function<void(KnobEvent)> callback);
};