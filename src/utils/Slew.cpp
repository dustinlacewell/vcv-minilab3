#include <rack.hpp>

#include "Slew.hpp"
#include "VoltageMode.hpp"

using namespace rack::dsp;

Slew::Slew(float limit) {
    this->limit = limit;
    this->voltageMode = VoltageMode::UNIPOLAR_1;
}

Slew::Slew(float limit, VoltageMode voltageMode) {
    this->limit = limit;
    this->voltageMode = voltageMode;
}

VoltageMode Slew::getVoltageMode() {
    return voltageMode;
}

void Slew::setVoltageMode(VoltageMode newVoltageMode) {
    voltageMode = newVoltageMode;
}

float Slew::getLimit() {
    return limit;
}

void Slew::setLimit(float newLimit) {
    limit = newLimit;
}

float Slew::getTarget() {
    return target;
}

void Slew::setTarget(float newTarget) {
    target = newTarget;
}

float Slew::getSlewed(float sampleTime) {
    if (this->limit <= 0.00001f) {
        return this->target;
    }

    float limitValue = 1 / this->limit;

    switch (this->voltageMode) {
        case VoltageMode::UNIPOLAR_1:
        case VoltageMode::BIPOLAR_1:
            break;
        case VoltageMode::UNIPOLAR_5:
        case VoltageMode::BIPOLAR_5:
            limitValue *= 5.f;
            break;
        case VoltageMode::UNIPOLAR_10:
        case VoltageMode::BIPOLAR_10:
            limitValue *= 10.f;
            break;
    }

    slewLimiter.setRiseFall(limitValue, limitValue);
    return slewLimiter.process(sampleTime, this->target);
}

json_t* Slew::toJson() {
    json_t* rootJ = json_object();
    json_object_set_new(rootJ, "limit", json_real(limit));
    json_object_set_new(rootJ, "target", json_real(target));
    json_object_set_new(rootJ, "voltageMode", json_integer(voltageMode));
    json_object_set_new(rootJ, "slewLimiterOut", json_real(slewLimiter.out));
    return rootJ;
}

void Slew::fromJson(json_t* rootJ) {
    json_t* limitJ = json_object_get(rootJ, "limit");
    if (limitJ) {
        setLimit(json_number_value(limitJ));
    }
    json_t* targetJ = json_object_get(rootJ, "target");
    if (targetJ) {
        setTarget(json_number_value(targetJ));
    }
    json_t* voltageModeJ = json_object_get(rootJ, "voltageMode");
    if (voltageModeJ) {
        voltageMode = (VoltageMode)json_integer_value(voltageModeJ);
    }
    json_t* slewLimiterOutJ = json_object_get(rootJ, "slewLimiterOut");
    if (slewLimiterOutJ) {
        slewLimiter.out = json_number_value(slewLimiterOutJ);
    }
}