#pragma once

#include <rack.hpp>
#include "utils/Relay.hpp"
#include "utils/EventedInputQueue.hpp"
#include "slew/SlewVoltage.hpp"
#include "slew/SlewLimitQuantity.hpp"
#include "utils/VoltageMode.hpp"

// use the rack::dsp namespace for convenience
using namespace rack::dsp;


struct MiniLab3 : Module {
    enum ParamId {
        PARAMS_LEN
    };
    enum InputId {
        INPUTS_LEN
    };
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
    enum LightId {
        LIGHTS_LEN
    };

    EventedInputQueue midiInput;
    Relay<midi::Message &> midiRelay;

    // 16 boolean gates
    bool gates[16] = {false};

    SlewLimitQuantity *bendLimit = new SlewLimitQuantity("Bend Slew", 0, 10);
    SlewVoltage *bend = new SlewVoltage(bendLimit, VoltageMode::BIPOLAR_1, 0, 16383);

    SlewLimitQuantity *modLimit = new SlewLimitQuantity("Mod Slew", 0, 10);
    SlewVoltage *mod = new SlewVoltage(modLimit);

    float sampleTime = 0.0f;

    int knobStrength = 2;
    VoltageMode knobVoltageMode = VoltageMode::BIPOLAR_1;
    SlewLimitQuantity *knobLimit = new SlewLimitQuantity("Knob Slew", 0, 0.8f);
    // 8 SlewVoltages
    SlewVoltage *knobs[8] = {new SlewVoltage(knobLimit), new SlewVoltage(knobLimit), new SlewVoltage(knobLimit), new SlewVoltage(knobLimit), new SlewVoltage(knobLimit), new SlewVoltage(knobLimit), new SlewVoltage(knobLimit), new SlewVoltage(knobLimit)};

    VoltageMode sliderVoltageMode = VoltageMode::UNIPOLAR_1;
    SlewLimitQuantity *sliderLimit = new SlewLimitQuantity("Slider Slew", 0, 0.8f);
    // 4 SlewVoltages
    SlewVoltage *sliders[4] = {new SlewVoltage(sliderLimit), new SlewVoltage(sliderLimit), new SlewVoltage(sliderLimit), new SlewVoltage(sliderLimit)};

    MiniLab3();
    json_t *dataToJson() override;
    void dataFromJson(json_t *rootJ) override;
    void process(const ProcessArgs &args) override;
    void processMidi(midi::Message &msg);
    void processNoteOn(midi::Message &msg);
    void processNoteOff(midi::Message &msg);
    void processPitchBend(midi::Message &msg);
    void processControlChange(midi::Message &msg);
    void processKnob(int knob, int value);
    void processSlider(int slider, int value);
    bool gateIsOpen();
    int isGateKey(midi::Message &msg);
};