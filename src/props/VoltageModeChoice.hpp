#pragma once

#include "../utils/VoltageMode.hpp"
#include "Choice.hpp"

struct VoltageModeChoice : Choice<VoltageMode> {
    VoltageModeChoice(
        std::string label,
        std::function<void(VoltageMode)> callback
    );
    std::vector<std::tuple<std::string, VoltageMode>> getChoices() override;
};