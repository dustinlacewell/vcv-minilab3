#include "RelativeParam.hpp"
#include "props/CallbackQuantity.hpp"
#include "props/ResolutionChoice.hpp"

void RelativeParam::recalc(Strength strength, Resolution resolution) {

    switch (resolution) {
        case Resolution::Two:
            clamp->setMax(2);
            break;
        case Resolution::Three:
            clamp->setMax(3);
            break;
        case Resolution::Four:
            clamp->setMax(4);
            break;
        case Resolution::Five:
            clamp->setMax(5);
            break;
        case Resolution::Eight:
            clamp->setMax(8);
            break;
        case Resolution::Twelve:
            clamp->setMax(12);
            break;
        case Resolution::Low:
            clamp->setMax(128);
            break;
        case Resolution::Medium:
            clamp->setMax(1024);
            break;
        case Resolution::High:
            clamp->setMax(8192);
            break;
    }

    auto max = getMax();
    switch (strength) {
        case Strength::OneTick:
            pile->setStrength(1);
            break;
        case Strength::TwoTicks:
            pile->setStrength(2);
            break;
        case Strength::TenTicks:
            pile->setStrength(10);
            break;
        case Strength::OnePercent:
            pile->setStrength(max / 100);
            break;
        case Strength::TwoPercent:
            pile->setStrength(max / 50);
            break;
        case Strength::FivePercent:
            pile->setStrength(max / 20);
            break;
        case Strength::TenPercent:
            pile->setStrength(max / 10);
            break;
        case Strength::TwentyPercent:
            pile->setStrength(max / 5);
            break;
        case Strength::FiftyPercent:
            pile->setStrength(max / 2);
            break;
        case Strength::Toggle:
            pile->setStrength(max);
            break;
    }
}

RelativeParam::RelativeParam(std::string name, Output* output)
    : BaseParam(name, output) {
    this->strengthChoice = std::make_unique<StrengthChoice>(
        "Relative Strength",
        [this](Strength strength) {
            recalc(strength, resolutionChoice->get());
            resend();
        }
    );

    this->resolutionChoice = std::make_unique<ResolutionChoice>(
        "Resolution",
        [this](Resolution resolution) {
            recalc(strengthChoice->get(), resolution);
            resend();
        }
    );

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
        slew->getCurrentOutput() = normalized;
        auto scaled = rescaler->rescale(normalized);
        output->setVoltage(scaled);
        sendCallbacks(normalized);
    }
}

int RelativeParam::getStrength() {
    return strengthChoice->get();
}

void RelativeParam::setStrength(Strength strength) {
    this->strengthChoice->set(strength);
}

void RelativeParam::setResolution(Resolution resolution) {
    this->resolutionChoice->set(resolution);
}

json_t* RelativeParam::toJson() {
    auto rootJ = BaseParam::toJson();
    json_object_set_new(rootJ, "strength", strengthChoice->toJson());
    json_object_set_new(rootJ, "resolution", resolutionChoice->toJson());
    return rootJ;
}

void RelativeParam::fromJson(json_t* rootJ) {
    json_t* strengthJ = json_object_get(rootJ, "strength");
    if (strengthJ) {
        strengthChoice->fromJson(strengthJ);
    }

    json_t* resolutionJ = json_object_get(rootJ, "resolution");
    if (resolutionJ) {
        resolutionChoice->fromJson(resolutionJ);
    }
    BaseParam::fromJson(rootJ);
}