#include <rack.hpp>

#include "../utils/AbsoluteParam.hpp"
#include "AbsoluteParamMenu.hpp"
#include "MenuSlider.hpp"
#include "ModularContextMenuItem.hpp"

using namespace rack;

AbsoluteParamMenu::AbsoluteParamMenu(AbsoluteParam* param) {
    this->param = param;
}

void AbsoluteParamMenu::appendContextMenu(Menu* menu) {
    menu->addChild(new MenuSlider(this->param->slewLimitQuantity));
}