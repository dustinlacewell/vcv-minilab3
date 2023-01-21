#pragma once

#include <rack.hpp>

using namespace rack;

#include "ModularContextMenuItem.hpp"
#include "props/StrengthChoice.hpp"

struct StrengthSelector : ModularContextMenuItem {
    StrengthChoice* choice;

    StrengthSelector(StrengthChoice* choice);
    void appendContextMenu(Menu* menu) override;
};