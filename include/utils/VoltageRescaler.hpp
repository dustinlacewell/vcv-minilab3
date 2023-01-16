#pragma once

#include <rack.hpp>

#include <utils/VoltageMode.hpp>
#include <utils/ClampedValue.hpp>

using namespace rack::dsp;

template <typename T>
struct VoltageRescaler {

    VoltageMode voltageMode = VoltageMode::UNIPOLAR_1;
    ClampedValue<T>* clampedValue;

    VoltageRescaler(ClampedValue<T>* clamp);
    VoltageRescaler(ClampedValue<T>* clamp, VoltageMode voltageMode);

    VoltageMode getVoltageMode();
    void setVoltageMode(VoltageMode voltageMode);

    float getVoltage(float sampleTime, int sampleRate);

    json_t* toJson();
    void fromJson(json_t* rootJ, ClampedValue<T>* clamp);    
};