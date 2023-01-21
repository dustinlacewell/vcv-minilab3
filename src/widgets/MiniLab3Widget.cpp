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

    auto* display = createWidget<MidiDisplay>(mm2px(Vec(0.0, 13.f)));
    display->box.size = mm2px(Vec(60.960, 29.012));
    display->setMidiPort(module ? &module->midiInput : nullptr);
    addChild(display);

    addChild(createLightCentered<SmallLight<GreenLight>>(
        mm2px(Vec(5, 8.894)), module, MiniLab3::GATE_LIGHT
    ));

    createAbsolutePort(
        Vec(30.48, 51.645),
        module,
        MiniLab3::GATE_OUTPUT,
        [](MiniLab3* lab) { return lab->gateParam; }
    );

    createAbsolutePort(
        Vec(51.577, 51.645),
        module,
        MiniLab3::VELOCITY_OUTPUT,
        [](MiniLab3* lab) { return lab->velocityParam; }
    );

    createAbsolutePort(
        Vec(9.382, 51.645),
        module,
        MiniLab3::NOTE_OUTPUT,
        [](MiniLab3* lab) { return lab->noteParam; }
    );

    createAbsolutePort(
        Vec(18.282, 66.756),
        module,
        MiniLab3::BEND_OUTPUT,
        [](MiniLab3* lab) { return lab->bendParam; }
    );

    createAbsolutePort(
        Vec(41.677, 66.756),
        module,
        MiniLab3::MOD_OUTPUT,
        [](MiniLab3* lab) { return lab->modParam; }
    );

    std::vector<Vec> knobPositions = {
        Vec(6.445, 84.75),
        Vec(22.119, 84.75),
        Vec(38.768, 84.75),
        Vec(56.03, 84.75),
        Vec(6.445, 97.364),
        Vec(22.119, 97.364),
        Vec(38.768, 97.364),
        Vec(55.428, 97.364)};

    for (int i = 0; i < knobPositions.size(); i++) {
        createRelativePort(
            knobPositions[i],
            module,
            MiniLab3::KNOB1_OUTPUT + i,
            [&i](MiniLab3* lab) { return lab->knobParams[i]; }
        );
    }

    std::vector<Vec> sliderPositions = {
        Vec(6.445, 115.842),
        Vec(22.119, 115.842),
        Vec(38.768, 115.842),
        Vec(55.729, 115.842),
    };

    for (int i = 0; i < sliderPositions.size(); i++) {
        createAbsolutePort(
            sliderPositions[i],
            module,
            MiniLab3::SLIDER1_OUTPUT + i,
            [&i](MiniLab3* lab) { return lab->sliderParams[i]; }
        );
    }
}

void MiniLab3Widget::appendContextMenu(Menu* menu) {
    ModuleWidget::appendContextMenu(menu);
    auto* module = dynamic_cast<MiniLab3*>(this->module);
    assert(module);
}

void MiniLab3Widget::step() {
    ModuleWidget::step();
}