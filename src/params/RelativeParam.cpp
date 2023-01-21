#include "RelativeParam.hpp"
#include "props/CallbackQuantity.hpp"

RelativeParam::RelativeParam(std::string name, Output* output)
    : BaseParam(name, output) {
    this->strengthChoice =
        new StrengthChoice("Relative Strength", [this](int choice) {
            this->pile->setStrength(choice);
        });

    this->resetData = RelativeParam::toJson();
}

void RelativeParam::send(int value) {
    auto piled = pile->send(value);
    auto clamped = clamp->clamped(piled);
    pile->setValue(clamped);
    auto normalized = clamp->normalized(clamped);
    if (slew->getLimit() > 0.0f) {
        slew->setTarget(normalized);
        auto slewed = slew->getSlewed(APP->engine->getSampleTime());
        auto scaled = rescaler->rescale(slewed);
        output->setVoltage(scaled);
        sendCallbacks(normalized);
    } else {
        slew->setTarget(normalized);
        slew->slewLimiter.out = normalized;
        auto scaled = rescaler->rescale(normalized);
        output->setVoltage(scaled);
        sendCallbacks(normalized);
    }
}

int RelativeParam::getStrength() {
    return strengthChoice->get();
}

void RelativeParam::setStrength(int strength) {
    this->strengthChoice->set(strength);
}

json_t* RelativeParam::toJson() {
    auto rootJ = BaseParam::toJson();
    json_object_set_new(rootJ, "strength", strengthChoice->toJson());
    return rootJ;
}

void RelativeParam::fromJson(json_t* rootJ) {
    BaseParam::fromJson(rootJ);
    json_t* strengthJ = json_object_get(rootJ, "strength");
    if (strengthJ) {
        strengthChoice->fromJson(strengthJ);
    }
}