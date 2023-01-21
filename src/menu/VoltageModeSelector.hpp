#pragma once

#include <rack.hpp>

using namespace rack;

#include "ModularContextMenuItem.hpp"
#include "props/VoltageModeChoice.hpp"

struct VoltageModeSelector : ModularContextMenuItem {
    VoltageModeChoice* voltageChoice;

    VoltageModeSelector(VoltageModeChoice* voltageChoice);
    void appendContextMenu(Menu* menu) override;
};