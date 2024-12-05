#pragma once

#include <rack.hpp>

using namespace rack;

#include "ModularContextMenuItem.hpp"
#include "props/ResolutionChoice.hpp"

struct ResolutionSelector : ModularContextMenuItem {
    ResolutionChoice* resolutionChoice;

    ResolutionSelector(ResolutionChoice* resolutionChoice);
    void appendContextMenu(Menu* menu) override;
};