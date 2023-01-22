#include <rack.hpp>

#include "MiniLab3.hpp"
#include "MiniLab3Widget.hpp"

#include "plugin.hpp"

MiniLab3Widget::MiniLab3Widget(MiniLab3* module) {
    setModule(module);
    setPanel(createPanel(asset::plugin(pluginInstance, "res/Lab.svg")));

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
        Vec(22.179, 49.891),
        module,
        MiniLab3::GATE_OUTPUT,
        [](MiniLab3* lab) { return lab->gate; }
    );

    createAbsolutePort(
        Vec(6.149, 49.891),
        module,
        MiniLab3::BEND_OUTPUT,
        [](MiniLab3* lab) { return lab->bend; }
    );

    createAbsolutePort(
        Vec(38.771, 49.891),
        module,
        MiniLab3::MOD_OUTPUT,
        [](MiniLab3* lab) { return lab->mod; }
    );

    std::vector<Vec> knobPositions = {
        Vec(6.149, 102.393),
        Vec(17.023, 102.393),
        Vec(27.897, 102.393),
        Vec(38.771, 102.393),
        Vec(6.149, 113.266),
        Vec(17.023, 113.266),
        Vec(27.897, 113.266),
        Vec(38.771, 113.266),
    };

    for (int i = 0; i < knobPositions.size(); i++) {
        createRelativePort(
            knobPositions[i],
            module,
            MiniLab3::KNOB1_OUTPUT + i,
            [&i](MiniLab3* lab) { return lab->knobs[i]; }
        );
    }

    std::vector<Vec> sliderPositions = {
        Vec(6.149, 78.392),
        Vec(17.023, 78.392),
        Vec(27.897, 78.392),
        Vec(38.771, 78.392),
    };

    for (int i = 0; i < sliderPositions.size(); i++) {
        createAbsolutePort(
            sliderPositions[i],
            module,
            MiniLab3::SLIDER1_OUTPUT + i,
            [&i](MiniLab3* lab) { return lab->sliders[i]; }
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