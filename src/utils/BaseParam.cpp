#include <rack.hpp>

#include "BaseParam.hpp"

using namespace rack;

BaseParam::BaseParam(engine::Output* output) {
    this->output = output;
    this->clamp = new Clamp<int>(0, 127);
    this->rescaler = new VoltageRescaler();
    this->slew = new Slew(0.0f);

    this->slewLimitQuantity = new CallbackQuantity(
        "Slew limit", 0.0f, 1.0f, [this](float value) { slew->setLimit(value); }
    );

    this->voltageModeChoice =
        new VoltageModeChoice("Voltage mode", [this](VoltageMode voltageMode) {
            this->rescaler->setVoltageMode(voltageMode);
        });
}

BaseParam::~BaseParam() {
    delete this->clamp;
    delete this->rescaler;
    delete this->slew;
    delete this->slewLimitQuantity;
    delete this->voltageModeChoice;
}

void BaseParam::setRange(int min, int max) {
    this->clamp->setMin(min);
    this->clamp->setMax(max);
}

void BaseParam::setSlew(float newSlew) const {
    this->slewLimitQuantity->setValue(newSlew);
}

void BaseParam::setVoltageMode(VoltageMode newVoltageMode) const {
    this->voltageModeChoice->set(newVoltageMode);
}

void BaseParam::send(int value) {
    auto normalized = clamp->normalized(value);
    auto scaled = rescaler->rescale(normalized);
    if (slew->getLimit() > 0.0f) {
        slew->setTarget(scaled);
        auto slewed = slew->getSlewed(APP->engine->getSampleTime());
        output->setVoltage(slewed);
    } else {
        output->setVoltage(scaled);
    }
}

void BaseParam::process() {
    if (slew->getLimit() > 0.0f) {
        auto slewed = slew->getSlewed(APP->engine->getSampleTime());
        output->setVoltage(slewed);
    }
}

json_t* BaseParam::toJson() {
    json_t* rootJ = json_object();
    json_object_set_new(rootJ, "clamp", clamp->toJson());
    json_object_set_new(rootJ, "rescaler", rescaler->toJson());
    json_object_set_new(rootJ, "slew", slew->toJson());
    json_object_set_new(rootJ, "slewLimit", slewLimitQuantity->toJson());
    json_object_set_new(rootJ, "voltageMode", voltageModeChoice->toJson());
    return rootJ;
}

void BaseParam::fromJson(json_t* rootJ) {
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

    // Update slew limit quantity
    json_t* slewLimitJ = json_object_get(rootJ, "slewLimit");
    if (slewLimitJ) {
        slewLimitQuantity->fromJson(slewLimitJ);
    }

    // Update voltage mode choice
    json_t* voltageModeJ = json_object_get(rootJ, "voltageMode");
    if (voltageModeJ) {
        voltageModeChoice->fromJson(voltageModeJ);
    }
}