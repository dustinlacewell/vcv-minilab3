#include <rack.hpp>

#include "MiniLab3.hpp"
#include "MiniLab3Widget.hpp"

#include "plugin.hpp"

MiniLab3Widget::MiniLab3Widget(MiniLab3* module) {
    setModule(module);
    setPanel(createPanel(asset::plugin(pluginInstance, "res/MiniLab3.svg")));

    addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0))
    );
    addChild(createWidget<ScrewSilver>(
        Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)
    ));
    addChild(createWidget<ScrewSilver>(Vec(
        box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH
    )));

    createAbsolutePort(
        Vec(9.382, 51.645),
        module,
        MiniLab3::NOTE_OUTPUT,
        [](MiniLab3* lab) { return lab->noteParam; }
    );

    addOutput(createOutputCentered<PJ301MPort>(
        mm2px(Vec(30.48, 51.645)), module, MiniLab3::GATE_OUTPUT
    ));
    addOutput(createOutputCentered<PJ301MPort>(
        mm2px(Vec(51.577, 51.645)), module, MiniLab3::VELOCITY_OUTPUT
    ));
    addOutput(createOutputCentered<PJ301MPort>(
        mm2px(Vec(18.282, 66.756)), module, MiniLab3::BEND_OUTPUT
    ));
    addOutput(createOutputCentered<PJ301MPort>(
        mm2px(Vec(41.677, 66.756)), module, MiniLab3::MOD_OUTPUT
    ));
    addOutput(createOutputCentered<PJ301MPort>(
        mm2px(Vec(6.445, 84.75)), module, MiniLab3::KNOB1_OUTPUT
    ));
    addOutput(createOutputCentered<PJ301MPort>(
        mm2px(Vec(22.119, 84.75)), module, MiniLab3::KNOB2_OUTPUT
    ));
    addOutput(createOutputCentered<PJ301MPort>(
        mm2px(Vec(38.768, 84.75)), module, MiniLab3::KNOB3_OUTPUT
    ));
    addOutput(createOutputCentered<PJ301MPort>(
        mm2px(Vec(56.03, 84.75)), module, MiniLab3::KNOB4_OUTPUT
    ));
    addOutput(createOutputCentered<PJ301MPort>(
        mm2px(Vec(6.445, 97.364)), module, MiniLab3::KNOB5_OUTPUT
    ));
    addOutput(createOutputCentered<PJ301MPort>(
        mm2px(Vec(22.119, 97.364)), module, MiniLab3::KNOB6_OUTPUT
    ));
    addOutput(createOutputCentered<PJ301MPort>(
        mm2px(Vec(38.768, 97.364)), module, MiniLab3::KNOB7_OUTPUT
    ));
    addOutput(createOutputCentered<PJ301MPort>(
        mm2px(Vec(55.428, 97.364)), module, MiniLab3::KNOB8_OUTPUT
    ));
    addOutput(createOutputCentered<PJ301MPort>(
        mm2px(Vec(6.445, 115.842)), module, MiniLab3::SLIDER1_OUTPUT
    ));
    addOutput(createOutputCentered<PJ301MPort>(
        mm2px(Vec(22.119, 115.842)), module, MiniLab3::SLIDER2_OUTPUT
    ));
    addOutput(createOutputCentered<PJ301MPort>(
        mm2px(Vec(38.768, 115.842)), module, MiniLab3::SLIDER3_OUTPUT
    ));
    addOutput(createOutputCentered<PJ301MPort>(
        mm2px(Vec(55.729, 115.842)), module, MiniLab3::SLIDER4_OUTPUT
    ));

    addChild(createLightCentered<SmallLight<GreenLight>>(
        mm2px(Vec(5, 8.894)), module, MiniLab3::GATE_LIGHT
    ));

    auto* display = createWidget<MidiDisplay>(mm2px(Vec(0.0, 13.f)));
    display->box.size = mm2px(Vec(60.960, 29.012));
    display->setMidiPort(module ? &module->midiInput : nullptr);
    addChild(display);
}

void MiniLab3Widget::appendContextMenu(Menu* menu) {
    ModuleWidget::appendContextMenu(menu);
    auto* module = dynamic_cast<MiniLab3*>(this->module);
    assert(module);
}

void MiniLab3Widget::step() {
    ModuleWidget::step();
}