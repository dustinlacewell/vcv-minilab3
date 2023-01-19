#pragma once

#include <rack.hpp>

#include <MiniLab3.hpp>
#include <menu/ModularContextMenuItem.hpp>
#include <slew/SlewVoltage.hpp>


using namespace rack;


struct OutputPort : SvgPort {
    // vector of ModularContextMenuItems
    std::vector<ModularContextMenuItem *> contextMenus;

    OutputPort();
    void appendContextMenu(Menu *menu) override;
};