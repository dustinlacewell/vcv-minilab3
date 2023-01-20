#pragma once

#include "utils/Choice.hpp"
#include "utils/VoltageMode.hpp"

struct VoltageModeChoice : Choice<VoltageMode> {
    VoltageModeChoice(
        std::string label,
        std::function<void(VoltageMode)> callback
    );
    std::vector<std::tuple<std::string, VoltageMode>> getChoices() override;
};