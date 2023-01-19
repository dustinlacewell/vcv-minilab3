#include <rack.hpp>

#include <utils/Clamp.hpp>


using namespace rack::math;

template<typename T>
Clamp<T>::Clamp(T min, T max) {
    this->min = min;
    this->max = max;
}

template<typename T>
T Clamp<T>::clamped(T value) {
    return clamp(value, min, max);
}

template<typename T>
float Clamp<T>::normalized(T value) {
    auto clamped = this->clamped(value);
    return rescale(clamped, min, max, 0.0f, 1.0f);
}

template<typename T>
T Clamp<T>::getMin() {
    return min;
}

template<typename T>
T Clamp<T>::getMax() {
    return max;
}

template<typename T>
void Clamp<T>::setMin(T min) {
    this->min = min;
}

template<typename T>
void Clamp<T>::setMax(T max) {
    this->max = max;
}

// float
template<>
json_t *Clamp<float>::toJson() {
    json_t *rootJ = json_object();
    json_object_set_new(rootJ, "min", json_real(getMin()));
    json_object_set_new(rootJ, "max", json_real(getMax()));
    return rootJ;
}

template<>
void Clamp<float>::fromJson(json_t *rootJ) {
    json_t *minJ = json_object_get(rootJ, "min");
    if (minJ) {
        setMin(json_number_value(minJ));
    }
    json_t *maxJ = json_object_get(rootJ, "max");
    if (maxJ) {
        setMax(json_number_value(maxJ));
    }
}

// int
template<>
json_t *Clamp<int>::toJson() {
    json_t *rootJ = json_object();
    json_object_set_new(rootJ, "min", json_integer(getMin()));
    json_object_set_new(rootJ, "max", json_integer(getMax()));
    return rootJ;
}

template<>
void Clamp<int>::fromJson(json_t *rootJ) {
    json_t *minJ = json_object_get(rootJ, "min");
    if (minJ) {
        setMin(json_integer_value(minJ));
    }
    json_t *maxJ = json_object_get(rootJ, "max");
    if (maxJ) {
        setMax(json_integer_value(maxJ));
    }
}

// explicitly instantiate float and int
template
struct Clamp<float>;
template
struct Clamp<int>;