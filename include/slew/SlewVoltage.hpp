#pragma once

#include <rack.hpp>

#include <utils/VoltageMode.hpp>

#include "SlewLimitQuantity.hpp"

using namespace rack::dsp;

struct SlewVoltage {

    // create enum for voltage modes

	TSlewLimiter<float> slewLimiter;
    SlewLimitQuantity* slewLimitQuantity;
    float min;
    float max;
    float target;
    VoltageMode voltageMode;

    SlewVoltage(SlewLimitQuantity* slewLimitQuantity, VoltageMode voltageMode, float min, float max);

    SlewVoltage(SlewLimitQuantity* slewLimitQuantity, VoltageMode voltageMode);

    SlewVoltage(SlewLimitQuantity* slewLimitQuantity);

    void setVoltageMode(VoltageMode voltageMode);

    VoltageMode getVoltageMode();

    void setTarget(float target);

    float getTarget();

    float getVoltage(float sampleTime, int sampleRate);

    json_t* toJson();
    void fromJson(json_t* rootJ, SlewLimitQuantity* slewLimitQuantity);    
};