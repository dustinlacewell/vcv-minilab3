#include "RelativeParam.hpp"
#include "CallbackQuantity.hpp"

RelativeParam::RelativeParam(Output* output) : BaseParam(output) {
    this->pile = new Pile();
}

RelativeParam::~RelativeParam() {
    delete pile;
}

void RelativeParam::send(int value) {
    auto piled = pile->send(value);
    auto clamped = clamp->clamped(piled);
    pile->setValue(clamped);
    auto normalized = clamp->normalized(clamped);
    auto scaled = rescaler->rescale(normalized);
    if (slew->getLimit() > 0.0f) {
        slew->setTarget(scaled);
        auto slewed = slew->getSlewed(APP->engine->getSampleTime());
        output->setVoltage(slewed);
    } else {
        output->setVoltage(scaled);
    }
}

void RelativeParam::process() {
    if (slew->getLimit() > 0.0f) {
        auto slewed = slew->getSlewed(APP->engine->getSampleTime());
        output->setVoltage(slewed);
    }
}

json_t* RelativeParam::toJson() {
    auto rootJ = BaseParam::toJson();
    json_object_set_new(rootJ, "pile", pile->toJson());
    return rootJ;
}

void RelativeParam::fromJson(json_t* rootJ) {
    BaseParam::fromJson(rootJ);
    json_t* pileJ = json_object_get(rootJ, "pile");
    if (pileJ) {
        pile->fromJson(pileJ);
    }
}