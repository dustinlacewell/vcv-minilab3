#include <rack.hpp>

#include "ResolutionSelector.hpp"

using namespace rack;

ResolutionSelector::ResolutionSelector(ResolutionChoice* resolutionChoice) {
    this->resolutionChoice = resolutionChoice;
}

void ResolutionSelector::appendContextMenu(Menu* menu) {
    menu->addChild(createIndexSubmenuItem(
        "Resolution",
        {"Two",
         "Three",
         "Four",
         "Five",
         "Eight",
         "Twelve",
         "Low",
         "Medium",
         "High"},
        [this]() {
            Resolution mode = this->resolutionChoice->get();
            switch (mode) {
                case Resolution::Two:
                    return 0;
                case Resolution::Three:
                    return 1;
                case Resolution::Four:
                    return 2;
                case Resolution::Five:
                    return 3;
                case Resolution::Eight:
                    return 4;
                case Resolution::Twelve:
                    return 5;
                case Resolution::Low:
                    return 6;
                case Resolution::Medium:
                    return 7;
                case Resolution::High:
                    return 8;
                default:
                    return 7;
            }
        },
        [this](int mode) {
            switch (mode) {
                case 0:
                    this->resolutionChoice->set(Resolution::Two);
                    break;
                case 1:
                    this->resolutionChoice->set(Resolution::Three);
                    break;
                case 2:
                    this->resolutionChoice->set(Resolution::Four);
                    break;
                case 3:
                    this->resolutionChoice->set(Resolution::Five);
                    break;
                case 4:
                    this->resolutionChoice->set(Resolution::Eight);
                    break;
                case 5:
                    this->resolutionChoice->set(Resolution::Twelve);
                    break;
                case 6:
                    this->resolutionChoice->set(Resolution::Low);
                    break;
                case 7:
                    this->resolutionChoice->set(Resolution::Medium);
                    break;
                case 8:
                    this->resolutionChoice->set(Resolution::High);
                    break;
            }
        }
    ));
}