#include <rack.hpp>

#include "StrengthChoice.hpp"
#include "utils/Strength.hpp"

std::vector<std::tuple<std::string, Strength>> StrengthChoice::getChoices() {
    return {
        {"1", Strength::OneTick},
        {"2", Strength::TwoTicks},
        {"10", Strength::TenTicks},
        {"1%", Strength::OnePercent},
        {"2%", Strength::TwoPercent},
        {"5%", Strength::FivePercent},
        {"10%", Strength::TenPercent},
        {"25%", Strength::TwentyPercent},
        {"50%", Strength::FiftyPercent},
        {"Toggle", Strength::Toggle}
    };
}

StrengthChoice::StrengthChoice(
    std::string label,
    std::function<void(Strength)> callback
) {
    this->label = label;
    this->callback = callback;
    this->value = Strength::FivePercent;
}