#include <rack.hpp>

#include "MiniPad.hpp"
#include "MiniPadWidget.hpp"

#include "plugin.hpp"

MiniPadWidget::MiniPadWidget(MiniPad* module) : BaseWidget<MiniPad, MiniPadWidget>() {
    setModule(module);
    loadPanel("res/G8Pad.svg");
    createStatusLight(module);
    createIndexLabel(module);
    createGatePort(module);
    createTouchPort(module);
    createVelocityPort(module);
    createBendPort(module);
    createModPort(module);
    createKnobPorts(module);
}

void MiniPadWidget::step() {
    if (module) {
        auto* pad = dynamic_cast<MiniPad*>(module);
        auto _panel = dynamic_cast<SvgPanel*>(getPanel());

        if (!pad) {
            return;
        }

        int padId = pad->position;
        if (padId >= 0) {
            padIdText->text = string::f("%02d", padId);
            // if (_panel && _panel->panelBorder->isVisible()) {
            //     _panel->panelBorder->hide();
            //     _panel->fb->setDirty();
            // }
        } else {
            padIdText->text = "--";
            // if (_panel && !_panel->panelBorder->isVisible()) {
            //     _panel->panelBorder->show();
            //     _panel->fb->setDirty();
            // }
        }

    }

    updateBorders({modelMiniLab, modelMiniPad}, {modelMiniPad});
    ModuleWidget::step();
}

void MiniPadWidget::appendContextMenu(Menu* menu) {
    ModuleWidget::appendContextMenu(menu);
    auto* module = dynamic_cast<MiniPad*>(this->module);
    assert(module);
}

void MiniPadWidget::createStatusLight(MiniPad* module) {
    addChild(createLightCentered<SmallLight<GreenLight>>(
        findNamed("Light").value(), module, MiniPad::STATUS_LIGHT
    ));
}

void MiniPadWidget::createGatePort(MiniPad* module) {
    createAbsolutePort("Gate", module, MiniPad::GATE_OUTPUT, [](MiniPad* pad) {
        return pad->gate;
    });
}

void MiniPadWidget::createTouchPort(MiniPad* module) {
    createAbsolutePort("Touch", module, MiniPad::TOUCH_OUTPUT, [](MiniPad* pad) {
        return pad->touch;
    });
}

void MiniPadWidget::createVelocityPort(MiniPad* module) {
    createAbsolutePort("Vel", module, MiniPad::VELOCITY_OUTPUT, [](MiniPad* pad) {
        return pad->velocity;
    });
}

void MiniPadWidget::createBendPort(MiniPad* module) {
    createAbsolutePort("Bend", module, MiniPad::BEND_OUTPUT, [](MiniPad* pad) {
        return pad->bend;
    });
}

void MiniPadWidget::createModPort(MiniPad* module) {
    createAbsolutePort("Mod", module, MiniPad::MOD_OUTPUT, [](MiniPad* pad) {
        return pad->mod;
    });
}

void MiniPadWidget::createKnobPorts(MiniPad* module) {
    for (int i = 0; i < 8; i++) {
        createRelativePort(
            "Knob" + std::to_string(i + 1),
            module,
            MiniPad::KNOB_OUTPUT + i,
            [i](MiniPad* pad) { return pad->knobs[i]; }
        );
    }
}

void MiniPadWidget::createIndexLabel(MiniPad* module) {
    auto indexPos = findNamed("Index").value();
    indexPos.x -= 12;
    indexPos.y -= 12;

    padIdText = createWidget<LedTextDisplay>(indexPos);
    padIdText->text = "--";
    addChild(padIdText);
}