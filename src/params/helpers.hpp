#pragma once

#include <rack.hpp>
#include "AbsoluteParam.hpp"
#include "BaseModule.hpp"
#include "RelativeParam.hpp"

std::string makeIndexedName(std::string type, int index);

std::string knobName(int index);

std::string sliderName(int index);

RelativeParam* makeKnob(int baseIndex, int index, BaseModule* module);

AbsoluteParam* makeSlider(int baseIndex, int index, BaseModule* module);