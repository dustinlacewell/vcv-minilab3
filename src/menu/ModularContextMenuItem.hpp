#pragma once

#include <rack.hpp>

using namespace rack;

struct ModularContextMenuItem {
    virtual void appendContextMenu(Menu* menu);
};