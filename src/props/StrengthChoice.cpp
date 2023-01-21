#include <rack.hpp>

#include "StrengthChoice.hpp"

std::vector<std::tuple<std::string, int>> StrengthChoice::getChoices() {
    return {
        {"1 Notch", 1},
        {"2 Notch", 1},
        {"5 Notches", 5},
        {"10 Notches", 10},
        {"Toggle", 999}};
}

StrengthChoice::StrengthChoice(
    std::string label,
    std::function<void(int)> callback
) {
    this->label = label;
    this->callback = callback;
    this->value = 5;
}