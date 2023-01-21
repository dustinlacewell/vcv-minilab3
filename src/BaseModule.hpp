#pragma once

#include <rack.hpp>
#include "params/AbsoluteParam.hpp"
#include "params/RelativeParam.hpp"

struct BaseModule : rack::Module {
    // vector of BaseParams
    std::vector<BaseParam*> params;

    void processParams();
    AbsoluteParam* createAbsoluteOutput(int output, std::string label);
    AbsoluteParam* createAbsoluteOutput(
        int output,
        std::string label,
        std::function<void(AbsoluteParam*)> setupCallback
    );
    RelativeParam* createRelativeOutput(int output, std::string label);
    RelativeParam* createRelativeOutput(
        int output,
        std::string label,
        std::function<void(RelativeParam*)> setupCallback
    );

    void outputsToJson(json_t* rootJ);
    void outputsFromJson(json_t* rootJ);
    void onReset() override;
};