#pragma once

#include <rack.hpp>

#include "ModularContextMenuItem.hpp"
#include "params/AbsoluteParam.hpp"

using namespace rack;

struct AbsoluteParamMenu : ModularContextMenuItem {

    AbsoluteParam* param;

    AbsoluteParamMenu(AbsoluteParam* param);
    void appendContextMenu(Menu* menu) override;
};