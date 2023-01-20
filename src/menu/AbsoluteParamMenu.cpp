#include <rack.hpp>

#include "../utils/AbsoluteParam.hpp"
#include "AbsoluteParamMenu.hpp"
#include "MenuSlider.hpp"
#include "VoltageModeSelector.hpp"

using namespace rack;

AbsoluteParamMenu::AbsoluteParamMenu(AbsoluteParam* param) {
    this->param = param;
}

void AbsoluteParamMenu::appendContextMenu(Menu* menu) {
    menu->addChild(new MenuSlider(this->param->slewLimitQuantity));
    auto voltageSelector =
        new VoltageModeSelector(this->param->voltageModeChoice);
    voltageSelector->appendContextMenu(menu);
}