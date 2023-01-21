#pragma once

#include "Choice.hpp"

struct StrengthChoice : Choice<int> {
    StrengthChoice(std::string label, std::function<void(int)> callback);
    std::vector<std::tuple<std::string, int>> getChoices() override;
};