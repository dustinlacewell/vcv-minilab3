#include <rack.hpp>

#include "MiniLab3.hpp"
#include "MiniLab3Widget.hpp"
#include "menu/MenuSlider.hpp"
#include "plugin.hpp"
#include "ui/OutputPort.hpp"

// use the rack::dsp namespace for convenience
using namespace rack::dsp;

MiniLab3::MiniLab3() {
    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
    configOutput(NOTE_OUTPUT, "10V Note");
    configOutput(GATE_OUTPUT, "10V Gate");
    configOutput(VELOCITY_OUTPUT, "10V Velocity");
    configOutput(BEND_OUTPUT, "1V Bend");
    configOutput(MOD_OUTPUT, "1V Mod");
    configOutput(KNOB1_OUTPUT, "1V Knob 1");
    configOutput(KNOB2_OUTPUT, "1V Knob 2");
    configOutput(KNOB3_OUTPUT, "1V Knob 3");
    configOutput(KNOB4_OUTPUT, "1V Knob 4");
    configOutput(KNOB5_OUTPUT, "1V Knob 5");
    configOutput(KNOB6_OUTPUT, "1V Knob 6");
    configOutput(KNOB7_OUTPUT, "1V Knob 7");
    configOutput(KNOB8_OUTPUT, "1V Knob 8");
    configOutput(SLIDER1_OUTPUT, "1V Slider 1");
    configOutput(SLIDER2_OUTPUT, "1V Slider 2");
    configOutput(SLIDER3_OUTPUT, "1V Slider 3");
    configOutput(SLIDER4_OUTPUT, "1V Slider 4");

    bendParam = new AbsoluteParam(&outputs[BEND_OUTPUT]);
    bendParam->slewLimitQuantity->setValue(0.01f);
    modParam = new AbsoluteParam(&outputs[MOD_OUTPUT]);
    modParam->slewLimitQuantity->setValue(0.01f);

    for (int i = 0; i < 8; i++) {
        knobParams[i] = new RelativeParam(&outputs[KNOB1_OUTPUT + i]);
        knobParams[i]->slewLimitQuantity->setValue(0.01f);
    }

    for (int i = 0; i < 4; i++) {
        sliderParams[i] = new AbsoluteParam(&outputs[SLIDER1_OUTPUT + i]);
        sliderParams[i]->slewLimitQuantity->setValue(0.01f);
    }

    midiRouter = new MidiRouter(-1);

    midiRouter->onGateOpen([this]() { lights[GATE_LIGHT].setBrightness(1.0); });

    midiRouter->onGateClose([this]() { lights[GATE_LIGHT].setBrightness(0.0); }
    );

    midiRouter->onNoteOn([this](NoteOnEvent e) {
        float noteVoltage = e.note / 12.0 - 5.0f;
        outputs[NOTE_OUTPUT].setVoltage(noteVoltage);
        outputs[VELOCITY_OUTPUT].setVoltage(1.0f);
        outputs[GATE_OUTPUT].setVoltage(10.0f);
    });

    midiRouter->onNoteOff([this](NoteOffEvent e) {
        outputs[GATE_OUTPUT].setVoltage(0.0f);
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

    json_object_set_new(rootJ, "bendLimit", bendLimit->toJson());
    json_object_set_new(rootJ, "bend", bend->toJson());

    json_object_set_new(rootJ, "modLimit", modLimit->toJson());
    json_object_set_new(rootJ, "mod", mod->toJson());

    // knob strength: int
    json_object_set_new(rootJ, "knobStrength", json_integer(knobStrength));
    // knob voltage mode
    json_object_set_new(
        rootJ, "knobVoltageMode", json_integer(knobVoltageMode)
    );
    // knob slew limit
    json_object_set_new(rootJ, "knobLimit", knobLimit->toJson());
    // 8 SlewVoltage->toJson objects from knobs as json array
    json_t* knobsJ = json_array();
    for (int i = 0; i < 8; i++) {
        json_array_append_new(knobsJ, knobs[i]->toJson());
    }
    json_object_set_new(rootJ, "knobs", knobsJ);

    // slider voltage mode
    json_object_set_new(
        rootJ, "sliderVoltageMode", json_integer(sliderVoltageMode)
    );
    // slider slew limit
    json_object_set_new(rootJ, "sliderLimit", sliderLimit->toJson());
    // 4 SlewVoltage->toJson objects from sliders as json array
    json_t* slidersJ = json_array();
    for (int i = 0; i < 4; i++) {
        json_array_append_new(slidersJ, sliders[i]->toJson());
    }
    json_object_set_new(rootJ, "sliders", slidersJ);

    return rootJ;
}

void MiniLab3::dataFromJson(json_t* rootJ) {
    // midi input
    json_t* midiInputJ = json_object_get(rootJ, "midiInput");
    if (midiInputJ) {
        midiInput.fromJson(midiInputJ);
    }

    // bend slew limit
    json_t* bendLimitJ = json_object_get(rootJ, "bendLimit");
    if (bendLimitJ) {
        bendLimit->fromJson(bendLimitJ);
    }
    // bend voltage
    json_t* bendJ = json_object_get(rootJ, "bend");
    if (bendJ) {
        bend->fromJson(bendJ, bendLimit);
    }

    // mod slew limit
    json_t* modLimitJ = json_object_get(rootJ, "modLimit");
    if (modLimitJ) {
        modLimit->fromJson(modLimitJ);
    }
    // mod voltage
    json_t* modJ = json_object_get(rootJ, "mod");
    if (modJ) {
        mod->fromJson(modJ, modLimit);
    }

    // knob strength
    json_t* knobStrengthJ = json_object_get(rootJ, "knobStrength");
    if (knobStrengthJ) {
        knobStrength = json_integer_value(knobStrengthJ);
    }
    // knob voltage mode
    json_t* knobVoltageModeJ = json_object_get(rootJ, "knobVoltageMode");
    if (knobVoltageModeJ) {
        knobVoltageMode = (VoltageMode)json_integer_value(knobVoltageModeJ);
    }
    // knob slew limit
    json_t* knobLimitJ = json_object_get(rootJ, "knobLimit");
    if (knobLimitJ) {
        knobLimit->fromJson(knobLimitJ);
    }
    // knob voltages
    json_t* knobsJ = json_object_get(rootJ, "knobs");
    if (knobsJ) {
        for (int i = 0; i < 8; i++) {
            json_t* knobJ = json_array_get(knobsJ, i);
            if (knobJ) {
                knobs[i]->fromJson(knobJ, knobLimit);
            }
        }
    }

    // slider voltage mode
    json_t* sliderVoltageModeJ = json_object_get(rootJ, "sliderVoltageMode");
    if (sliderVoltageModeJ) {
        sliderVoltageMode = (VoltageMode)json_integer_value(sliderVoltageModeJ);
    }
    // slider slew limit
    json_t* sliderLimitJ = json_object_get(rootJ, "sliderLimit");
    if (sliderLimitJ) {
        sliderLimit->fromJson(sliderLimitJ);
    }
    // slider voltages
    json_t* slidersJ = json_object_get(rootJ, "sliders");
    if (slidersJ) {
        for (int i = 0; i < 4; i++) {
            json_t* sliderJ = json_array_get(slidersJ, i);
            if (sliderJ) {
                sliders[i]->fromJson(sliderJ, sliderLimit);
            }
        }
    }
}

void MiniLab3::process(const ProcessArgs& args) {

    midi::Message msg;
    while (midiInput.tryPop(&msg, args.frame)) {
        midiRouter->processMessage(msg);
    }

    if (midiRouter->gateOpen) {
        lights[GATE_LIGHT].setBrightness(1.0);
    } else {
        lights[GATE_LIGHT].setBrightness(0.0);
    }

    bendParam->process();
    modParam->process();
    for (auto& knob : knobParams) {
        knob->process();
    }
    for (auto& slider : sliderParams) {
        slider->process();
    }

    //    outputs[BEND_OUTPUT].setVoltage(
    //        bend->getVoltage(args.sampleTime, args.sampleRate)
    //    );
    //    outputs[MOD_OUTPUT].setVoltage(
    //        mod->getVoltage(args.sampleTime, args.sampleRate)
    //    );
    //    for (int i = 0; i < 8; i++) {
    //        outputs[KNOB1_OUTPUT + i].setVoltage(
    //            knobs[i]->getVoltage(args.sampleTime, args.sampleRate)
    //        );
    //    }
    //    for (int i = 0; i < 4; i++) {
    //        outputs[SLIDER1_OUTPUT + i].setVoltage(
    //            sliders[i]->getVoltage(args.sampleTime, args.sampleRate)
    //        );
    //    }
}

bool MiniLab3::gateIsOpen() {
    for (int i = 0; i < 16; i++) {
        if (gates[i]) {
            return false;
        }
    }
    return true;
}

int MiniLab3::isGateKey(midi::Message& msg) {
    if (msg.getChannel() != 15) {
        return -1;
    }

    if (msg.getStatus() != 0x9 && msg.getStatus() != 0x8) {
        return -1;
    }

    int note = msg.getNote();
    switch (note) {
        case 36:
            return 0;
        case 48:
            return 1;
        case 60:
            return 2;
        case 72:
            return 3;
        case 84:
            return 4;
        case 96:
            return 5;
        case 108:
            return 6;
        case 120:
            return 7;
        case 38:
            return 8;
        case 50:
            return 9;
        case 62:
            return 10;
        case 74:
            return 11;
        case 86:
            return 12;
        case 98:
            return 13;
        case 110:
            return 14;
        case 122:
            return 15;
        default:
            return -1;
    }
}

void MiniLab3::processMidi(midi::Message& msg) {
    int isPad = this->isGateKey(msg);

    if (isPad > -1) {
        switch (msg.getStatus()) {
            case 0x9:  // note on
                gates[isPad] = true;
                break;
            case 0x8:  // note off
                gates[isPad] = false;
                break;
        }

        return;
    }

    bool gateIsOpen = this->gateIsOpen();

    switch (msg.getStatus()) {
        case 0x9:  // note on
            processNoteOn(msg);
            break;
        case 0x8:  // note off
            if (isPad > -1) {
                gates[isPad] = false;
            } else {
                processNoteOff(msg);
            }
            break;
        case 0xe:  // pitch bend
            if (gateIsOpen) {
                processPitchBend(msg);
            }
            break;
        case 0xb:  // control change
            processControlChange(msg);
            break;
        default:
            break;
    }

    midiRelay.processMessage(msg);
}

void MiniLab3::processNoteOn(midi::Message& msg) {
    int note = msg.getNote();
    float noteVoltage = note / 12.0;
    int velocity = msg.getValue();
    float velocityVoltage = velocity / 127.0 * 10.0;
    outputs[NOTE_OUTPUT].setVoltage(noteVoltage);
    outputs[VELOCITY_OUTPUT].setVoltage(velocityVoltage);
    outputs[GATE_OUTPUT].setVoltage(10.0);
}

void MiniLab3::processNoteOff(midi::Message& msg) {
    outputs[VELOCITY_OUTPUT].setVoltage(0);
    outputs[GATE_OUTPUT].setVoltage(0);
}

void MiniLab3::processPitchBend(midi::Message& msg) {
    bend->setTarget(((uint16_t)msg.getValue() << 7) | msg.getNote());
}

void MiniLab3::processKnob(int knob, int value) {
    if (value == 62) {
        value = -knobStrength;
    } else if (value == 66) {
        value = knobStrength;
    } else {
        value = 0;
    }
    SlewVoltage* slew = knobs[knob];
    float newTarget = slew->getTarget() + value;
    float clampedTarget = clamp(newTarget, 0.0f, 127.0f);
    slew->setTarget(clampedTarget);
}

void MiniLab3::processSlider(int slider, int value) {
    SlewVoltage* slew = sliders[slider];
    slew->setTarget(value);
}

void MiniLab3::processControlChange(midi::Message& msg) {
    int note = msg.getNote();
    bool gateIsOpen = this->gateIsOpen();
    switch (note) {
        case 1:
            if (gateIsOpen) {
                mod->setTarget(msg.getValue());
            }
            break;
        case 80:
            sliders[0]->setTarget(msg.getValue());
            break;
        case 81:
            sliders[1]->setTarget(msg.getValue());
            break;
        case 82:
            sliders[2]->setTarget(msg.getValue());
            break;
        case 83:
            sliders[3]->setTarget(msg.getValue());
            break;
        case 102:
        case 103:
        case 104:
        case 105:
        case 106:
        case 107:
        case 108:
        case 109:
            if (gateIsOpen) {
                processKnob(note - 102, msg.getValue());
            }
            break;
        default:
            break;
    }
}

Model* modelMiniLab3 = createModel<MiniLab3, MiniLab3Widget>("MiniLab3");
