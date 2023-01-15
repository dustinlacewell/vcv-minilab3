#pragma once

#include <rack.hpp>

#include "MiniLab3.hpp"

using namespace rack;

struct OutputPort : SvgPort
{
    OutputPort();
    void appendContextMenu(Menu *menu) override;
};