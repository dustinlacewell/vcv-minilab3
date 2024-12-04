#include <rack.hpp>
#include <memory>

#include "BaseParam.hpp"

using namespace rack;

BaseParam::BaseParam(std::string name, engine::Output* output) {
    this->name = name;
    this->output = output;
    this->pile = std::make_unique<Pile>();
    this->clamp = std::make_unique<Clamp<int>>(0, 127);
    this->rescaler = std::make_unique<VoltageRescaler>();
    this->slew = std::make_unique<Slew>(0.0f);

    this->slewLimitQuantity = std::make_unique<CallbackQuantity>(
        "Slew limit", 0.0f, 2.0f, [this](float value) { slew->setLimit(value); }
    );

    this->voltageModeChoice = std::make_unique<VoltageModeChoice>(
        "Voltage mode", [this](VoltageMode voltageMode) {
            this->rescaler->setVoltageMode(voltageMode);
            this->send(this->pile->getValue());
        });

    this->resetData = BaseParam::toJson();
}

void BaseParam::save() {
    this->resetData = BaseParam::toJson();
}

void BaseParam::load() {
    this->fromJson(this->resetData);
}

std::string BaseParam::getName() {
    return this->name;
}

void BaseParam::setValue(int newValue) {
    if (newValue == pile->getValue()) {
        return;
    }
    pile->setValue(newValue);
    auto normal = clamp->normalized(pile->getValue());
    this->slew->setTarget(normal);
    this->slew->getCurrentOutput() = normal;
}

float BaseParam::getValue() {
    return pile->getValue();
}

float BaseParam::getNormalizedValue() {
    return clamp->normalized(getValue());
}

int BaseParam::getMin() {
    return clamp->getMin();
}

int BaseParam::getMax() {
    return clamp->getMax();
}

void BaseParam::setRange(int min, int max) {
    clamp->setMin(min);
    clamp->setMax(max);
    resend();
}

float BaseParam::getSlew() {
    return slew->getSlewed(APP->engine->getSampleTime());
}

void BaseParam::setSlew(float newSlew) {
    slewLimitQuantity->setValue(newSlew);
    resend();
}

VoltageMode BaseParam::getVoltageMode() {
    return rescaler->getVoltageMode();
}

void BaseParam::setVoltageMode(VoltageMode newVoltageMode) {
    voltageModeChoice->set(newVoltageMode);
    resend();
}

void BaseParam::sendCallbacks(float value) {
    for (auto& callback : this->valueChangeCallbacks) {
        callback(value);
    }
}

void BaseParam::whenValueChanges(std::function<void(float)> callback) {
    this->valueChangeCallbacks.push_back(callback);
}

bool BaseParam::slewEnabled() {
    return slew->getLimit() > 0.0f;
}

void BaseParam::resend() {
    send(getValue());
}

void BaseParam::send(int value) {
    this->pile->setValue(value);
    auto normalized = clamp->normalized(value);

    if (slew->getLimit() > 0.0f) {
        slew->setTarget(normalized);
        auto slewed = slew->getSlewed(APP->engine->getSampleTime());
        auto scaled = rescaler->rescale(slewed);
        output->setVoltage(scaled);
        sendCallbacks(normalized);
    } else {
        slew->setTarget(normalized);
        slew->getCurrentOutput() = normalized;
        auto scaled = rescaler->rescale(normalized);
        output->setVoltage(scaled);
        sendCallbacks(normalized);
    }
}

void BaseParam::process() {
    auto target = slew->getTarget();
    auto current = slew->getCurrentOutput();

    if (abs(target - current) < 0.0001f) {
        return;
    }

    auto slewed = slewEnabled() ? slew->getSlewed(APP->engine->getSampleTime())
                                : slew->getTarget();

    auto scaled = rescaler->rescale(slewed);
    output->setVoltage(scaled);
}

json_t* BaseParam::toJson() {
    json_t* rootJ = json_object();
    json_object_set_new(rootJ, "pile", pile->toJson());
    json_object_set_new(rootJ, "clamp", clamp->toJson());
    json_object_set_new(rootJ, "rescaler", rescaler->toJson());
    json_object_set_new(rootJ, "slew", slew->toJson());
    json_object_set_new(rootJ, "slewLimit", slewLimitQuantity->toJson());
    json_object_set_new(rootJ, "voltageMode", voltageModeChoice->toJson());
    return rootJ;
}

void BaseParam::fromJson(json_t* rootJ) {
    json_t* slewLimitJ = json_object_get(rootJ, "slewLimit");
    if (slewLimitJ) {
        slewLimitQuantity->fromJson(slewLimitJ);
    }
    json_t* voltageModeJ = json_object_get(rootJ, "voltageMode");
    if (voltageModeJ) {
        voltageModeChoice->fromJson(voltageModeJ);
    }
    json_t* pileJ = json_object_get(rootJ, "pile");
    if (pileJ) {
        pile->fromJson(pileJ);
    }
    json_t* clampJ = json_object_get(rootJ, "clamp");
    if (clampJ) {
        clamp->fromJson(clampJ);
    }
    json_t* rescalerJ = json_object_get(rootJ, "rescaler");
    if (rescalerJ) {
        rescaler->fromJson(rescalerJ);
    }
    json_t* slewJ = json_object_get(rootJ, "slew");
    if (slewJ) {
        slew->fromJson(slewJ);
    }
}