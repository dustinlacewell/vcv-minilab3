#pragma once

#include <rack.hpp>


using namespace rack::engine;

#include "CallbackQuantity.hpp"
#include "Clamp.hpp"
#include "Pile.hpp"
#include "Slew.hpp"
#include "VoltageMode.hpp"
#include "VoltageRescaler.hpp"


struct RelativeParam {
    engine::Output *output;
    Pile *pile;
    Clamp<int> *clamp;
    VoltageRescaler *rescaler;
    Slew *slew;

    CallbackQuantity *slewLimitQuantity;

    RelativeParam(engine::Output *output);
    RelativeParam(engine::Output *output, float slew);
    RelativeParam(engine::Output *output, float slew, int strength);
    RelativeParam(engine::Output *output, float slew, int strength, int min, int max);
    ~RelativeParam();

    void send(int value);
    void process();

    json_t *toJson();
    void fromJson(json_t *rootJ);
};