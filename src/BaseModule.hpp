#pragma once

#include <array>
#include <memory>
#include <rack.hpp>
#include <vector>
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
    midi::InputQueue midiInput;

    std::vector<std::unique_ptr<BaseParam>> params;
    AbsoluteParam* gate;
    AbsoluteParam* velocity;
    AbsoluteParam* bend;
    AbsoluteParam* mod;
    AbsoluteParam* touch;
    std::array<RelativeParam*, 8> knobs{};

    BaseModule();

    json_t* dataToJson() override;
    void dataFromJson(json_t* rootJ) override;
    void onReset() override;

   protected:
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

   private:
    void outputsToJson(json_t* rootJ);
    void outputsFromJson(json_t* rootJ);
};