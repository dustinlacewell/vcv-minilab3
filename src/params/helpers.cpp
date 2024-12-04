#include "helpers.hpp"
#include "BaseModule.hpp"

std::string makeIndexedName(std::string type, int index) {
    return type + " " + std::to_string(index + 1);
}

std::string knobName(int index) {
    return makeIndexedName("Knob", index);
}

std::string sliderName(int index) {
    return makeIndexedName("Slider", index);
}

RelativeParam* makeKnob(int baseIndex, int index, BaseModule* module) {
    auto name = knobName(index);
    auto knobIndex = baseIndex + index;
    return module->createRelativeOutput(knobIndex, name, [index](RelativeParam* p) {
        p->setStrength(2);
        if (index < 4) {
            p->setVoltageMode(VoltageMode::BIPOLAR_5);
            p->setValue(64);
        } else {
            p->setVoltageMode(VoltageMode::UNIPOLAR_10);
            p->setValue(0);
        }
    });
}

AbsoluteParam* makeSlider(int baseIndex, int index, BaseModule* module) {
    auto name = sliderName(index);
    auto sliderIndex = baseIndex + index;
    return module->createAbsoluteOutput(sliderIndex, name, [](AbsoluteParam* p) {
        p->setSlew(0.0f);
        p->setVoltageMode(VoltageMode::UNIPOLAR_10);
        p->setValue(0);
    });
}