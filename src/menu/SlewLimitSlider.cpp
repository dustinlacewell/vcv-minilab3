#include <rack.hpp>

#include <slew/SlewLimitQuantity.hpp>
#include <menu/MenuSlider.hpp>
#include <menu/ModularContextMenuItem.hpp>
#include <menu/SlewLimitSlider.hpp>


using namespace rack;

SlewLimitSlider::SlewLimitSlider(SlewLimitQuantity *quantity) {
    this->quantity = quantity;
}

void SlewLimitSlider::appendContextMenu(Menu *menu) {
    menu->addChild(new MenuSlider(this->quantity));
}