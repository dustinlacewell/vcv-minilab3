#pragma once

#include <rack.hpp>

using namespace rack;

#include "../slew/SlewVoltage.hpp"
#include "ModularContextMenuItem.hpp"

struct SlewLimitSlider : ModularContextMenuItem {

    SlewLimitQuantity* quantity;

    SlewLimitSlider(SlewLimitQuantity* quantity);
    void appendContextMenu(Menu* menu) override;
};