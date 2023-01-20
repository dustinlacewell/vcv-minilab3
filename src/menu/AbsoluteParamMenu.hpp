#pragma once

#include <rack.hpp>

using namespace rack;

#include "../slew/SlewVoltage.hpp"
#include "../utils/AbsoluteParam.hpp"
#include "ModularContextMenuItem.hpp"

struct AbsoluteParamMenu : ModularContextMenuItem {

    AbsoluteParam* param;

    AbsoluteParamMenu(AbsoluteParam* param);
    void appendContextMenu(Menu* menu) override;
};