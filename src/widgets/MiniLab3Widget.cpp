#include <rack.hpp>

#include "MiniLab3.hpp"
#include "MiniLab3Widget.hpp"

#include <G8Pad.hpp>
#include <MiniLog.hpp>
#include "plugin.hpp"

MiniLab3Widget::MiniLab3Widget(MiniLab3* module)
    : BaseWidget<MiniLab3, MiniLab3Widget>() {
    setModule(module);
    loadPanel(asset::plugin(pluginInstance, "res/Lab.svg"));
    createMidiSelector(module);
    createStatusLight(module);
    createGatePort(module);
    createBendPort(module);
    createModPort(module);
    createKnobPorts(module);
}

void MiniLab3Widget::createGatePort(MiniLab3* module) {
    auto gatePosMaybe = findNamed("Gate");

    if (!gatePosMaybe.has_value()) {
        DEBUG("No gate position found");
        return;
    }

    auto gatePos = gatePosMaybe.value();

    createAbsolutePort(
        gatePos,
        module,
        MiniLab3::GATE_OUTPUT,
        [](MiniLab3* lab) { return lab->gate; },
        false
    );
}

void MiniLab3Widget::createBendPort(MiniLab3* module) {
    auto bendPosMaybe = findNamed("Bend");

    if (!bendPosMaybe.has_value()) {
        DEBUG("No bend position found");
        return;
    }

    auto bendPos = bendPosMaybe.value();

    createAbsolutePort(
        bendPos,
        module,
        MiniLab3::BEND_OUTPUT,
        [](MiniLab3* lab) { return lab->bend; },
        false
    );
}

void MiniLab3Widget::createModPort(MiniLab3* module) {
    auto modPosMaybe = findNamed("Mod");

    if (!modPosMaybe.has_value()) {
        DEBUG("No mod position found");
        return;
    }

    auto modPos = modPosMaybe.value();

    createAbsolutePort(
        modPos,
        module,
        MiniLab3::MOD_OUTPUT,
        [](MiniLab3* lab) { return lab->mod; },
        false
    );
}

void MiniLab3Widget::createKnobPorts(MiniLab3* module) {
    for (int i = 0; i < 8; i++) {
        auto knobPosMaybe = findNamed("Knob" + std::to_string(i + 1));

        if (!knobPosMaybe.has_value()) {
            DEBUG("No knob %d position found", i + 1);
            continue;
        }

        auto knobPos = knobPosMaybe.value();

        createRelativePort(
            knobPos,
            module,
            MiniLab3::KNOB_OUTPUT + i,
            [i](MiniLab3* lab) { return lab->knobs[i]; },
            false
        );
    }
}

void MiniLab3Widget::createSliderPorts(MiniLab3* module) {
    for (int i = 0; i < 4; i++) {
        auto sliderPosMaybe = findNamed("Slider" + std::to_string(i + 1));

        if (!sliderPosMaybe.has_value()) {
            DEBUG("No slider %d position found", i + 1);
            continue;
        }

        auto sliderPos = sliderPosMaybe.value();

        createAbsolutePort(
            sliderPos,
            module,
            MiniLab3::SLIDER_OUTPUT + i,
            [i](MiniLab3* lab) { return lab->sliders[i]; },
            false
        );
    }
}

void MiniLab3Widget::createStatusLight(MiniLab3* module) {
    addChild(createLightCentered<SmallLight<GreenLight>>(
        findNamed("Light").value(), module, MiniLab3::STATUS_LIGHT
    ));
}

void MiniLab3Widget::createMidiSelector(MiniLab3* module) {
    auto* display = createWidget<MidiDisplay>(mm2px(Vec(0.0, 11.f)));
    display->box.size = mm2px(Vec(5.08 * 9, 29.012));
    display->setMidiPort(module ? &module->midiInput : nullptr);
    addChild(display);
}

void MiniLab3Widget::appendContextMenu(Menu* menu) {
    ModuleWidget::appendContextMenu(menu);
    auto* module = dynamic_cast<MiniLab3*>(this->module);
    assert(module);
}

void MiniLab3Widget::step() {
    if (module) {
        auto* lab = dynamic_cast<MiniLab3*>(module);

        if (!lab) {
            return;
        }

        auto _panel = dynamic_cast<SvgPanel*>(getPanel());
        auto rightExpander = lab->rightExpander.module;
        if (rightExpander && _panel->panelBorder->isVisible()) {
            _panel->panelBorder->hide();
            _panel->fb->setDirty();
        } else if (!rightExpander && !_panel->panelBorder->isVisible()) {
            _panel->panelBorder->show();
            _panel->fb->setDirty();
        }
    }

    ModuleWidget::step();
}