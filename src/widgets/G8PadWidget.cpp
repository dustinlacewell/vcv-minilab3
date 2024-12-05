#include <rack.hpp>

#include "G8Pad.hpp"
#include "G8PadWidget.hpp"

#include "plugin.hpp"

G8PadWidget::G8PadWidget(G8Pad* module) : BaseWidget<G8Pad, G8PadWidget>() {
    setModule(module);
    loadPanel(asset::plugin(pluginInstance, "res/G8Pad.svg"));
    createStatusLight(module);
    createIndexLabel(module);
    createGatePort(module);
    createTouchPort(module);
    createVelocityPort(module);
    createBendPort(module);
    createModPort(module);
    createKnobPorts(module);
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

void G8PadWidget::createStatusLight(G8Pad* module) {
    addChild(createLightCentered<SmallLight<GreenLight>>(
        findNamed("Light").value(), module, G8Pad::STATUS_LIGHT
    ));
}

void G8PadWidget::createGatePort(G8Pad* module) {
    createAbsolutePort("Gate", module, G8Pad::GATE_OUTPUT, [](G8Pad* pad) {
        return pad->gate;
    });
}

void G8PadWidget::createTouchPort(G8Pad* module) {
    createAbsolutePort("Touch", module, G8Pad::TOUCH_OUTPUT, [](G8Pad* pad) {
        return pad->touch;
    });
}

void G8PadWidget::createVelocityPort(G8Pad* module) {
    createAbsolutePort("Vel", module, G8Pad::VELOCITY_OUTPUT, [](G8Pad* pad) {
        return pad->velocity;
    });
}

void G8PadWidget::createBendPort(G8Pad* module) {
    createAbsolutePort("Bend", module, G8Pad::BEND_OUTPUT, [](G8Pad* pad) {
        return pad->bend;
    });
}

void G8PadWidget::createModPort(G8Pad* module) {
    createAbsolutePort("Mod", module, G8Pad::MOD_OUTPUT, [](G8Pad* pad) {
        return pad->mod;
    });
}

void G8PadWidget::createKnobPorts(G8Pad* module) {
    for (int i = 0; i < 8; i++) {
        createRelativePort(
            "Knob" + std::to_string(i + 1),
            module,
            G8Pad::KNOB_OUTPUT + i,
            [i](G8Pad* pad) { return pad->knobs[i]; }
        );
    }
}

void G8PadWidget::createIndexLabel(G8Pad* module) {
    auto indexPos = findNamed("Index").value();
    indexPos.x -= 12;
    indexPos.y -= 12;

    padIdText = createWidget<LedTextDisplay>(indexPos);
    padIdText->text = "--";
    addChild(padIdText);
}