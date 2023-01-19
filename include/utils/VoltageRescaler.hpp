#pragma once

#include <rack.hpp>

#include <utils/VoltageMode.hpp>
#include <utils/Clamp.hpp>


using namespace rack::dsp;


struct VoltageRescaler {

    VoltageMode voltageMode = VoltageMode::UNIPOLAR_1;

    VoltageRescaler();
    VoltageRescaler(VoltageMode voltageMode);

    VoltageMode getVoltageMode();
    void setVoltageMode(VoltageMode voltageMode);

    float rescale(float normalizedVoltage);

    json_t *toJson();
    void fromJson(json_t *rootJ);
};