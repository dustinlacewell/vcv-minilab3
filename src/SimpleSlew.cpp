#include <rack.hpp>

#include "plugin.hpp"


using namespace rack;


struct SimpleSlew : Module {

    float lastOutput = 0.0f;

    enum ParamId {
        SPEED_PARAM, PARAMS_LEN
    };
    enum InputId {
        CV_INPUT, INPUTS_LEN
    };
    enum OutputId {
        CV_OUTPUT, OUTPUTS_LEN
    };
    enum LightId {
        LIGHTS_LEN
    };

    SimpleSlew() {
        config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
        configParam(SPEED_PARAM, 0.f, 1.f, 0.1f, "");
        configInput(CV_INPUT, "");
        configOutput(CV_OUTPUT, "");
    }

    void process(const ProcessArgs &args) override {
        // this simple slew is just a linear ramp
        float speed = params[SPEED_PARAM].getValue();
        float input = inputs[CV_INPUT].getVoltage();
        float sampleTime = args.sampleTime;
        float delta = input - lastOutput;
        float slew = speed * 10 * sampleTime;
        if (delta > slew) {
            lastOutput += slew;
        } else if (delta < -slew) {
            lastOutput -= slew;
        } else {
            lastOutput = input;
        }
        outputs[CV_OUTPUT].setVoltage(lastOutput);
    }
};


struct SimpleSlewWidget : ModuleWidget {
    SimpleSlewWidget(SimpleSlew *
    module) {
        setModule(module);
        setPanel(createPanel(asset::plugin(pluginInstance, "res/SimpleSlew.svg")));

        addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
        addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
        addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
        addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

        addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(7.62, 70.738)), module, SimpleSlew::SPEED_PARAM));

        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.62, 30.75)), module, SimpleSlew::CV_INPUT));

        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7.62, 109.747)), module, SimpleSlew::CV_OUTPUT));
    }
};


Model *modelSimpleSlew = createModel<SimpleSlew, SimpleSlewWidget>("SimpleSlew");