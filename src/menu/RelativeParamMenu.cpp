#include <rack.hpp>

#include <menu/MenuSlider.hpp>
#include <menu/RelativeParamMenu.hpp>
#include <utils/RelativeParam.hpp>

using namespace rack;

RelativeParamMenu::RelativeParamMenu(RelativeParam* param) {
    this->param = param;
}

void RelativeParamMenu::appendContextMenu(Menu* menu) {
    menu->addChild(new MenuSlider(this->param->slewLimitQuantity));
}