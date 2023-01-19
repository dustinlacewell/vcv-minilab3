#include <utils/CallbackQuantity.hpp>


CallbackQuantity::CallbackQuantity(std::string label, float min, float max, std::function<void(float)> callback) {
    this->label = label;
    this->callback = callback;
    setRange(min, max);
}

std::string CallbackQuantity::getLabel() {
    return label;
}

float CallbackQuantity::getValue() {
    return value;
}

void CallbackQuantity::setValue(float newValue) {
    value = clamp(newValue, min_value, max_value);
    callback(value);
}

float CallbackQuantity::getDefaultValue() {
    return 0.5f;
}

int CallbackQuantity::getDisplayPrecision() {
    return 2;
}

float CallbackQuantity::getMaxValue() {
    return max_value;
}

float CallbackQuantity::getMinValue() {
    return min_value;
}

void CallbackQuantity::setRange(float min, float max) {
    min_value = min;
    max_value = max;
}

json_t *CallbackQuantity::toJson() {
    json_t *rootJ = json_object();
    json_object_set_new(rootJ, "value", json_real(getValue()));
    json_object_set_new(rootJ, "min", json_real(getMinValue()));
    json_object_set_new(rootJ, "max", json_real(getMaxValue()));
    json_object_set_new(rootJ, "label", json_string(getLabel().c_str()));
    return rootJ;
}

void CallbackQuantity::fromJson(json_t *rootJ) {
    json_t *valueJ = json_object_get(rootJ, "value");
    if (valueJ) {
        setValue(json_number_value(valueJ));
    }
    json_t *minJ = json_object_get(rootJ, "min");
    json_t *maxJ = json_object_get(rootJ, "max");
    if (minJ && maxJ) {
        setRange(json_number_value(minJ), json_number_value(maxJ));
    }
    json_t *labelJ = json_object_get(rootJ, "label");
    if (labelJ) {
        label = json_string_value(labelJ);
    }
}