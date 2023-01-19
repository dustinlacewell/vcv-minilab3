#include <G8Pad.hpp>
#include <menu/AbsoluteParamMenu.hpp>
#include <rack.hpp>
#include <ui/LedText.hpp>
#include <ui/OutputPort.hpp>
#include <utils/AbsoluteParam.hpp>
#include <utils/PadBinder.hpp>
#include <utils/RelativeParam.hpp>

#include "../plugin.hpp"

using namespace rack;

using namespace std::experimental;

G8Pad::G8Pad() : knobs{} {
    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
    configOutput(GATE_OUTPUT, "Pad Gate");
    configOutput(VELOCITY_OUTPUT, "Pad Velocity");
    configOutput(BEND_OUTPUT, "Pitch Bend");
    configOutput(TOUCH_OUTPUT, "Aftertouch");
    configOutput(MOD_OUTPUT, "Mod-wheel");
    configOutput(KNOB1_OUTPUT, "Knob 1");
    configOutput(KNOB2_OUTPUT, "Knob 2");
    configOutput(KNOB3_OUTPUT, "Knob 3");
    configOutput(KNOB4_OUTPUT, "Knob 4");
    configOutput(KNOB5_OUTPUT, "Knob 5");
    configOutput(KNOB6_OUTPUT, "Knob 6");
    configOutput(KNOB7_OUTPUT, "Knob 7");
    configOutput(KNOBS8_OUTPUT, "Knob 8");
    configLight(GATE_LIGHT, "Gate Light");

    divider.setDivision(1024 * 4);

    bend = new AbsoluteParam(&outputs[BEND_OUTPUT]);
    mod = new AbsoluteParam(&outputs[MOD_OUTPUT]);
    touch = new AbsoluteParam(&outputs[TOUCH_OUTPUT]);

    for (int i = 0; i < 8; i++) {
        knobs[i] = new RelativeParam(&outputs[KNOB1_OUTPUT + i]);
    }

    padBinder = new PadBinder(&midiInput);
    padMidi = new PadMidi(-1);

    padMidi->onGateOpen([this](GateOpenEvent e) {
        outputs[GATE_OUTPUT].setVoltage(10.0f);
        outputs[VELOCITY_OUTPUT].setVoltage(e.velocity / 127.0f * 10.0f);
        lights[GATE_LIGHT].setBrightness(1.0);
    });

    padMidi->onGateClose([this]() {
        outputs[GATE_OUTPUT].setVoltage(0.0f);
        lights[GATE_LIGHT].setBrightness(0.0);

        bend->send(64);
        touch->send(0);
    });

    padMidi->onAftertouch([this](AftertouchEvent e) {
        touch->send(e.aftertouch);
    });
    padMidi->onPitchBend([this](PitchBendEvent e) { bend->send(e.pitchBend); });
    padMidi->onModWheel([this](ModWheelEvent e) { mod->send(e.modWheel); });
    padMidi->onKnob([this](KnobEvent e) { knobs[e.knob]->send(e.value); });
}

int G8Pad::getId() const {
    return padMidi->padId;
}

void G8Pad::processParams() {
    touch->process();
    bend->process();
    mod->process();
    for (auto& knob : knobs) {
        knob->process();
    }
}

void G8Pad::processMidi(int frame) {
    midi::Message msg;
    while (midiInput.tryPop(&msg, frame)) {
        padMidi->processMessage(msg);
    }
}

void G8Pad::processBinder() {
    if (divider.process()) {
        padMidi->padId = padBinder->process(leftExpander.module);
    }
}

void G8Pad::process(const ProcessArgs& args) {
    processBinder();
    processMidi(args.frame);
    processParams();
}

Model* modelG8Pad = createModel<G8Pad, G8PadWidget>("G8Pad");