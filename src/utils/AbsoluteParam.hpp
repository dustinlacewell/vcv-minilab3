#pragma once

#include "CallbackQuantity.hpp"
#include "Clamp.hpp"
#include "Slew.hpp"
#include "VoltageMode.hpp"
#include "VoltageRescaler.hpp"


struct AbsoluteParam {
    engine::Output *output;
    Clamp<int> *clamp;
    VoltageRescaler *rescaler;
    Slew *slew;

    CallbackQuantity *slewLimitQuantity;

    AbsoluteParam(engine::Output *output);
    AbsoluteParam(engine::Output *output, int min, int max);
    AbsoluteParam(engine::Output *output, int min, int max, float slew);
    ~AbsoluteParam();

    void send(int value);
    void process();

    json_t *toJson();
    void fromJson(json_t *rootJ);
};