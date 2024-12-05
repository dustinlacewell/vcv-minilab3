
#include "ResolutionChoice.hpp"

std::vector<std::tuple<std::string, Resolution>> ResolutionChoice::getChoices(
) {
    return {
        {"Two", Resolution::Two},
        {"Three", Resolution::Three},
        {"Four", Resolution::Four},
        {"Five", Resolution::Five},
        {"Eight", Resolution::Eight},
        {"Twelve", Resolution::Twelve},
        {"Low", Resolution::Low},
        {"Medium", Resolution::Medium},
        {"High", Resolution::High}
    };
}

ResolutionChoice::ResolutionChoice(
    std::string label,
    std::function<void(Resolution)> callback
) {
    this->label = label;
    this->callback = callback;
    this->value = Resolution::Medium;
}