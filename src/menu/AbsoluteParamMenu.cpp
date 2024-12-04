#include <rack.hpp>

#include "AbsoluteParamMenu.hpp"
#include "MenuSlider.hpp"
#include "VoltageModeSelector.hpp"
#include "params/AbsoluteParam.hpp"

using namespace rack;

AbsoluteParamMenu::AbsoluteParamMenu(AbsoluteParam* param) {
    this->param = param;
}

void AbsoluteParamMenu::appendContextMenu(Menu* menu) {
    menu->addChild(new MenuSlider(this->param->slewLimitQuantity.get()));
    auto voltageSelector =
        new VoltageModeSelector(this->param->voltageModeChoice.get());
    voltageSelector->appendContextMenu(menu);
}