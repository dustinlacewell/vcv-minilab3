#pragma once

#include <rack.hpp>

using namespace rack::engine;

#include "BaseParam.hpp"
#include "Pile.hpp"
#include "VoltageMode.hpp"

struct RelativeParam : BaseParam {
    Pile* pile;

    RelativeParam(engine::Output* output);
    RelativeParam(engine::Output* output, int min, int max);
    RelativeParam(engine::Output* output, int min, int max, float slew);
    RelativeParam(
        engine::Output* output,
        int min,
        int max,
        float slew,
        VoltageMode voltageMode
    );

    RelativeParam(
        engine::Output* output,
        int min,
        int max,
        float slew,
        VoltageMode voltageMode,
        int strength
    );

    ~RelativeParam();

    void send(int value) override;
    void process() override;

    json_t* toJson() override;
    void fromJson(json_t* rootJ) override;
};