#pragma once

#include <rack.hpp>

#include <utils/VoltageMode.hpp>

#include <slew/SlewLimitQuantity.hpp>


using namespace rack::dsp;


struct Slew {
    TSlewLimiter<float> slewLimiter;
    float limit;
    float target;
    VoltageMode voltageMode;

    Slew(float limit, VoltageMode voltageMode);
    Slew(float limit);

    VoltageMode getVoltageMode();
    void setVoltageMode(VoltageMode voltageMode);

    float getLimit();
    void setLimit(float limit);

    float getTarget();
    void setTarget(float target);

    float getSlewed(float sampleTime);

    json_t *toJson();
    void fromJson(json_t *rootJ);
};