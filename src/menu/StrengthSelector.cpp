#include <rack.hpp>

#include "StrengthSelector.hpp"

using namespace rack;

StrengthSelector::StrengthSelector(StrengthChoice* choice) {
    this->choice = choice;
}

void StrengthSelector::appendContextMenu(Menu* menu) {
    auto choices = this->choice->getChoices();

    std::vector<std::string> labels;
    for (auto c : choices) {
        labels.push_back(std::get<0>(c));
    }

    menu->addChild(createIndexSubmenuItem(
        this->choice->getLabel(),
        labels,
        [this]() {
            switch (this->choice->get()) {
                case 1:
                    return 0;
                case 2:
                    return 1;
                case 5:
                    return 2;
                case 10:
                    return 3;
                case 999:
                    return 4;
                default:
                    return 0;
            }
        },
        [this](int mode) {
            switch (mode) {
                case 0:
                    this->choice->set(1);
                    break;
                case 1:
                    this->choice->set(2);
                    break;
                case 2:
                    this->choice->set(5);
                    break;
                case 3:
                    this->choice->set(10);
                    break;
                case 4:
                    this->choice->set(999);
                    break;
                default:
                    this->choice->set(5);
                    break;
            }
        }
    ));
}