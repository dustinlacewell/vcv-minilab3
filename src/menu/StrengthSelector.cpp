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
        "Turn Speed",
        labels,
        [this]() {
            Strength strength = this->choice->get();
            switch (strength) {
                case Strength::OneTick:
                    return 0;
                case Strength::TwoTicks:
                    return 1;
                case Strength::TenTicks:
                    return 2;
                case Strength::OnePercent:
                    return 3;
                case Strength::TwoPercent:
                    return 4;
                case Strength::FivePercent:
                    return 5;
                case Strength::TenPercent:
                    return 6;
                case Strength::TwentyPercent:
                    return 7;
                case Strength::FiftyPercent:
                    return 8;
                case Strength::Toggle:
                    return 9;
                default:
                    return 5;
            }
        },
        [this](int mode) {
            switch (mode) {
                case 0:
                    this->choice->set(Strength::OneTick);
                    break;
                case 1:
                    this->choice->set(Strength::TwoTicks);
                    break;
                case 2:
                    this->choice->set(Strength::TenTicks);
                    break;
                case 3:
                    this->choice->set(Strength::OnePercent);
                    break;
                case 4:
                    this->choice->set(Strength::TwoPercent);
                    break;
                case 5:
                    this->choice->set(Strength::FivePercent);
                    break;
                case 6:
                    this->choice->set(Strength::TenPercent);
                    break;
                case 7:
                    this->choice->set(Strength::TwentyPercent);
                    break;
                case 8:
                    this->choice->set(Strength::FiftyPercent);
                    break;
                case 9:
                    this->choice->set(Strength::Toggle);
                    break;
            }
        }
    ));
}