
#include <utils/ClampedValue.hpp>

template <typename T>
ClampedValue<T>::ClampedValue(T value, T min, T max) {
    this->value = value < min ? min : value > max ? max : value;
    this->min = min;
    this->max = max;
}

template <typename T>
T ClampedValue<T>::getValue() {
    return value;
}

template <typename T>
void ClampedValue<T>::setValue(T value) {
    this->value = value < min ? min : value > max ? max : value;
}

template <typename T>
T ClampedValue<T>::getMin() {
    return min;
}

template <typename T>
T ClampedValue<T>::getMax() {
    return max;
}

template <typename T>
void ClampedValue<T>::setRange(T min, T max) {
    this->min = min;
    this->max = max;
}

template <typename T>
void ClampedValue<T>::setMin(T min) {
    this->min = min;
}

template <typename T>
void ClampedValue<T>::setMax(T max) {
    this->max = max;
}

// implement json serialization for both T = float and T = int

// float
template <>
json_t* ClampedValue<float>::toJson() {
    json_t* rootJ = json_object();
    json_object_set_new(rootJ, "value", json_real(getValue()));
    json_object_set_new(rootJ, "min", json_real(getMin()));
    json_object_set_new(rootJ, "max", json_real(getMax()));
    return rootJ;
}

template <>
void ClampedValue<float>::fromJson(json_t* rootJ) {
    json_t* valueJ = json_object_get(rootJ, "value");
    if (valueJ)
        setValue(json_number_value(valueJ));
    json_t* minJ = json_object_get(rootJ, "min");
    if (minJ)
        setMin(json_number_value(minJ));
    json_t* maxJ = json_object_get(rootJ, "max");
    if (maxJ)
        setMax(json_number_value(maxJ));
}

// int
template <>
json_t* ClampedValue<int>::toJson() {
    json_t* rootJ = json_object();
    json_object_set_new(rootJ, "value", json_integer(getValue()));
    json_object_set_new(rootJ, "min", json_integer(getMin()));
    json_object_set_new(rootJ, "max", json_integer(getMax()));
    return rootJ;
}

template <>
void ClampedValue<int>::fromJson(json_t* rootJ) {
    json_t* valueJ = json_object_get(rootJ, "value");
    if (valueJ)
        setValue(json_integer_value(valueJ));
    json_t* minJ = json_object_get(rootJ, "min");
    if (minJ)
        setMin(json_integer_value(minJ));
    json_t* maxJ = json_object_get(rootJ, "max");
    if (maxJ)
        setMax(json_integer_value(maxJ));
}

// explicitly instantiate float and int
template struct ClampedValue<float>;
template struct ClampedValue<int>;