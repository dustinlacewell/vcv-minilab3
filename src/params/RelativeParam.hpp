#pragma once

#include <rack.hpp>
#include "BaseParam.hpp"
#include "props/ResolutionChoice.hpp"
#include "props/StrengthChoice.hpp"
#include "utils/Pile.hpp"

using namespace rack::engine;

struct RelativeParam : BaseParam {
    std::unique_ptr<StrengthChoice> strengthChoice;
    std::unique_ptr<ResolutionChoice> resolutionChoice;

    RelativeParam(std::string name, engine::Output* output);

    void send(int value) override;
    int getStrength();
    void setStrength(Strength strength);
    void setResolution(Resolution resolution);
    json_t* toJson() override;
    void fromJson(json_t* rootJ) override;

   private:
    void recalc(Strength strength, Resolution resolution);
};