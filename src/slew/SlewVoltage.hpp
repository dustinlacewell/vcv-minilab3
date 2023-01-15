#pragma once

#include "../plugin.hpp"

#include "SlewLimitQuantity.hpp"
#include "VoltageMode.hpp"

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

SlewVoltage::SlewVoltage(SlewLimitQuantity* slewLimitQuantity, VoltageMode voltageMode, float min, float max) {
    this->slewLimitQuantity = slewLimitQuantity;
    this->voltageMode = voltageMode;
    this->min = min;
    this->max = max;
}

SlewVoltage::SlewVoltage(SlewLimitQuantity* slewLimitQuantity, VoltageMode voltageMode) {
    this->slewLimitQuantity = slewLimitQuantity;
    this->voltageMode = voltageMode;
    this->min = 0.f;
    this->max = 127.f;
}

SlewVoltage::SlewVoltage(SlewLimitQuantity* slewLimitQuantity) {
    this->slewLimitQuantity = slewLimitQuantity;
    this->voltageMode = UNIPOLAR_1;
    this->min = 0.f;
    this->max = 127.f;
}

void SlewVoltage::setVoltageMode(VoltageMode voltageMode) {
    this->voltageMode = voltageMode;
}

VoltageMode SlewVoltage::getVoltageMode() {
    return voltageMode;
}

void SlewVoltage::setTarget(float target) {
    this->target = target;
}

float SlewVoltage::getTarget() {
    return target;
}

float SlewVoltage::getVoltage(float sampleTime, int sampleRate) {
    float limitValue = 1.f / slewLimitQuantity->getValue(); // 0 - 1
    slewLimiter.setRiseFall(max * limitValue, max * limitValue);
    float value = slewLimiter.process(sampleTime, target);

    switch (voltageMode) {
        case UNIPOLAR_1:
            return rescale(value, min, max, 0.f, 1.f);
        case UNIPOLAR_10:
            return rescale(value, min, max, 0.f, 10.f);
        case BIPOLAR_1:
            return rescale(value, min, max, -1.f, 1.f);
        case BIPOLAR_10:
            return rescale(value, min, max, -10.f, 10.f);
    }
}

json_t* SlewVoltage::toJson() {
    json_t* rootJ = json_object();
    json_object_set_new(rootJ, "target", json_real(target));
    json_object_set_new(rootJ, "min", json_real(min));
    json_object_set_new(rootJ, "max", json_real(max));
    json_object_set_new(rootJ, "voltageMode", json_integer(voltageMode));
    json_object_set_new(rootJ, "slewLimiterOut", json_real(slewLimiter.out));
}

void SlewVoltage::fromJson(json_t* rootJ, SlewLimitQuantity* slewLimitQuantity) {
    json_t* targetJ = json_object_get(rootJ, "target");
    if (targetJ)
        setTarget(json_number_value(targetJ));
    json_t* minJ = json_object_get(rootJ, "min");
    if (minJ)
        min = json_number_value(minJ);
    json_t* maxJ = json_object_get(rootJ, "max");
    if (maxJ)
        max = json_number_value(maxJ);
    json_t* voltageModeJ = json_object_get(rootJ, "voltageMode");
    if (voltageModeJ)
        voltageMode = (VoltageMode)json_integer_value(voltageModeJ);
    json_t* slewLimiterOutJ = json_object_get(rootJ, "slewLimiterOut");
    if (slewLimiterOutJ)
        slewLimiter.out = json_number_value(slewLimiterOutJ);
    
    this->slewLimitQuantity = slewLimitQuantity;
} 