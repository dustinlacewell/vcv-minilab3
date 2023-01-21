#include <rack.hpp>

#include "VoltageModeSelector.hpp"

using namespace rack;

VoltageModeSelector::VoltageModeSelector(VoltageModeChoice* voltageChoice) {
    this->voltageChoice = voltageChoice;
}

void VoltageModeSelector::appendContextMenu(Menu* menu) {
    menu->addChild(createIndexSubmenuItem(
        "Mode",
        {"0V:1V", "0V:5V", "0V:10V", "-1V:1V", "-5V:5V", "-10V:10V"},
        [this]() {
            VoltageMode mode = this->voltageChoice->get();
            switch (mode) {
                case VoltageMode::UNIPOLAR_1:
                    return 0;
                case VoltageMode::UNIPOLAR_5:
                    return 1;
                case VoltageMode::UNIPOLAR_10:
                    return 2;
                case VoltageMode::BIPOLAR_1:
                    return 3;
                case VoltageMode::BIPOLAR_5:
                    return 4;
                case VoltageMode::BIPOLAR_10:
                    return 5;
                default:
                    return 0;
            }
        },
        [this](int mode) {
            switch (mode) {
                case 0:
                    this->voltageChoice->set(VoltageMode::UNIPOLAR_1);
                    break;
                case 1:
                    this->voltageChoice->set(VoltageMode::UNIPOLAR_5);
                    break;
                case 2:
                    this->voltageChoice->set(VoltageMode::UNIPOLAR_10);
                    break;
                case 3:
                    this->voltageChoice->set(VoltageMode::BIPOLAR_1);
                    break;
                case 4:
                    this->voltageChoice->set(VoltageMode::BIPOLAR_5);
                    break;
                case 5:
                    this->voltageChoice->set(VoltageMode::BIPOLAR_10);
                    break;
            }
        }
    ));
}