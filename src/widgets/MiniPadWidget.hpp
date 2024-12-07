#pragma once

#include "MiniPad.hpp"
#include "ui/LedText.hpp"
#include "ui/SvgHelper.hpp"
#include "widgets/BaseWidget.hpp"

struct MiniPadWidget : BaseWidget<MiniPad, MiniPadWidget> {
    LedTextDisplay* padIdText;

    explicit MiniPadWidget(MiniPad* module);

    void step() override;
    void appendContextMenu(Menu* menu) override;

   private:
    void createStatusLight(MiniPad* module);
    void createGatePort(MiniPad* module);
    void createTouchPort(MiniPad* module);
    void createVelocityPort(MiniPad* module);
    void createBendPort(MiniPad* module);
    void createModPort(MiniPad* module);
    void createKnobPorts(MiniPad* module);
    void createIndexLabel(MiniPad* module);
};