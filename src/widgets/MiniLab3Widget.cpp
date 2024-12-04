#include <rack.hpp>

#include "MiniLab3.hpp"
#include "MiniLab3Widget.hpp"

#include "plugin.hpp"

static const float MY_SVG_DPI = 75.f;
static const float MY_MM_PER_IN = 25.4f;

inline math::Vec mmm2px(math::Vec mm) {
	auto result = mm.mult(MY_SVG_DPI / MY_MM_PER_IN);
    return result;
}

inline math::Vec px2mm(math::Vec px) {
    return px.mult(MY_MM_PER_IN / MY_SVG_DPI);
}

MiniLab3Widget::MiniLab3Widget(MiniLab3* module) 
    : BaseWidget<MiniLab3>()
    , SvgHelper<MiniLab3Widget>() 
{
    setModule(module);
    loadPanel(asset::plugin(pluginInstance, "res/Lab.svg"));

    // MIDI SELECTOR
    auto* display = createWidget<MidiDisplay>(mm2px(Vec(0.0, 11.f)));
    display->box.size = mm2px(Vec(5.08 * 9, 29.012));
    display->setMidiPort(module ? &module->midiInput : nullptr);
    addChild(display);

    // ACTIVITY LIGHT
    addChild(createLightCentered<SmallLight<GreenLight>>(
        findNamed("Light").value(), module, MiniLab3::STATUS_LIGHT
    ));

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

    auto bendPos = findNamed("Bend").value();
    createAbsolutePort(
        bendPos,
        module,
        MiniLab3::BEND_OUTPUT,
        [](MiniLab3* lab) { return lab->bend; },
        false
    );

    auto modPos = findNamed("Mod").value();
    createAbsolutePort(
        modPos,
        module,
        MiniLab3::MOD_OUTPUT,
        [](MiniLab3* lab) { return lab->mod; },
        false
    );

    std::vector<Vec> knobPositions = {
        findNamed("Knob1").value(),
        findNamed("Knob2").value(),
        findNamed("Knob3").value(),
        findNamed("Knob4").value(),
        findNamed("Knob5").value(),
        findNamed("Knob6").value(),
        findNamed("Knob7").value(),
        findNamed("Knob8").value(),
    };

    for (int i = 0; i < knobPositions.size(); i++) {
        createRelativePort(
            knobPositions[i],
            module,
            MiniLab3::KNOB_OUTPUT + i,
            [i](MiniLab3* lab) { return lab->knobs[i]; },
            false
        );
    }

    std::vector<Vec> sliderPositions = {
        findNamed("Fader1").value(),
        findNamed("Fader2").value(),
        findNamed("Fader3").value(),
        findNamed("Fader4").value(),
    };

    for (int i = 0; i < sliderPositions.size(); i++) {
        createAbsolutePort(
            sliderPositions[i],
            module,
            MiniLab3::SLIDER_OUTPUT + i,
            [i](MiniLab3* lab) -> AbsoluteParam* { return lab->sliders[i]; },
            false
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