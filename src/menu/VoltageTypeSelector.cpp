#include <rack.hpp>

#include "../slew/SlewVoltage.hpp"
#include "ModularContextMenuItem.hpp"
#include "VoltageTypeSelector.hpp"

using namespace rack;

VoltageTypeSelector::VoltageTypeSelector(SlewVoltage* voltage) {
    this->voltage = voltage;
}

void VoltageTypeSelector::appendContextMenu(Menu* menu) {
    menu->addChild(createIndexPtrSubmenuItem(
        "Voltage mode", {"0V:1V", "0V:10V", "-1V:1V", "-10V:10V"},
        &voltage->voltageMode
    ));
}