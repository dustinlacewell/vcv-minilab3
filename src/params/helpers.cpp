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
    auto knob = module->createRelativeOutput(knobIndex, name, [=](auto p) {
        p->setStrength(2);
        p->setSlew(0.5f);

        if (index < 4) {
            p->setVoltageMode(VoltageMode::BIPOLAR_5);
            p->setValue(64);
        } else {
            p->setVoltageMode(VoltageMode::UNIPOLAR_10);
            p->setValue(0);
        }
    });

    return knob;
}

AbsoluteParam* makeSlider(int baseIndex, int index, BaseModule* module) {
    auto name = sliderName(index);
    auto sliderIndex = baseIndex + index;
    auto slider = module->createAbsoluteOutput(sliderIndex, name, [=](auto p) {
        p->setSlew(0.0f);
        p->setVoltageMode(VoltageMode::UNIPOLAR_10);
        p->setValue(0);
    });
    return slider;
}