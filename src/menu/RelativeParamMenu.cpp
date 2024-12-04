#include <rack.hpp>

#include "MenuSlider.hpp"
#include "RelativeParamMenu.hpp"
#include "StrengthSelector.hpp"
#include "VoltageModeSelector.hpp"
#include "params/RelativeParam.hpp"

using namespace rack;

RelativeParamMenu::RelativeParamMenu(RelativeParam* param) {
    this->param = param;
}

void RelativeParamMenu::appendContextMenu(Menu* menu) {
    menu->addChild(new MenuSlider(this->param->slewLimitQuantity.get()));
    auto voltageSelector =
        new VoltageModeSelector(this->param->voltageModeChoice.get());
    voltageSelector->appendContextMenu(menu);
    auto strengthSelector = new StrengthSelector(this->param->strengthChoice.get());
    strengthSelector->appendContextMenu(menu);
}