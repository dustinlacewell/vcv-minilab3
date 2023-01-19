#pragma once

#include <rack.hpp>


using namespace rack;

#include <slew/SlewVoltage.hpp>
#include "ModularContextMenuItem.hpp"


struct VoltageTypeSelector : ModularContextMenuItem {

    SlewVoltage *voltage;

    VoltageTypeSelector(SlewVoltage *voltage);
    void appendContextMenu(Menu *menu) override;
};