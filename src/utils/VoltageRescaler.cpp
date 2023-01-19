#include <rack.hpp>
#include <utils/VoltageMode.hpp>
#include <utils/VoltageRescaler.hpp>


using namespace rack::math;

VoltageRescaler::VoltageRescaler() {
    this->voltageMode = VoltageMode::UNIPOLAR_1;
}

VoltageRescaler::VoltageRescaler(VoltageMode voltageMode) {
    this->voltageMode = voltageMode;
}

VoltageMode VoltageRescaler::getVoltageMode() {
    return voltageMode;
}

void VoltageRescaler::setVoltageMode(VoltageMode voltageMode) {
    this->voltageMode = voltageMode;
}

float VoltageRescaler::rescale(float voltage) {
    switch (this->voltageMode) {
        case VoltageMode::UNIPOLAR_1:return voltage;
            break;
        case VoltageMode::UNIPOLAR_5:return rack::math::rescale(voltage, 0.0f, 1.0f, 0.0f, 5.0f);
            break;
        case VoltageMode::UNIPOLAR_10:return rack::math::rescale(voltage, 0.0f, 1.0f, 0.0f, 10.0f);
            break;
        case VoltageMode::BIPOLAR_1:return rack::math::rescale(voltage, 0.0f, 1.0f, -1.0f, 1.0f);
            break;
        case VoltageMode::BIPOLAR_5:return rack::math::rescale(voltage, 0.0f, 1.0f, -5.0f, 5.0f);
            break;
        case VoltageMode::BIPOLAR_10:return rack::math::rescale(voltage, 0.0f, 1.0f, -10.0f, 10.0f);
            break;
    }
}

json_t *VoltageRescaler::toJson() {
    json_t *rootJ = json_object();
    json_object_set_new(rootJ, "voltageMode", json_integer(voltageMode));
    return rootJ;
}

void VoltageRescaler::fromJson(json_t *rootJ) {
    json_t *voltageModeJ = json_object_get(rootJ, "voltageMode");
    if (voltageModeJ) {
        voltageMode = (VoltageMode) json_integer_value(voltageModeJ);
    }
}