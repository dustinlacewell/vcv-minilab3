#include <rack.hpp>

#include "../G8Pad.hpp"
#include "../MiniLab3.hpp"
#include "../slew/SlewVoltage.hpp"
#include "OutputPort.hpp"

using namespace rack;

OutputPort::OutputPort() {
    setSvg(Svg::load(asset::system("res/ComponentLibrary/PJ301M.svg")));
    this->contextMenus = std::vector<ModularContextMenuItem*>();
}

void OutputPort::appendContextMenu(Menu* menu) {
    menu->addChild(new MenuSeparator());
    for (auto contextMenu : this->contextMenus) {
        contextMenu->appendContextMenu(menu);
    }
};