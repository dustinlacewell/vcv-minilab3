#pragma once

#include <experimental/memory>
#include <rack.hpp>
#include "MiniLab3.hpp"
#include "params/AbsoluteParam.hpp"
#include "params/RelativeParam.hpp"
#include "utils/PadBinder.hpp"
#include "widgets/BaseWidget.hpp"

using namespace std::experimental;

using namespace rack::dsp;

struct G8Pad : BaseModule {
    enum ParamId { PARAMS_LEN };
    enum InputId { INPUTS_LEN };
    enum OutputId {
        GATE_OUTPUT,
        VELOCITY_OUTPUT,
        BEND_OUTPUT,
        TOUCH_OUTPUT,
        MOD_OUTPUT,
        KNOB1_OUTPUT,
        KNOB2_OUTPUT,
        KNOB3_OUTPUT,
        KNOB4_OUTPUT,
        KNOB5_OUTPUT,
        KNOB6_OUTPUT,
        KNOB7_OUTPUT,
        KNOBS8_OUTPUT,
        OUTPUTS_LEN
    };
    enum LightId { GATE_LIGHT, LIGHTS_LEN };

    midi::InputQueue midiInput;
    dsp::ClockDivider divider;
    PadBinder* padBinder;
    MidiRouter* padMidi;

    AbsoluteParam* gate;
    AbsoluteParam* velocity;
    AbsoluteParam* bend;
    AbsoluteParam* mod;
    AbsoluteParam* touch;
    RelativeParam* knobs[8];

    G8Pad();

    json_t* dataToJson() override;
    void dataFromJson(json_t* rootJ) override;

    int getId() const;
    void process(const ProcessArgs& args) override;
    void processBinder();
    void processParams();
    void processMidi(int frame);
};