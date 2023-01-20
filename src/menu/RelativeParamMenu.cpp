#include <rack.hpp>

#include "../utils/RelativeParam.hpp"
#include "MenuSlider.hpp"
#include "RelativeParamMenu.hpp"

using namespace rack;

RelativeParamMenu::RelativeParamMenu(RelativeParam* param) {
    this->param = param;
}

void RelativeParamMenu::appendContextMenu(Menu* menu) {
    menu->addChild(new MenuSlider(this->param->slewLimitQuantity));
}