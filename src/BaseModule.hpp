#pragma once

#include <rack.hpp>
#include "params/AbsoluteParam.hpp"
#include "params/RelativeParam.hpp"

struct BaseModule : rack::Module {
    enum ParamIds { NUM_PARAMS };

    enum InputIds { MIDI_INPUT, NUM_INPUTS };

    enum LightIds { STATUS_LIGHT, NUM_LIGHTS };

    enum OutputIds {
        GATE_OUTPUT,
        VELOCITY_OUTPUT,
        BEND_OUTPUT,
        MOD_OUTPUT,
        TOUCH_OUTPUT,
        KNOB_OUTPUT,
        KNOB_OUTPUT_LAST = KNOB_OUTPUT + 7,
        SLIDER_OUTPUT,
        SLIDER_OUTPUT_LAST = SLIDER_OUTPUT + 3,
        NUM_OUTPUTS
    };

    int position = -1;
    bool isActive = false;

    std::vector<BaseParam*> params;
    midi::InputQueue midiInput;

    AbsoluteParam* gate = nullptr;
    AbsoluteParam* velocity = nullptr;
    AbsoluteParam* bend = nullptr;
    AbsoluteParam* mod = nullptr;
    AbsoluteParam* touch = nullptr;
    std::array<RelativeParam*, 8> knobs{};

    BaseModule() {
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
        configInput(MIDI_INPUT, "MIDI");
        configLight(STATUS_LIGHT, "Status");

        gate = createAbsoluteOutput(GATE_OUTPUT, "Gate", [](AbsoluteParam* p) {
            p->setSlew(0.0f);
            p->setRange(0, 1);
            p->setVoltageMode(VoltageMode::UNIPOLAR_5);
            p->setValue(0);
        });
        velocity = createAbsoluteOutput(
            VELOCITY_OUTPUT,
            "Velocity",
            [](AbsoluteParam* p) {
                p->setSlew(0.0f);
                p->setRange(0, 127);
                p->setVoltageMode(VoltageMode::UNIPOLAR_10);
                p->setValue(0);
            }
        );
        bend = createAbsoluteOutput(
            BEND_OUTPUT,
            "Pitch Bend",
            [](AbsoluteParam* p) {
                p->setSlew(0.0f);
                p->setRange(0, 127);
                p->setVoltageMode(VoltageMode::BIPOLAR_5);
                p->setValue(64);
            }
        );
        mod = createAbsoluteOutput(
            MOD_OUTPUT,
            "Modulation",
            [](AbsoluteParam* p) {
                p->setSlew(0.0f);
                p->setRange(0, 127);
                p->setVoltageMode(VoltageMode::UNIPOLAR_10);
                p->setValue(0);
            }
        );
        touch = createAbsoluteOutput(
            TOUCH_OUTPUT,
            "Aftertouch",
            [](AbsoluteParam* p) {
                p->setSlew(0.0f);
                p->setRange(0, 127);
                p->setVoltageMode(VoltageMode::UNIPOLAR_10);
                p->setValue(0);
            }
        );

        for (int i = 0; i < 8; i++) {
            knobs[i] = createRelativeOutput(
                KNOB_OUTPUT + i, rack::string::f("Knob %d", i + 1)
            );
        }
    }

    void processParams();
    AbsoluteParam* createAbsoluteOutput(int output, std::string label);
    AbsoluteParam* createAbsoluteOutput(
        int output,
        std::string label,
        std::function<void(AbsoluteParam*)> setupCallback
    );
    RelativeParam* createRelativeOutput(int output, std::string label);
    RelativeParam* createRelativeOutput(
        int output,
        std::string label,
        std::function<void(RelativeParam*)> setupCallback
    );

    void outputsToJson(json_t* rootJ);
    void outputsFromJson(json_t* rootJ);
    void onReset() override;
};