#pragma once

#include <rack.hpp>

using namespace rack::dsp;

template <typename T>
struct ClampedValue {

    T value;
    T min;
    T max;

    ClampedValue(T value, T min, T max);
    T getValue();
    void setValue(T value);
    T getMin();
    T getMax();
    void setRange(T min, T max);
    void setMin(T min);
    void setMax(T max);

    json_t* toJson();
    void fromJson(json_t* rootJ);    
};