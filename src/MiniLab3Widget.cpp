#include <rack.hpp>

#include "MiniLab3.hpp"
#include "MiniLab3Widget.hpp"

#include "plugin.hpp"

template struct BaseWidget<MiniLab3>;

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

    addOutput(createOutputCentered<PJ301MPort>(
        mm2px(Vec(9.382, 51.645)), module, MiniLab3::NOTE_OUTPUT
    ));
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

    // add menu slider for controlling bend slew
    auto* module = dynamic_cast<MiniLab3*>(this->module);
    assert(module);
    //
    //    menu->addChild(new MenuSeparator());
    //    menu->addChild(createSubmenuItem("Bend", "", [=](Menu* menu) {
    //        menu->addChild(new MenuSlider(module->bendLimit));
    //        menu->addChild(createIndexPtrSubmenuItem(
    //            "Voltage mode",
    //            {"0V:1V", "0V:10V", "-1V:1V", "-10V:10V"},
    //            &module->bend->voltageMode
    //        ));
    //    }));
    //
    //    menu->addChild(createSubmenuItem("Mod", "", [=](Menu* menu) {
    //        menu->addChild(new MenuSlider(module->modLimit));
    //        menu->addChild(createIndexPtrSubmenuItem(
    //            "Voltage mode",
    //            {"0V:1V", "0V:10V", "-1V:1V", "-10V:10V"},
    //            &module->mod->voltageMode
    //        ));
    //    }));
    //
    //    menu->addChild(createSubmenuItem("Sliders", "", [=](Menu* menu) {
    //        menu->addChild(new MenuSlider(module->sliderLimit));
    //        menu->addChild(createSubmenuItem("Voltage mode", "", [=](Menu* menu) {
    //            menu->addChild(createCheckMenuItem(
    //                "0V:1V",
    //                "",
    //                [=]() {
    //                    return module->sliderVoltageMode == VoltageMode::UNIPOLAR_1;
    //                },
    //                [=]() {
    //                    for (int i = 0; i < 4; i++) {
    //                        module->sliders[i]->voltageMode =
    //                            VoltageMode::UNIPOLAR_1;
    //                    }
    //                }
    //            ));
    //            menu->addChild(createCheckMenuItem(
    //                "0V:10V",
    //                "",
    //                [=]() {
    //                    return module->sliderVoltageMode ==
    //                           VoltageMode::UNIPOLAR_10;
    //                },
    //                [=]() {
    //                    for (int i = 0; i < 4; i++) {
    //                        module->sliders[i]->voltageMode =
    //                            VoltageMode::UNIPOLAR_10;
    //                    }
    //                }
    //            ));
    //            menu->addChild(createCheckMenuItem(
    //                "-1V:1V",
    //                "",
    //                [=]() {
    //                    return module->sliderVoltageMode == VoltageMode::BIPOLAR_1;
    //                },
    //                [=]() {
    //                    for (int i = 0; i < 4; i++) {
    //                        module->sliders[i]->voltageMode =
    //                            VoltageMode::BIPOLAR_1;
    //                    }
    //                }
    //            ));
    //            menu->addChild(createCheckMenuItem(
    //                "-10V:10V",
    //                "",
    //                [=]() {
    //                    return module->sliderVoltageMode == VoltageMode::BIPOLAR_10;
    //                },
    //                [=]() {
    //                    for (int i = 0; i < 4; i++) {
    //                        module->sliders[i]->voltageMode =
    //                            VoltageMode::BIPOLAR_10;
    //                    }
    //                }
    //            ));
    //        }));
    //    }));
    //
    //    menu->addChild(createSubmenuItem("Knobs", "", [=](Menu* menu) {
    //        menu->addChild(new MenuSlider(module->knobLimit));
    //        menu->addChild(createSubmenuItem("Strength", "", [=](Menu* menu) {
    //            menu->addChild(createCheckMenuItem(
    //                "1",
    //                "",
    //                [=]() { return module->knobStrength == 1; },
    //                [=]() { module->knobStrength = 1; }
    //            ));
    //            menu->addChild(createCheckMenuItem(
    //                "2",
    //                "",
    //                [=]() { return module->knobStrength == 2; },
    //                [=]() { module->knobStrength = 2; }
    //            ));
    //            menu->addChild(createCheckMenuItem(
    //                "4",
    //                "",
    //                [=]() { return module->knobStrength == 4; },
    //                [=]() { module->knobStrength = 4; }
    //            ));
    //            menu->addChild(createCheckMenuItem(
    //                "8",
    //                "",
    //                [=]() { return module->knobStrength == 8; },
    //                [=]() { module->knobStrength = 8; }
    //            ));
    //        }));
    //        menu->addChild(createSubmenuItem("Voltage mode", "", [=](Menu* menu) {
    //            menu->addChild(createCheckMenuItem(
    //                "0V:1V",
    //                "",
    //                [=]() {
    //                    return module->knobVoltageMode == VoltageMode::UNIPOLAR_1;
    //                },
    //                [=]() {
    //                    for (int i = 0; i < 8; i++) {
    //                        module->knobs[i]->voltageMode = VoltageMode::UNIPOLAR_1;
    //                    }
    //                }
    //            ));
    //            menu->addChild(createCheckMenuItem(
    //                "0V:10V",
    //                "",
    //                [=]() {
    //                    return module->knobVoltageMode == VoltageMode::UNIPOLAR_10;
    //                },
    //                [=]() {
    //                    for (int i = 0; i < 8; i++) {
    //                        module->knobs[i]->voltageMode =
    //                            VoltageMode::UNIPOLAR_10;
    //                    }
    //                }
    //            ));
    //            menu->addChild(createCheckMenuItem(
    //                "-1V:1V",
    //                "",
    //                [=]() {
    //                    return module->knobVoltageMode == VoltageMode::BIPOLAR_1;
    //                },
    //                [=]() {
    //                    for (int i = 0; i < 8; i++) {
    //                        module->knobs[i]->voltageMode = VoltageMode::BIPOLAR_1;
    //                    }
    //                }
    //            ));
    //            menu->addChild(createCheckMenuItem(
    //                "-10V:10V",
    //                "",
    //                [=]() {
    //                    return module->knobVoltageMode == VoltageMode::BIPOLAR_10;
    //                },
    //                [=]() {
    //                    for (int i = 0; i < 8; i++) {
    //                        module->knobs[i]->voltageMode = VoltageMode::BIPOLAR_10;
    //                    }
    //                }
    //            ));
    //        }));
    //    }));
}