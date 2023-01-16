
#include <rack.hpp>
#include <utils/VoltageMode.hpp>
#include <utils/ClampedValue.hpp>
#include <utils/VoltageRescaler.hpp>

using namespace rack::math;


template <typename T>
VoltageRescaler<T>::VoltageRescaler(ClampedValue<T>* clamp) {
    this->clampedValue = clamp;
    this->voltageMode = VoltageMode::UNIPOLAR_1;
}

template <typename T>
VoltageRescaler<T>::VoltageRescaler(ClampedValue<T>* clamp, VoltageMode voltageMode) {
    this->clampedValue = clamp;
    this->voltageMode = voltageMode;
}

template <typename T>
VoltageMode VoltageRescaler<T>::getVoltageMode() {
    return voltageMode;
}

template <typename T>
void VoltageRescaler<T>::setVoltageMode(VoltageMode voltageMode) {
    this->voltageMode = voltageMode;
}

template <>
float VoltageRescaler<float>::getVoltage(float sampleTime, int sampleRate) {
    float value = clampedValue->getValue();
    float min = clampedValue->getMin();
    float max = clampedValue->getMax();
    float voltage = 0.0f;
    switch (voltageMode) {
        case VoltageMode::UNIPOLAR_1:
            return rescale(value, min, max, 0.0f, 1.0f);
            break;
        case VoltageMode::UNIPOLAR_5:
            return rescale(value, min, max, 0.0f, 5.0f);
            break;
        case VoltageMode::UNIPOLAR_10:
            return rescale(value, min, max, 0.0f, 10.0f);
            break;
        case VoltageMode::BIPOLAR_1:
            return rescale(value, min, max, -1.0f, 1.0f);
            break;
        case VoltageMode::BIPOLAR_5:
            return rescale(value, min, max, -5.0f, 5.0f);
            break;
        case VoltageMode::BIPOLAR_10:
            return rescale(value, min, max, -10.0f, 10.0f);
            break;
    }
}

template <>
float VoltageRescaler<int>::getVoltage(float sampleTime, int sampleRate) {
    int value = clampedValue->getValue();
    int min = clampedValue->getMin();
    int max = clampedValue->getMax();
    float voltage = 0.0f;
    switch (voltageMode) {
        case VoltageMode::UNIPOLAR_1:
            return rescale(value, min, max, 0.0f, 1.0f);
            break;
        case VoltageMode::UNIPOLAR_5:
            return rescale(value, min, max, 0.0f, 5.0f);
            break;
        case VoltageMode::UNIPOLAR_10:
            return rescale(value, min, max, 0.0f, 10.0f);
            break;
        case VoltageMode::BIPOLAR_1:
            return rescale(value, min, max, -1.0f, 1.0f);
            break;
        case VoltageMode::BIPOLAR_5:
            return rescale(value, min, max, -5.0f, 5.0f);
            break;
        case VoltageMode::BIPOLAR_10:
            return rescale(value, min, max, -10.0f, 10.0f);
            break;
    }
}

template <typename T>
json_t* VoltageRescaler<T>::toJson() {
    json_t* rootJ = json_object();
    json_object_set_new(rootJ, "voltageMode", json_integer(voltageMode));
    json_object_set_new(rootJ, "clampedValue", clampedValue->toJson());
    return rootJ;
}

template <typename T>
void VoltageRescaler<T>::fromJson(json_t* rootJ, ClampedValue<T>* clamp) {
    json_t* voltageModeJ = json_object_get(rootJ, "voltageMode");
    if (voltageModeJ) {
        voltageMode = (VoltageMode)json_integer_value(voltageModeJ);
    }
    clampedValue = clamp;
}

template struct VoltageRescaler<float>;
template struct VoltageRescaler<int>;