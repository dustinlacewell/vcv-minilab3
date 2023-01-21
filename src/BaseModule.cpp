
#include "BaseModule.hpp"

#include <utility>
#include "params/AbsoluteParam.hpp"
#include "params/RelativeParam.hpp"

AbsoluteParam* BaseModule::createAbsoluteOutput(int output, std::string label) {
    configOutput(output, label);
    auto param = new AbsoluteParam(label, &outputs[output]);
    params.push_back(param);
    return param;
}

AbsoluteParam* BaseModule::createAbsoluteOutput(
    int output,
    std::string label,
    std::function<void(AbsoluteParam*)> setupCallback
) {
    auto param = createAbsoluteOutput(output, std::string(label));
    setupCallback(param);
    param->save();
    return param;
}

RelativeParam* BaseModule::createRelativeOutput(int output, std::string label) {
    configOutput(output, label);
    auto param = new RelativeParam(label, &outputs[output]);
    params.push_back(param);
    return param;
}

RelativeParam* BaseModule::createRelativeOutput(
    int output,
    std::string label,
    std::function<void(RelativeParam*)> setupCallback
) {
    auto param = createRelativeOutput(output, std::string(label));
    setupCallback(param);
    param->save();
    return param;
}

void BaseModule::processParams() {
    for (auto param : params) {
        param->process();
    }
}

void BaseModule::outputsToJson(json_t* rootJ) {
    for (auto param : params) {
        json_object_set_new(rootJ, param->getName().c_str(), param->toJson());
    }
}

void BaseModule::outputsFromJson(json_t* rootJ) {
    for (auto param : params) {
        json_t* paramJ = json_object_get(rootJ, param->getName().c_str());
        if (paramJ) {
            param->fromJson(paramJ);
        }
    }
}

void BaseModule::onReset() {
    for (auto param : params) {
        param->load();
    }
}
