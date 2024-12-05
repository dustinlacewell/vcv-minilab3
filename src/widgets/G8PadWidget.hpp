#pragma once

#include "G8Pad.hpp"
#include "ui/LedText.hpp"
#include "ui/SvgHelper.hpp"
#include "widgets/BaseWidget.hpp"

struct G8PadWidget : BaseWidget<G8Pad, G8PadWidget> {
    LedTextDisplay* padIdText;

    explicit G8PadWidget(G8Pad* module);

    void step() override;
    void appendContextMenu(Menu* menu) override;

   private:
    void createStatusLight(G8Pad* module);
    void createGatePort(G8Pad* module);
    void createTouchPort(G8Pad* module);
    void createVelocityPort(G8Pad* module);
    void createBendPort(G8Pad* module);
    void createModPort(G8Pad* module);
    void createKnobPorts(G8Pad* module);
    void createIndexLabel(G8Pad* module);
};