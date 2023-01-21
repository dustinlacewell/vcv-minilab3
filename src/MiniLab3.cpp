#include <rack.hpp>

#include "MiniLab3.hpp"
#include "widgets/MiniLab3Widget.hpp"

using namespace rack::dsp;

MiniLab3::MiniLab3() {
    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
    configLight(GATE_LIGHT, "MiniLab3 gate");

    lights[GATE_LIGHT].setBrightness(1.0f);

    gateParam = createAbsoluteOutput(GATE_OUTPUT, "Gate");
    gateParam->setSlew(0.0f);
    gateParam->setRange(0, 1);
    gateParam->setVoltageMode(VoltageMode::UNIPOLAR_10);

    velocityParam = createAbsoluteOutput(VELOCITY_OUTPUT, "Velocity");
    velocityParam->setVoltageMode(VoltageMode::UNIPOLAR_10);
    velocityParam->setSlew(0.1f);

    noteParam = createAbsoluteOutput(NOTE_OUTPUT, "Note");
    noteParam->setRange(0, 120);
    noteParam->setVoltageMode(VoltageMode::BIPOLAR_5);
    noteParam->setSlew(0.0f);

    bendParam = createAbsoluteOutput(BEND_OUTPUT, "Bend");
    bendParam->setSlew(0.01f);

    modParam = createAbsoluteOutput(MOD_OUTPUT, "Mod");
    modParam->setSlew(0.01f);

    for (int i = 0; i < 8; i++) {
        knobParams[i] = createRelativeOutput(
            KNOB1_OUTPUT + i, "Knob " + std::to_string(i + 1)
        );
        knobParams[i]->setSlew(0.01f);
        knobParams[i]->send(0);
    }

    for (int i = 0; i < 4; i++) {
        sliderParams[i] = createAbsoluteOutput(
            SLIDER1_OUTPUT + i, "Slider " + std::to_string(i + 1)
        );
        sliderParams[i]->setSlew(0.01f);
        sliderParams[i]->send(0);
    }

    midiRouter = new MidiRouter(-1);

    midiRouter->onGateOpen([this]() { lights[GATE_LIGHT].setBrightness(1.0); });
    midiRouter->onGateClose([this]() { lights[GATE_LIGHT].setBrightness(0.0); }
    );

    midiRouter->onNoteOn([this](NoteOnEvent e) {
        noteParam->send(e.note);
        velocityParam->send(e.velocity);
        gateParam->send(1);
        notesOn++;
    });

    midiRouter->onNoteOff([this](NoteOffEvent e) {
        notesOn--;
        if (notesOn == 0) {
            gateParam->send(0);
        }
    });

    midiRouter->onPitchBend([this](PitchBendEvent e) {
        bendParam->send(e.pitchBend);
    });

    midiRouter->onModWheel([this](ModWheelEvent e) {
        modParam->send(e.modWheel);
    });

    midiRouter->onKnob([this](KnobEvent e) {
        knobParams[e.knob]->send(e.value);
    });

    midiRouter->onSlider([this](SliderEvent e) {
        sliderParams[e.slider]->send(e.value);
    });
}

json_t* MiniLab3::dataToJson() {
    json_t* rootJ = json_object();
    json_object_set_new(rootJ, "midiInput", midiInput.toJson());
    json_object_set_new(rootJ, "bend", bendParam->toJson());
    json_object_set_new(rootJ, "mod", modParam->toJson());
    json_t* knobsJ = json_array();
    for (int i = 0; i < 8; i++) {
        json_t* knobJ = knobParams[i]->toJson();
        json_array_append_new(knobsJ, knobJ);
    }
    json_object_set_new(rootJ, "knobs", knobsJ);
    json_t* slidersJ = json_array();
    for (int i = 0; i < 4; i++) {
        json_t* sliderJ = sliderParams[i]->toJson();
        json_array_append_new(slidersJ, sliderJ);
    }
    json_object_set_new(rootJ, "sliders", slidersJ);
    json_object_set_new(rootJ, "midiRouter", midiRouter->toJson());
    return rootJ;
}

void MiniLab3::dataFromJson(json_t* rootJ) {
    // midi input
    json_t* midiInputJ = json_object_get(rootJ, "midiInput");
    if (midiInputJ) {
        midiInput.fromJson(midiInputJ);
    }
    json_t* bendJ = json_object_get(rootJ, "bend");
    if (bendJ) {
        bendParam->fromJson(bendJ);
    }
    json_t* modJ = json_object_get(rootJ, "mod");
    if (modJ) {
        modParam->fromJson(modJ);
    }
    json_t* knobsJ = json_object_get(rootJ, "knobs");
    if (knobsJ) {
        for (int i = 0; i < 8; i++) {
            json_t* knobJ = json_array_get(knobsJ, i);
            if (knobJ) {
                knobParams[i]->fromJson(knobJ);
            }
        }
    }
    json_t* slidersJ = json_object_get(rootJ, "sliders");
    if (slidersJ) {
        for (int i = 0; i < 4; i++) {
            json_t* sliderJ = json_array_get(slidersJ, i);
            if (sliderJ) {
                sliderParams[i]->fromJson(sliderJ);
            }
        }
    }
}

void MiniLab3::process(const ProcessArgs& args) {
    midi::Message msg;
    while (midiInput.tryPop(&msg, args.frame)) {
        midiRouter->processMessage(msg);
    }

    BaseModule::process(args);
}

Model* modelMiniLab3 = createModel<MiniLab3, MiniLab3Widget>("MiniLab3");
