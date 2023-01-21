#pragma once

#include <rack.hpp>

using namespace rack::dsp;

template <typename T>
struct Clamp {

    T min;
    T max;

    Clamp(T min, T max);

    T clamped(T value);
    float normalized(T value);

    T getMin();
    T getMax();

    void setMin(T newMin);
    void setMax(T newMax);

    json_t* toJson();
    void fromJson(json_t* rootJ);
};