#pragma once

#include <rack.hpp>

using namespace rack::engine;

#include "BaseParam.hpp"
#include "props/StrengthChoice.hpp"
#include "utils/Pile.hpp"
#include "utils/VoltageMode.hpp"

struct RelativeParam : BaseParam {

    StrengthChoice* strengthChoice;

    RelativeParam(engine::Output* output);

    void send(int value) override;

    int getStrength();
    void setStrength(int value);

    json_t* toJson() override;
    void fromJson(json_t* rootJ) override;
};