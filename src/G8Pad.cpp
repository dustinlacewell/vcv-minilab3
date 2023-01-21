#include <rack.hpp>

#include "G8Pad.hpp"
#include "widgets/G8PadWidget.hpp"

using namespace rack;

using namespace std::experimental;

G8Pad::G8Pad() : knobs{} {
    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
    configLight(GATE_LIGHT, "Gate Light");

    divider.setDivision(1024 * 4);

    gate = createAbsoluteOutput(GATE_OUTPUT, "Gate");
    gate->setSlew(0.0f);
    gate->setRange(0, 1);
    gate->setVoltageMode(VoltageMode::UNIPOLAR_10);

    velocity = createAbsoluteOutput(VELOCITY_OUTPUT, "Velocity");
    velocity->setVoltageMode(VoltageMode::UNIPOLAR_10);
    velocity->setSlew(0.1f);

    bend = createAbsoluteOutput(BEND_OUTPUT, "Bend");
    bend->setVoltageMode(VoltageMode::BIPOLAR_5);
    bend->setSlew(0.01f);

    mod = createAbsoluteOutput(MOD_OUTPUT, "Mod");
    mod->setSlew(0.01f);

    touch = createAbsoluteOutput(TOUCH_OUTPUT, "Touch");
    touch->setSlew(0.5f);
    touch->setVoltageMode(VoltageMode::UNIPOLAR_10);

    for (int i = 0; i < 8; i++) {
        knobs[i] = createRelativeOutput(
            KNOB1_OUTPUT + i, "Knob " + std::to_string(i + 1)
        );
        knobs[i]->setVoltageMode(VoltageMode::BIPOLAR_5);
    }

    padBinder = new PadBinder(&midiInput);
    padMidi = new MidiRouter(0);

    padMidi->onGateOpen([this]() {
        gate->send(1);
        lights[GATE_LIGHT].setBrightness(1.0);
    });

    padMidi->onGateClose([this]() {
        gate->send(0);
        bend->send(64);
        touch->send(0);
        lights[GATE_LIGHT].setBrightness(0.0);
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

json_t* G8Pad::dataToJson() {
    json_t* rootJ = json_object();
    json_object_set_new(rootJ, "midiInput", midiInput.toJson());
    json_object_set_new(rootJ, "padMidi", padMidi->toJson());
    json_object_set_new(rootJ, "gate", gate->toJson());
    json_object_set_new(rootJ, "velocity", velocity->toJson());
    json_object_set_new(rootJ, "bend", bend->toJson());
    json_object_set_new(rootJ, "mod", mod->toJson());
    json_object_set_new(rootJ, "touch", touch->toJson());

    json_t* knobsJ = json_array();
    for (int i = 0; i < 8; i++) {
        json_t* knobJ = knobs[i]->toJson();
        json_array_append_new(knobsJ, knobJ);
    }
    json_object_set_new(rootJ, "knobs", knobsJ);

    return rootJ;
}

void G8Pad::dataFromJson(json_t* rootJ) {
    json_t* bendJ = json_object_get(rootJ, "bend");
    if (bendJ) {
        bend->fromJson(bendJ);
    }

    json_t* gateJ = json_object_get(rootJ, "gate");
    if (gateJ) {
        gate->fromJson(gateJ);
    }

    json_t* velocityJ = json_object_get(rootJ, "velocity");
    if (velocityJ) {
        velocity->fromJson(velocityJ);
    }

    json_t* modJ = json_object_get(rootJ, "mod");
    if (modJ) {
        mod->fromJson(modJ);
    }

    json_t* touchJ = json_object_get(rootJ, "touch");
    if (touchJ) {
        touch->fromJson(touchJ);
    }

    json_t* knobsJ = json_object_get(rootJ, "knobs");
    if (knobsJ) {
        for (int i = 0; i < 8; i++) {
            json_t* knobJ = json_array_get(knobsJ, i);
            if (knobJ) {
                knobs[i]->fromJson(knobJ);
            }
        }
    }
}

Model* modelG8Pad = createModel<G8Pad, G8PadWidget>("G8Pad");