#pragma once

#include <rack.hpp>
#include "params/AbsoluteParam.hpp"
#include "params/RelativeParam.hpp"

struct BaseModule : rack::Module {
    // vector of BaseParams
    std::vector<BaseParam*> params;

    void process(const ProcessArgs& args) override;
    AbsoluteParam* createAbsoluteOutput(int output, std::string label);
    RelativeParam* createRelativeOutput(int output, std::string label);
};