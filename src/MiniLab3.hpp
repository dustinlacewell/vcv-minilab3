#pragma once

#include <rack.hpp>
#include "BaseModule.hpp"
#include "params/AbsoluteParam.hpp"
#include "params/RelativeParam.hpp"
#include "props/SlewLimitQuantity.hpp"
#include "routing/MidiRouter.hpp"
#include "utils/VoltageMode.hpp"
#include "widgets/BaseWidget.hpp"

// use the rack::dsp namespace for convenience
using namespace rack::dsp;

struct MiniLab3 : BaseModule {
    enum ParamId { PARAMS_LEN };
    enum InputId { INPUTS_LEN };
    enum OutputId {
        NOTE_OUTPUT,
        GATE_OUTPUT,
        VELOCITY_OUTPUT,
        BEND_OUTPUT,
        MOD_OUTPUT,
        KNOB1_OUTPUT,
        KNOB2_OUTPUT,
        KNOB3_OUTPUT,
        KNOB4_OUTPUT,
        KNOB5_OUTPUT,
        KNOB6_OUTPUT,
        KNOB7_OUTPUT,
        KNOB8_OUTPUT,
        SLIDER1_OUTPUT,
        SLIDER2_OUTPUT,
        SLIDER3_OUTPUT,
        SLIDER4_OUTPUT,
        OUTPUTS_LEN
    };
    enum LightId { GATE_LIGHT, LIGHTS_LEN };

    int notesOn = 0;  // how many notes are currently on

    midi::InputQueue midiInput;
    MidiRouter* router;

    AbsoluteParam* gate;
    AbsoluteParam* velocity;
    AbsoluteParam* note;
    AbsoluteParam* bend;
    AbsoluteParam* mod;
    AbsoluteParam* sliders[4];
    RelativeParam* knobs[8];

    MiniLab3();
    json_t* dataToJson() override;
    void dataFromJson(json_t* rootJ) override;
    void process(const ProcessArgs& args) override;
    void processMidi(int frame);
    void onGateOpen();
    void onGateClose();
    void onNoteOn(NoteOnEvent event);
    void onNoteOff(NoteOffEvent event);
    void onPitchBend(PitchBendEvent event);
    void onModWheel(ModWheelEvent event);
    void onKnob(KnobEvent event);
    void onSlider(SliderEvent event);
};