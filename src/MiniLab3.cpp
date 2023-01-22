#include <rack.hpp>

#include "MiniLab3.hpp"
#include "params/helpers.hpp"
#include "widgets/MiniLab3Widget.hpp"

using namespace rack::dsp;

MiniLab3::MiniLab3() {
    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
    configLight(GATE_LIGHT, "MiniLab3 gate");

    lights[GATE_LIGHT].setBrightness(1.0f);

    gate = createAbsoluteOutput(GATE_OUTPUT, "Gate", [](auto p) {
        p->setSlew(0.0f);
        p->setRange(0, 1);
        p->setVoltageMode(VoltageMode::UNIPOLAR_5);
        p->setValue(0);
    });

    velocity = createAbsoluteOutput(VELOCITY_OUTPUT, "Velocity", [](auto p) {
        p->setVoltageMode(VoltageMode::UNIPOLAR_10);
        p->setSlew(0.25f);
        p->setValue(0);
    });

    note = createAbsoluteOutput(NOTE_OUTPUT, "Note", [](auto p) {
        p->setVoltageMode(VoltageMode::UNIPOLAR_10);
        p->setRange(0, 120);
        p->setSlew(0.0f);
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

    for (int i = 0; i < 8; i++) {
        knobs[i] = makeKnob(KNOB1_OUTPUT, i, this);
    }

    for (int i = 0; i < 4; i++) {
        sliders[i] = makeSlider(SLIDER1_OUTPUT, i, this);
    }

    router = new MidiRouter(-1);
    router->onGateOpen([this]() { onGateOpen(); });
    router->onGateClose([this]() { onGateClose(); });
    router->onNoteOn([this](auto event) { onNoteOn(event); });
    router->onNoteOff([this](auto e) { onNoteOff(e); });
    router->onPitchBend([this](auto e) { onPitchBend(e); });
    router->onModWheel([this](auto e) { onModWheel(e); });
    router->onKnob([this](auto e) { onKnob(e); });
    router->onSlider([this](auto e) { onSlider(e); });
}

void MiniLab3::onGateOpen() {
    lights[GATE_LIGHT].setBrightness(1.0);
}

void MiniLab3::onGateClose() {
    lights[GATE_LIGHT].setBrightness(0.0);
}

void MiniLab3::onNoteOn(NoteOnEvent event) {
    note->send(event.note);
    velocity->send(event.velocity);
    gate->send(1);
    notesOn++;
}

void MiniLab3::onNoteOff(NoteOffEvent event) {
    notesOn--;
    if (notesOn == 0) {
        gate->send(0);
    }
}

void MiniLab3::onPitchBend(PitchBendEvent event) {
    bend->send(event.pitchBend);
}

void MiniLab3::onModWheel(ModWheelEvent event) {
    mod->send(event.modWheel);
}

void MiniLab3::onKnob(KnobEvent event) {
    knobs[event.knob]->send(event.value);
}

void MiniLab3::onSlider(SliderEvent event) {
    sliders[event.slider]->send(event.value);
}

void MiniLab3::processMidi(int frame) {
    midi::Message msg;
    while (midiInput.tryPop(&msg, frame)) {
        router->processMessage(msg);
    }
}

void MiniLab3::process(const ProcessArgs& args) {
    processMidi(args.frame);
    processParams();
}

json_t* MiniLab3::dataToJson() {
    json_t* rootJ = json_object();
    json_object_set_new(rootJ, "midiInput", midiInput.toJson());
    json_object_set_new(rootJ, "router", router->toJson());
    outputsToJson(rootJ);
    return rootJ;
}

void MiniLab3::dataFromJson(json_t* rootJ) {
    json_t* midiInputJ = json_object_get(rootJ, "midiInput");
    if (midiInputJ) {
        midiInput.fromJson(midiInputJ);
    }
    json_t* routerJ = json_object_get(rootJ, "router");
    if (routerJ) {
        router->fromJson(routerJ);
    }
    outputsFromJson(rootJ);
}

Model* modelMiniLab3 = createModel<MiniLab3, MiniLab3Widget>("MiniLab3");
