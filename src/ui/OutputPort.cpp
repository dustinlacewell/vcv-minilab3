
#include <rack.hpp>

#include <MiniLab3.hpp>
#include <G8Pad.hpp>
#include <ui/OutputPort.hpp>
#include <slew/SlewVoltage.hpp>

using namespace rack;


OutputPort::OutputPort() {
    setSvg(Svg::load(asset::system("res/ComponentLibrary/PJ301M.svg")));
    voltage = nullptr;
    slewLimitQuantity = nullptr;
}

void OutputPort::appendContextMenu(Menu *menu)
{
    menu->addChild(new MenuSeparator());
    if (voltage) {        
        menu->addChild(createIndexPtrSubmenuItem("Voltage mode", {"0V:1V", "0V:10V", "-1V:1V", "-10V:10V"}, &voltage->voltageMode));    
    }
    if (slewLimitQuantity) {
        menu->addChild(new MenuSlider(slewLimitQuantity));
    }
};