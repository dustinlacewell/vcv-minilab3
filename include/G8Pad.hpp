#pragma once

#include <experimental/memory>
#include <rack.hpp>
#include <utils/PadBinder.hpp>
#include <utils/PadMidi.hpp>
#include <utils/RelativeParam.hpp>

#include "MiniLab3.hpp"
#include "menu/MenuSlider.hpp"
#include "plugin.hpp"
#include "ui/LedText.hpp"
#include "ui/OutputPort.hpp"
#include "utils/AbsoluteParam.hpp"
#include "utils/Relay.hpp"
#include "utils/RelayCallback.hpp"

using namespace std::experimental;

using namespace rack::dsp;

struct G8Pad : Module {
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
    PadMidi* padMidi;

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

struct G8PadWidget : ModuleWidget {
    LedTextDisplay* padIdText;

    explicit G8PadWidget(G8Pad* module);

    void step() override;
    void appendContextMenu(Menu* menu) override;

    OutputPort* createAbsolutePort(
        Vec pos, G8Pad* module, int outputId, AbsoluteParam* param
    );

    OutputPort* createRelativePort(
        Vec pos, G8Pad* module, int outputId, RelativeParam* param
    );
};