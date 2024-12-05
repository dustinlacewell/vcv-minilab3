
#include "VoltageModeChoice.hpp"
#include "utils/VoltageMode.hpp"

std::vector<std::tuple<std::string, VoltageMode>> VoltageModeChoice::getChoices(
) {
    return {
        {"0V:1V", VoltageMode::UNIPOLAR_1},
        {"0V:5V", VoltageMode::UNIPOLAR_5},
        {"0V:10V", VoltageMode::UNIPOLAR_10},
        {"-1V:1V", VoltageMode::BIPOLAR_1},
        {"-5V:5V", VoltageMode::BIPOLAR_5},
        {"-10V:10V", VoltageMode::BIPOLAR_10}
    };
}

VoltageModeChoice::VoltageModeChoice(
    std::string label,
    std::function<void(VoltageMode)> callback
) {
    this->label = label;
    this->callback = callback;
    this->value = VoltageMode::UNIPOLAR_1;
}