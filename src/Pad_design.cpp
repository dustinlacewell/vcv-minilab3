#include "plugin.hpp"

struct G8Pad : Module {
    enum ParamId { PARAMS_LEN };
    enum InputId { INPUTS_LEN };
    enum OutputId {
        GATE_OUTPUT,
        TOUCH_OUTPUT,
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
        KNOBS8_OUTPUT,
        OUTPUTS_LEN
    };
    enum LightId { LIGHTS_LEN };

    G8Pad() {
        config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
        configOutput(GATE_OUTPUT, "");
        configOutput(TOUCH_OUTPUT, "");
        configOutput(VELOCITY_OUTPUT, "");
        configOutput(BEND_OUTPUT, "");
        configOutput(MOD_OUTPUT, "");
        configOutput(KNOB1_OUTPUT, "");
        configOutput(KNOB2_OUTPUT, "");
        configOutput(KNOB3_OUTPUT, "");
        configOutput(KNOB4_OUTPUT, "");
        configOutput(KNOB5_OUTPUT, "");
        configOutput(KNOB6_OUTPUT, "");
        configOutput(KNOB7_OUTPUT, "");
        configOutput(KNOBS8_OUTPUT, "");
    }

    void process(const ProcessArgs& args) override {}
};

struct G8PadWidget : ModuleWidget {
    G8PadWidget(G8Pad* module) {
        setModule(module);
        setPanel(createPanel(asset::plugin(pluginInstance, "res/G8Pad.svg")));

        addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
        addChild(
            createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0))
        );
        addChild(createWidget<ScrewSilver>(
            Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)
        ));
        addChild(createWidget<ScrewSilver>(Vec(
            box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH
        )));

        addOutput(createOutputCentered<PJ301MPort>(
            mm2px(), module, G8Pad::GATE_OUTPUT
        ));
        addOutput(createOutputCentered<PJ301MPort>(
            mm2px(), module, G8Pad::TOUCH_OUTPUT
        ));
        addOutput(createOutputCentered<PJ301MPort>(
            mm2px(), module, G8Pad::VELOCITY_OUTPUT
        ));
        addOutput(createOutputCentered<PJ301MPort>(
            mm2px(), module, G8Pad::BEND_OUTPUT
        ));
        addOutput(
            createOutputCentered<PJ301MPort>(mm2px(), module, G8Pad::MOD_OUTPUT)
        );
        addOutput(createOutputCentered<PJ301MPort>(
            mm2px(), module, G8Pad::KNOB1_OUTPUT
        ));
        addOutput(createOutputCentered<PJ301MPort>(
            mm2px(), module, G8Pad::KNOB2_OUTPUT
        ));
        addOutput(createOutputCentered<PJ301MPort>(
            mm2px(), module, G8Pad::KNOB3_OUTPUT
        ));
        addOutput(createOutputCentered<PJ301MPort>(
            mm2px(), module, G8Pad::KNOB4_OUTPUT
        ));
        addOutput(createOutputCentered<PJ301MPort>(
            mm2px(), module, G8Pad::KNOB5_OUTPUT
        ));
        addOutput(createOutputCentered<PJ301MPort>(
            mm2px(), module, G8Pad::KNOB6_OUTPUT
        ));
        addOutput(createOutputCentered<PJ301MPort>(
            mm2px(), module, G8Pad::KNOB7_OUTPUT
        ));
        addOutput(createOutputCentered<PJ301MPort>(
            mm2px(), module, G8Pad::KNOBS8_OUTPUT
        ));
    }
};

Model* modelG8Pad = createModel<G8Pad, G8PadWidget>("G8Pad");