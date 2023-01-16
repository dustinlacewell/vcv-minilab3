#pragma once

#include <rack.hpp>

#include <MiniLab3.hpp>
#include <slew/SlewVoltage.hpp>

using namespace rack;

struct OutputPort : SvgPort
{
    SlewVoltage* voltage;
    SlewLimitQuantity* slewLimitQuantity;

    OutputPort();
    void appendContextMenu(Menu *menu) override;
};