#pragma once

#include "SlewLimitQuantity.hpp"

SlewLimitQuantity::SlewLimitQuantity(std::string label) {
    this->label = label;
}

SlewLimitQuantity::SlewLimitQuantity(std::string label, float min, float max) {
    this->label = label;
    setRange(min, max);
}

SlewLimitQuantity::SlewLimitQuantity() {
    this->label = "Slew Limit";
}

void SlewLimitQuantity::setValue(float newValue) {
    value = clamp(newValue, min_value, max_value);
}

float SlewLimitQuantity::getValue() {
    return value;
}

float SlewLimitQuantity::getDefaultValue() {
    return 0.5f;
}

std::string SlewLimitQuantity::getLabel() {
    return label;
}

int SlewLimitQuantity::getDisplayPrecision() {
    return 2;
}

float SlewLimitQuantity::getMaxValue() {
    return max_value;
}

float SlewLimitQuantity::getMinValue() {
    return min_value;
}

void SlewLimitQuantity::setRange(float min, float max) {
    min_value = min;
    max_value = max;
}

json_t* SlewLimitQuantity::toJson() {
    json_t* rootJ = json_object();
    json_object_set_new(rootJ, "value", json_real(getValue()));
    json_object_set_new(rootJ, "min", json_real(getMinValue()));
    json_object_set_new(rootJ, "max", json_real(getMaxValue()));
    json_object_set_new(rootJ, "label", json_string(getLabel().c_str()));
    return rootJ;
}

void SlewLimitQuantity::fromJson(json_t* rootJ) {
    json_t* valueJ = json_object_get(rootJ, "value");
    if (valueJ) {
        setValue(json_number_value(valueJ));
    }
    json_t* minJ = json_object_get(rootJ, "min");
    json_t* maxJ = json_object_get(rootJ, "max");
    if (minJ && maxJ) {
        setRange(json_number_value(minJ), json_number_value(maxJ));
    }
    json_t* labelJ = json_object_get(rootJ, "label");
    if (labelJ) {
        label = json_string_value(labelJ);
    }
}