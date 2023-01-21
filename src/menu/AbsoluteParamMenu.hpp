#pragma once

#include <rack.hpp>

using namespace rack;

#include "ModularContextMenuItem.hpp"
#include "params/AbsoluteParam.hpp"

struct AbsoluteParamMenu : ModularContextMenuItem {

    AbsoluteParam* param;

    AbsoluteParamMenu(AbsoluteParam* param);
    void appendContextMenu(Menu* menu) override;
};