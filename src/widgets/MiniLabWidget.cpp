#include <rack.hpp>

#include "MiniLab.hpp"
#include "MiniLabWidget.hpp"

#include "MiniLog.hpp"
#include "MiniPad.hpp"
#include "plugin.hpp"

MiniLabWidget::MiniLabWidget(MiniLab* module)
    : BaseWidget<MiniLab, MiniLabWidget>() {
    setModule(module);
    loadPanel("res/Lab.svg");

    createMidiSelector(module);
    createStatusLight(module);
    createGatePort(module);
    createBendPort(module);
    createModPort(module);
    createKnobPorts(module);
    createSliderPorts(module);
}

void MiniLabWidget::createGatePort(MiniLab* module) {
    auto gatePosMaybe = findNamed("Gate");

    if (!gatePosMaybe.has_value()) {
        DEBUG("No gate position found");
        return;
    }

    auto gatePos = *gatePosMaybe;

    createAbsolutePort(
        gatePos,
        module,
        MiniLab::GATE_OUTPUT,
        [](MiniLab* lab) { return lab->gate; }
    );
}

void MiniLabWidget::createBendPort(MiniLab* module) {
    auto bendPosMaybe = findNamed("Bend");

    if (!bendPosMaybe.has_value()) {
        DEBUG("No bend position found");
        return;
    }

    auto bendPos = *bendPosMaybe;

    createAbsolutePort(
        bendPos,
        module,
        MiniLab::BEND_OUTPUT,
        [](MiniLab* lab) { return lab->bend; }
    );
}

void MiniLabWidget::createModPort(MiniLab* module) {
    auto modPosMaybe = findNamed("Mod");

    if (!modPosMaybe.has_value()) {
        DEBUG("No mod position found");
        return;
    }

    auto modPos = *modPosMaybe;

    createAbsolutePort(
        modPos,
        module,
        MiniLab::MOD_OUTPUT,
        [](MiniLab* lab) { return lab->mod; }
    );
}

void MiniLabWidget::createKnobPorts(MiniLab* module) {
    for (int i = 0; i < 8; i++) {
        auto knobPosMaybe = findNamed("Knob" + std::to_string(i + 1));

        if (!knobPosMaybe.has_value()) {
            DEBUG("No knob %d position found", i + 1);
            continue;
        }

        auto knobPos = *knobPosMaybe;

        createRelativePort(
            knobPos,
            module,
            MiniLab::KNOB_OUTPUT + i,
            [i](MiniLab* lab) { return lab->knobs[i]; }
        );
    }
}

void MiniLabWidget::createSliderPorts(MiniLab* module) {
    for (int i = 0; i < 4; i++) {
        auto sliderPosMaybe = findNamed("Fader" + std::to_string(i + 1));

        if (!sliderPosMaybe.has_value()) {
            DEBUG("No slider %d position found", i + 1);
            continue;
        }

        auto sliderPos = *sliderPosMaybe;

        createAbsolutePort(
            sliderPos,
            module,
            MiniLab::SLIDER_OUTPUT + i,
            [i](MiniLab* lab) { return lab->sliders[i]; }
        );
    }
}

void MiniLabWidget::createStatusLight(MiniLab* module) {
    addChild(createLightCentered<SmallLight<GreenLight>>(
        *findNamed("Light"), module, MiniLab::STATUS_LIGHT
    ));
}

void MiniLabWidget::createMidiSelector(MiniLab* module) {
    auto* display = createWidget<MidiDisplay>(Vec(2, 32));
    display->box.size = Vec(131, 90);
    display->setMidiPort(module ? &module->midiInput : nullptr);
    addChild(display);
}

void MiniLabWidget::appendContextMenu(Menu* menu) {
    ModuleWidget::appendContextMenu(menu);
    auto* module = dynamic_cast<MiniLab*>(this->module);
    assert(module);
}

void MiniLabWidget::step() {
    updateBorders({modelMiniLog}, {modelMiniPad});
    ModuleWidget::step();
}