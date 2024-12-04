#pragma once

#include <rack.hpp>
#include "BaseParam.hpp"
#include "props/StrengthChoice.hpp"
#include "utils/Pile.hpp"
#include "utils/VoltageMode.hpp"

using namespace rack::engine;

struct RelativeParam : BaseParam {
    StrengthChoice* strengthChoice;

    RelativeParam(std::string name, engine::Output* output);
    ~RelativeParam() override;

    void send(int value) override;
    int getStrength();
    void setStrength(int value);
    json_t* toJson() override;
    void fromJson(json_t* rootJ) override;
};