#pragma once

#include <rack.hpp>

#include "MiniLog.hpp"
#include "ui/SvgHelper.hpp"
#include "ui/TextLogWidget.hpp"
#include "widgets/BaseWidget.hpp"

struct MiniLogWidget : BaseWidget<MiniLog, MiniLogWidget> {
    TextLogWidget* log;
    bool dirty = true;

    explicit MiniLogWidget(MiniLog* module);

    void step() override;

   private:
    void createStatusLight(MiniLog* module);
    void createLogWidget(MiniLog* module);
};