#pragma once

#include <rack.hpp>


using namespace rack;

// this is a rack::Quantity that takes a callback which is called when the value is set

struct CallbackQuantity : Quantity {
    std::string label;
    float value = 0.5f;
    float min_value = 0.f;
    float max_value = 1.f;

    std::function<void(float)> callback;

    CallbackQuantity(std::string label, float min, float max, std::function<void(float)> callback);

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