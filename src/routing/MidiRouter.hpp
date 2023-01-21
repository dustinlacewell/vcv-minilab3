#include <rack.hpp>
#include "MidiEvents.hpp"

using namespace rack;

struct MidiRouter {
    int padId = -1;
    int otherGates = 0;
    bool gateOpen = false;

    std::vector<std::function<void()>> gateOpenCallbacks;
    std::vector<std::function<void()>> gateCloseCallbacks;
    std::vector<std::function<void(NoteOnEvent)>> noteOnCallbacks;
    std::vector<std::function<void(NoteOffEvent)>> noteOffCallbacks;
    std::vector<std::function<void(AftertouchEvent)>> aftertouchCallbacks;
    std::vector<std::function<void(PitchBendEvent)>> pitchBendCallbacks;
    std::vector<std::function<void(ModWheelEvent)>> modWheelCallbacks;
    std::vector<std::function<void(KnobEvent)>> knobCallbacks;
    std::vector<std::function<void(SliderEvent)>> sliderCallbacks;

    MidiRouter(int padId);
    ~MidiRouter();

    json_t* toJson();
    void fromJson(json_t* rootJ);
    void open();
    void close();
    void processMessage(midi::Message& msg);
    void processNoteOn(midi::Message& msg);
    void processGateNoteOn(midi::Message& msg);
    void processNoteOff(midi::Message& msg);
    void processGateNoteOff(midi::Message& msg);
    void processAftertouch(midi::Message& msg);
    void processPitchBend(midi::Message& msg);
    void processModWheel(midi::Message& msg);
    void processKnob(midi::Message& msg);
    void processSlider(midi::Message& msg);
    void processControlChange(midi::Message& msg);

    // callback register functions
    void onGateOpen(std::function<void()> callback);
    void onGateClose(std::function<void()> callback);
    void onNoteOn(std::function<void(NoteOnEvent)> callback);
    void onNoteOff(std::function<void(NoteOffEvent)> callback);
    void onAftertouch(std::function<void(AftertouchEvent)> callback);
    void onPitchBend(std::function<void(PitchBendEvent)> callback);
    void onModWheel(std::function<void(ModWheelEvent)> callback);
    void onKnob(std::function<void(KnobEvent)> callback);
    void onSlider(std::function<void(SliderEvent)> callback);
};