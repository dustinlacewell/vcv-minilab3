#pragma once

#include <rack.hpp>
#include "ModularContextMenuItem.hpp"
#include "props/VoltageModeChoice.hpp"

using namespace rack;

struct VoltageModeSelector : ModularContextMenuItem {
    VoltageModeChoice* voltageChoice;

    VoltageModeSelector(VoltageModeChoice* voltageChoice);
    void appendContextMenu(Menu* menu) override;
};