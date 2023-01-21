#pragma once

#include <rack.hpp>

using namespace rack;

#include "ModularContextMenuItem.hpp"
#include "params/RelativeParam.hpp"

struct RelativeParamMenu : ModularContextMenuItem {

    RelativeParam* param;

    RelativeParamMenu(RelativeParam* param);
    void appendContextMenu(Menu* menu) override;
};