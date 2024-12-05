#include "Slew.hpp"
#include <rack.hpp>
#include "VoltageMode.hpp"

using namespace rack::dsp;

namespace {
static constexpr float EPSILON = 0.00001f;
static constexpr float VOLTAGE_MODE_SCALES[] = {
    1.f,   // UNIPOLAR_1/BIPOLAR_1
    1.f,   // Paired mode
    5.f,   // UNIPOLAR_5/BIPOLAR_5
    5.f,   // Paired mode
    10.f,  // UNIPOLAR_10/BIPOLAR_10
    10.f   // Paired mode
};
}  // namespace

Slew::Slew(float limit)
    : limit(limit), voltageMode(VoltageMode::UNIPOLAR_1), target(0.f) {}

Slew::Slew(float limit, VoltageMode voltageMode)
    : limit(limit), voltageMode(voltageMode), target(0.f) {}

float Slew::getSlewed(float sampleTime) noexcept {
    if (limit <= EPSILON) {
        return target;
    }

    const float limitValue =
        (1.f / limit) *
        VOLTAGE_MODE_SCALES[static_cast<std::size_t>(voltageMode)];
    slewLimiter.setRiseFall(limitValue, limitValue);
    return slewLimiter.process(sampleTime, target);
}

json_t* Slew::toJson() const {
    json_t* const rootJ = json_object();
    if (!rootJ)
        return nullptr;

    json_object_set_new(rootJ, "limit", json_real(limit));
    json_object_set_new(rootJ, "target", json_real(target));
    json_object_set_new(
        rootJ, "voltageMode", json_integer(static_cast<int>(voltageMode))
    );
    json_object_set_new(rootJ, "slewLimiterOut", json_real(slewLimiter.out));

    return rootJ;
}

void Slew::fromJson(json_t* const rootJ) {
    if (!rootJ)
        return;

    if (json_t* const limitJ = json_object_get(rootJ, "limit")) {
        limit = static_cast<float>(json_number_value(limitJ));
    }

    if (json_t* const targetJ = json_object_get(rootJ, "target")) {
        target = static_cast<float>(json_number_value(targetJ));
    }

    if (json_t* const voltageModeJ = json_object_get(rootJ, "voltageMode")) {
        voltageMode =
            static_cast<VoltageMode>(json_integer_value(voltageModeJ));
    }

    if (json_t* const slewLimiterOutJ =
            json_object_get(rootJ, "slewLimiterOut")) {
        slewLimiter.out =
            static_cast<float>(json_number_value(slewLimiterOutJ));
    }
}