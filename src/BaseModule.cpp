

#include "BaseModule.hpp"

#include <utility>
#include "params/AbsoluteParam.hpp"
#include "params/RelativeParam.hpp"

AbsoluteParam* BaseModule::createAbsoluteOutput(int output, std::string label) {
    configOutput(output, std::move(label));
    auto param = new AbsoluteParam(&outputs[output]);
    params.push_back(param);
    return param;
}

RelativeParam* BaseModule::createRelativeOutput(int output, std::string label) {
    configOutput(output, std::move(label));
    auto param = new RelativeParam(&outputs[output]);
    params.push_back(param);
    return param;
}

void BaseModule::process(const ProcessArgs& args) {
    for (auto param : params) {
        param->process();
    }
}
