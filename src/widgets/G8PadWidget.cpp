#include <rack.hpp>

#include "G8Pad.hpp"
#include "G8PadWidget.hpp"

#include "plugin.hpp"

G8PadWidget::G8PadWidget(G8Pad* module) : BaseWidget<G8Pad>(), SvgHelper<G8PadWidget>() {

    setModule(module);
    loadPanel(asset::plugin(pluginInstance, "res/G8Pad.svg"));

    createAbsolutePort(
        findNamed("Gate").value(),
        module,
        G8Pad::GATE_OUTPUT,
        [](G8Pad* pad) { return pad->gate; },
        false
    );

    createAbsolutePort(
        findNamed("Touch").value(),
        module,
        G8Pad::TOUCH_OUTPUT,
        [](G8Pad* pad) { return pad->touch; },
        false
    );

    createAbsolutePort(
        findNamed("Vel").value(),
        module,
        G8Pad::VELOCITY_OUTPUT,
        [](G8Pad* pad) { return pad->velocity; },
        false
    );

    createAbsolutePort(
        findNamed("Bend").value(),
        module,
        G8Pad::BEND_OUTPUT,
        [](G8Pad* pad) { return pad->bend; },
        false
    );

    createAbsolutePort(
        findNamed("Mod").value(),
        module,
        G8Pad::MOD_OUTPUT,
        [](G8Pad* pad) { return pad->mod; },
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
            G8Pad::KNOB_OUTPUT + i,
            [i](G8Pad* pad) { return pad->knobs[i]; },
            false
        );
    }

    addChild(createLightCentered<SmallLight<GreenLight>>(
        findNamed("Light").value(), 
        module, 
        G8Pad::STATUS_LIGHT
    ));

    auto indexPos = findNamed("Index").value();

    indexPos.x -= 12;
    indexPos.y -= 12;

    padIdText = createWidget<LedTextDisplay>(
        indexPos
    );
    padIdText->text = "--";
    addChild(padIdText);
}

void G8PadWidget::step() {
    if (module) {
        auto* pad = dynamic_cast<G8Pad*>(module);
        int padId = pad->position;
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