#include <rack.hpp>

#include <MiniLab3.hpp>
#include <G8Pad.hpp>
#include <ui/OutputPort.hpp>
#include <slew/SlewVoltage.hpp>


using namespace rack;

OutputPort::OutputPort() {
    setSvg(Svg::load(asset::system("res/ComponentLibrary/PJ301M.svg")));
    this->contextMenus = std::vector<ModularContextMenuItem *>();
}

void OutputPort::appendContextMenu(Menu *menu) {
    menu->addChild(new MenuSeparator());
    for (auto contextMenu : this->contextMenus) {
        contextMenu->appendContextMenu(menu);
    }
};