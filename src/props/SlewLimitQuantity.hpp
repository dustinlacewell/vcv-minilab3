#pragma once

#include <rack.hpp>


using namespace rack;


struct SlewLimitQuantity : Quantity {
    std::string label;
    float value = 0.5f;
    float min_value = 0.f;
    float max_value = 1.f;

    SlewLimitQuantity();
    SlewLimitQuantity(std::string label);
    SlewLimitQuantity(std::string label, float min, float max);

    std::string getLabel() override;

    float getValue() override;
    void setValue(float newValue) override;
    float getDefaultValue() override;
    int getDisplayPrecision() override;

    float getMaxValue() override;
    float getMinValue() override;
    void setRange(float min, float max);

    json_t *toJson();
    void fromJson(json_t *rootJ);
};