#pragma once

#include <rack.hpp>

#include "VoltageMode.hpp"

#include "props/SlewLimitQuantity.hpp"

using namespace rack::dsp;

struct Slew {
    TSlewLimiter<float> slewLimiter;
    float limit;
    float target;
    VoltageMode voltageMode;

    Slew(float limit, VoltageMode voltageMode);
    Slew(float limit);

    VoltageMode getVoltageMode();
    void setVoltageMode(VoltageMode newVoltageMode);

    float getLimit();
    void setLimit(float newLimit);

    float getTarget();
    void setTarget(float newTarget);

    float getSlewed(float sampleTime);

    json_t* toJson();
    void fromJson(json_t* rootJ);
};