#include <rack.hpp>

#include "G8Pad.hpp"
#include "params/helpers.hpp"
#include "widgets/G8PadWidget.hpp"

using namespace rack;

using namespace std::experimental;

G8Pad::G8Pad() : knobs{} {
    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
    configLight(GATE_LIGHT, "Gate Light");

    divider.setDivision(1024 * 4);

    gate = createAbsoluteOutput(GATE_OUTPUT, "Gate", [](auto p) {
        p->setSlew(0.0f);
        p->setRange(0, 1);
        p->setVoltageMode(VoltageMode::UNIPOLAR_10);
        p->setValue(0);
    });

    velocity = createAbsoluteOutput(VELOCITY_OUTPUT, "Velocity", [](auto p) {
        p->setVoltageMode(VoltageMode::UNIPOLAR_10);
        p->setSlew(0.25f);
        p->setValue(0);
    });

    bend = createAbsoluteOutput(BEND_OUTPUT, "Bend", [](auto p) {
        p->setSlew(0.25f);
        p->setVoltageMode(VoltageMode::BIPOLAR_5);
        p->setValue(64);
    });

    mod = createAbsoluteOutput(MOD_OUTPUT, "Mod", [](auto p) {
        p->setSlew(0.25f);
        p->setVoltageMode(VoltageMode::UNIPOLAR_10);
        p->setValue(0);
    });

    touch = createAbsoluteOutput(TOUCH_OUTPUT, "Touch", [](auto p) {
        p->setSlew(0.25f);
        p->setVoltageMode(VoltageMode::UNIPOLAR_10);
        p->setValue(0);
    });

    for (int i = 0; i < 8; i++) {
        knobs[i] = makeKnob(KNOB1_OUTPUT, i, this);
    }

    binder = new PadBinder(&midiInput);
    router = new MidiRouter(0);

    router->onGateOpen([this]() { onGateOpen(); });
    router->onGateClose([this]() { onGateClose(); });
    router->onNoteOn([this](auto event) { onNoteOn(event); });
    router->onAftertouch([this](auto e) { onAftertouch(e); });
    router->onPitchBend([this](auto e) { onPitchBend(e); });
    router->onModWheel([this](auto e) { onModWheel(e); });
    router->onKnob([this](auto e) { onKnob(e); });
}

void G8Pad::onGateOpen() {
    gate->send(1);
    lights[GATE_LIGHT].setBrightness(1.0);
}

void G8Pad::onGateClose() {
    gate->send(0);
    bend->send(64);
    touch->send(0);
    lights[GATE_LIGHT].setBrightness(0.0);
}

void G8Pad::onNoteOn(NoteOnEvent event) {
    velocity->send(event.velocity);
}

void G8Pad::onPitchBend(PitchBendEvent event) {
    bend->send(event.pitchBend);
}

void G8Pad::onModWheel(ModWheelEvent event) {
    mod->send(event.modWheel);
}

void G8Pad::onAftertouch(AftertouchEvent event) {
    touch->send(event.aftertouch);
}

void G8Pad::onKnob(KnobEvent event) {
    knobs[event.knob]->send(event.value);
}

int G8Pad::getId() const {
    return router->padId;
}

void G8Pad::processMidi(int frame) {
    midi::Message msg;
    while (midiInput.tryPop(&msg, frame)) {
        router->processMessage(msg);
    }
}

void G8Pad::processBinder() {
    if (divider.process()) {
        router->padId = binder->process(leftExpander.module);
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
    json_object_set_new(rootJ, "router", router->toJson());
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