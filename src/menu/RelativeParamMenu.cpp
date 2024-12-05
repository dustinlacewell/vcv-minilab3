#include <rack.hpp>

#include "MenuSlider.hpp"
#include "RelativeParamMenu.hpp"
#include "ResolutionSelector.hpp"
#include "StrengthSelector.hpp"
#include "VoltageModeSelector.hpp"
#include "params/RelativeParam.hpp"

using namespace rack;

RelativeParamMenu::RelativeParamMenu(RelativeParam* param) {
    this->param = param;
}

void RelativeParamMenu::appendContextMenu(Menu* menu) {
    auto slew = new MenuSlider(this->param->slewLimitQuantity.get());
    menu->addChild(slew);
    auto voltageSelector =
        new VoltageModeSelector(this->param->voltageModeChoice.get());
    voltageSelector->appendContextMenu(menu);
    auto resolutionSelector =
        new ResolutionSelector(this->param->resolutionChoice.get());
    resolutionSelector->appendContextMenu(menu);
    auto strengthSelector =
        new StrengthSelector(this->param->strengthChoice.get());
    strengthSelector->appendContextMenu(menu);
}