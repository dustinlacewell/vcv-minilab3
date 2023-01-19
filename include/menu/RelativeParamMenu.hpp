#pragma once

#include <rack.hpp>

using namespace rack;

#include <slew/SlewVoltage.hpp>
#include <utils/RelativeParam.hpp>
#include "ModularContextMenuItem.hpp"

struct RelativeParamMenu : ModularContextMenuItem {

    RelativeParam* param;

    RelativeParamMenu(RelativeParam* param);
    void appendContextMenu(Menu* menu) override;
};