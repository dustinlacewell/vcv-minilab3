#include "RelativeParam.hpp"
#include "CallbackQuantity.hpp"

RelativeParam::RelativeParam(engine::Output* output) {
    this->output = output;
    this->pile = new Pile();
    this->clamp = new Clamp<int>(0, 127);
    this->rescaler = new VoltageRescaler();
    this->slew = new Slew(0.0f);

    this->slewLimitQuantity = new CallbackQuantity(
        "Slew limit", 0.0f, 1.0f, [this](float value) { slew->setLimit(value); }
    );
}

RelativeParam::RelativeParam(engine::Output* output, float slew) {
    this->output = output;
    this->pile = new Pile();
    this->clamp = new Clamp<int>(0, 127);
    this->rescaler = new VoltageRescaler();
    this->slew = new Slew(slew);

    this->slewLimitQuantity =
        new CallbackQuantity("Slew limit", 0.0f, 1.0f, [this](float value) {
            this->slew->setLimit(value);
        });
}

RelativeParam::RelativeParam(engine::Output* output, float slew, int strength) {
    this->output = output;
    this->pile = new Pile(strength);
    this->clamp = new Clamp<int>(0, 127);
    this->rescaler = new VoltageRescaler();
    this->slew = new Slew(slew);

    this->slewLimitQuantity =
        new CallbackQuantity("Slew limit", 0.0f, 1.0f, [this](float value) {
            this->slew->setLimit(value);
        });
}

RelativeParam::RelativeParam(
    engine::Output* output, float slew, int strength, int min, int max
) {
    this->output = output;
    this->pile = new Pile(strength);
    this->clamp = new Clamp<int>(min, max);
    this->rescaler = new VoltageRescaler();
    this->slew = new Slew(slew);

    this->slewLimitQuantity =
        new CallbackQuantity("Slew limit", 0.0f, 1.0f, [this](float value) {
            this->slew->setLimit(value);
        });
}

RelativeParam::~RelativeParam() {
    delete pile;
    delete clamp;
    delete rescaler;
    delete slew;
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
    json_t* rootJ = json_object();
    json_object_set_new(rootJ, "pile", pile->toJson());
    json_object_set_new(rootJ, "clamp", clamp->toJson());
    json_object_set_new(rootJ, "rescaler", rescaler->toJson());
    json_object_set_new(rootJ, "slew", slew->toJson());
    return rootJ;
}

void RelativeParam::fromJson(json_t* rootJ) {
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

    // Update slew limit quantity
    slewLimitQuantity->setValue(slew->getLimit());
}