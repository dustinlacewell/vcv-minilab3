#include <rack.hpp>

#include <utils/AbsoluteParam.hpp>
#include <menu/MenuSlider.hpp>
#include <menu/ModularContextMenuItem.hpp>
#include <menu/AbsoluteParamMenu.hpp>


using namespace rack;

AbsoluteParamMenu::AbsoluteParamMenu(AbsoluteParam *param) {
    this->param = param;
}

void AbsoluteParamMenu::appendContextMenu(Menu *menu) {
    menu->addChild(new MenuSlider(this->param->slewLimitQuantity));
}