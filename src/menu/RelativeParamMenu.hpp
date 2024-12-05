#pragma once

#include <rack.hpp>
#include "ModularContextMenuItem.hpp"
#include "params/RelativeParam.hpp"

using namespace rack;

struct RelativeParamMenu : ModularContextMenuItem {

    RelativeParam* param;

    RelativeParamMenu(RelativeParam* param);
    void appendContextMenu(Menu* menu) override;
};