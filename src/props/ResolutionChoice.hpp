#pragma once

#include "../utils/Resolution.hpp"
#include "Choice.hpp"

struct ResolutionChoice : Choice<Resolution> {
    ResolutionChoice(
        std::string label,
        std::function<void(Resolution)> callback
    );
    std::vector<std::tuple<std::string, Resolution>> getChoices() override;
};