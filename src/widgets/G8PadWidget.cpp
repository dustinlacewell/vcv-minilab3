#include <rack.hpp>

#include "G8Pad.hpp"
#include "G8PadWidget.hpp"

#include "plugin.hpp"

G8PadWidget::G8PadWidget(G8Pad* module) {

    setModule(module);
    setPanel(createPanel(asset::plugin(pluginInstance, "res/G8Pad.svg")));

    addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0))
    );
    addChild(createWidget<ScrewSilver>(
        Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)
    ));
    addChild(createWidget<ScrewSilver>(Vec(
        box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH
    )));

    addOutput(createOutputCentered<PJ301MPort>(
        mm2px(Vec(15.189, 24.457)), module, G8Pad::GATE_OUTPUT
    ));

    createAbsolutePort(
        Vec(20.72, 58.734),
        module,
        G8Pad::MOD_OUTPUT,
        [](G8Pad* pad) { return pad->mod; }
    );

    createAbsolutePort(
        Vec(9.803, 41.619),
        module,
        G8Pad::TOUCH_OUTPUT,
        [](G8Pad* pad) { return pad->touch; }
    );

    createAbsolutePort(
        Vec(9.875, 58.734),
        module,
        G8Pad::BEND_OUTPUT,
        [](G8Pad* pad) { return pad->bend; }
    );

    addOutput(createOutputCentered<PJ301MPort>(
        mm2px(Vec(20.677, 41.619)), module, G8Pad::VELOCITY_OUTPUT
    ));

    createRelativePort(
        Vec(9.847, 78.694),
        module,
        G8Pad::KNOB1_OUTPUT,
        [](G8Pad* pad) { return pad->knobs[0]; }
    );
    createRelativePort(
        Vec(20.72, 78.694),
        module,
        G8Pad::KNOB2_OUTPUT,
        [](G8Pad* pad) { return pad->knobs[1]; }
    );
    createRelativePort(
        Vec(9.76, 91.276),
        module,
        G8Pad::KNOB3_OUTPUT,
        [](G8Pad* pad) { return pad->knobs[2]; }
    );
    createRelativePort(
        Vec(20.633, 91.276),
        module,
        G8Pad::KNOB4_OUTPUT,
        [](G8Pad* pad) { return pad->knobs[3]; }
    );
    createRelativePort(
        Vec(9.76, 103.857),
        module,
        G8Pad::KNOB5_OUTPUT,
        [](G8Pad* pad) { return pad->knobs[4]; }
    );
    createRelativePort(
        Vec(20.633, 103.857),
        module,
        G8Pad::KNOB6_OUTPUT,
        [](G8Pad* pad) { return pad->knobs[5]; }
    );
    createRelativePort(
        Vec(9.76, 116.438),
        module,
        G8Pad::KNOB7_OUTPUT,
        [](G8Pad* pad) { return pad->knobs[6]; }
    );
    createRelativePort(
        Vec(20.633, 116.439),
        module,
        G8Pad::KNOBS8_OUTPUT,
        [](G8Pad* pad) { return pad->knobs[7]; }
    );

    addChild(createLightCentered<SmallLight<GreenLight>>(
        mm2px(Vec(5, 8.894)), module, G8Pad::GATE_LIGHT
    ));

    padIdText = createWidget<LedTextDisplay>(mm2px(Vec(21.5, 5)));
    padIdText->text = "--";
    addChild(padIdText);
}

void G8PadWidget::step() {
    if (module) {
        auto* pad = dynamic_cast<G8Pad*>(module);
        int padId = pad->getId();
        if (padId >= 0) {
            padIdText->text = string::f("%02d", padId);
        } else {
            padIdText->text = "--";
        }
    }
    ModuleWidget::step();
}

void G8PadWidget::appendContextMenu(Menu* menu) {
    ModuleWidget::appendContextMenu(menu);
    auto* module = dynamic_cast<G8Pad*>(this->module);
    assert(module);
}