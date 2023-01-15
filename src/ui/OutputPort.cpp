
#include <rack.hpp>

#include <MiniLab3.hpp>
#include <ui/OutputPort.hpp>

using namespace rack;


OutputPort::OutputPort() {
    setSvg(Svg::load(asset::system("res/ComponentLibrary/CL1362.svg")));
}
    

void OutputPort::appendContextMenu(Menu *menu)
{
    menu->addChild(new MenuSeparator());
    menu->addChild(new MenuSeparator());
    // menu->addChild(createCheckMenuItem(
    //     "0V:1V", "",
    //     [=]() {
    //         return module->mod->getVoltageMode() == VoltageMode::UNIPOLAR_1;
    //     },
    //     [=]() {
    //         return module->mod->setVoltageMode(VoltageMode::UNIPOLAR_1);
    //     }));
    // menu->addChild(createCheckMenuItem(
    //     "0V:10V", "",
    //     [=]() {
    //         return module->mod->getVoltageMode() == VoltageMode::UNIPOLAR_10;
    //     },
    //     [=]() {
    //         return module->mod->setVoltageMode(VoltageMode::UNIPOLAR_10);
    //     }));
    // menu->addChild(createCheckMenuItem(
    //     "-1V:1V", "",
    //     [=]() {
    //         return module->mod->getVoltageMode() == VoltageMode::BIPOLAR_1;
    //     },
    //     [=]() {
    //         return module->mod->setVoltageMode(VoltageMode::BIPOLAR_1);
    //     }));
    // menu->addChild(createCheckMenuItem(
    //     "-10V:10V", "",
    //     [=]() {
    //         return module->mod->getVoltageMode() == VoltageMode::BIPOLAR_10;
    //     },
    //     [=]() {
    //         return module->mod->setVoltageMode(VoltageMode::BIPOLAR_10);
    //     }));
};