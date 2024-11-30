#pragma once

#include <rack.hpp>

#include "./Clamp.hpp"
#include "./VoltageMode.hpp"

using namespace rack::dsp;

struct VoltageRescaler {

    VoltageMode voltageMode = VoltageMode::UNIPOLAR_1;

    VoltageRescaler();
    VoltageRescaler(VoltageMode voltageMode);

    VoltageMode getVoltageMode();
    void setVoltageMode(VoltageMode newVoltageMode);

    float rescale(float normalizedVoltage);

    json_t* toJson();
    void fromJson(json_t* rootJ);
};