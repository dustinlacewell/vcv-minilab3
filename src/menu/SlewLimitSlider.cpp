#include <rack.hpp>

#include "../slew/SlewLimitQuantity.hpp"
#include "MenuSlider.hpp"
#include "ModularContextMenuItem.hpp"
#include "SlewLimitSlider.hpp"

using namespace rack;

SlewLimitSlider::SlewLimitSlider(SlewLimitQuantity* quantity) {
    this->quantity = quantity;
}

void SlewLimitSlider::appendContextMenu(Menu* menu) {
    menu->addChild(new MenuSlider(this->quantity));
}