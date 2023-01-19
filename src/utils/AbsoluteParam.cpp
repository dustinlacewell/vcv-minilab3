#include <utils/AbsoluteParam.hpp>
#include <utils/CallbackQuantity.hpp>


AbsoluteParam::AbsoluteParam(engine::Output *output) {
    this->output = output;
    this->clamp = new Clamp<int>(0, 127);
    this->rescaler = new VoltageRescaler();
    this->slew = new Slew(0.0f);

    this->slewLimitQuantity = new CallbackQuantity("Slew limit", 0.0f, 1.0f, [this](float value) { slew->setLimit(value); });
}

AbsoluteParam::AbsoluteParam(engine::Output *output, int min, int max) {
    this->output = output;
    this->clamp = new Clamp<int>(min, max);
    this->rescaler = new VoltageRescaler();
    this->slew = new Slew(0.0f);

    this->slewLimitQuantity = new CallbackQuantity("Slew limit", 0.0f, 1.0f, [this](float value) { slew->setLimit(value); });
}

AbsoluteParam::AbsoluteParam(engine::Output *output, int min, int max, float slew) {
    this->output = output;
    this->clamp = new Clamp<int>(min, max);
    this->rescaler = new VoltageRescaler();
    this->slew = new Slew(slew);

    this->slewLimitQuantity = new CallbackQuantity("Slew limit", 0.0f, 1.0f, [this](float value) { this->slew->setLimit(value); });
}

AbsoluteParam::~AbsoluteParam() {
    delete clamp;
    delete rescaler;
    delete slew;
}

void AbsoluteParam::send(int value) {
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

void AbsoluteParam::process() {
    if (slew->getLimit() > 0.0f) {
        auto slewed = slew->getSlewed(APP->engine->getSampleTime());
        output->setVoltage(slewed);
    }
}

json_t *AbsoluteParam::toJson() {
    json_t *rootJ = json_object();
    json_object_set_new(rootJ, "clamp", clamp->toJson());
    json_object_set_new(rootJ, "rescaler", rescaler->toJson());
    json_object_set_new(rootJ, "slew", slew->toJson());
    json_object_set_new(rootJ, "slewLimit", slewLimitQuantity->toJson());
    return rootJ;
}

void AbsoluteParam::fromJson(json_t *rootJ) {
    json_t *clampJ = json_object_get(rootJ, "clamp");
    if (clampJ) {
        clamp->fromJson(clampJ);
    }
    json_t *rescalerJ = json_object_get(rootJ, "rescaler");
    if (rescalerJ) {
        rescaler->fromJson(rescalerJ);
    }
    json_t *slewJ = json_object_get(rootJ, "slew");
    if (slewJ) {
        slew->fromJson(slewJ);
    }

    // Update slew limit quantity
    json_t *slewLimitJ = json_object_get(rootJ, "slewLimit");
    if (slewLimitJ) {
        slewLimitQuantity->fromJson(slewLimitJ);
    }
}