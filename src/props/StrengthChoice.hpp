#pragma once

#include "Choice.hpp"
#include "utils/Strength.hpp"

struct StrengthChoice : Choice<Strength> {
    StrengthChoice(std::string label, std::function<void(Strength)> callback);
    std::vector<std::tuple<std::string, Strength>> getChoices() override;
};