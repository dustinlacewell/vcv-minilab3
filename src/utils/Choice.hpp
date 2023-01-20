#pragma once

#include <rack.hpp>

using namespace rack;

template <typename T>
struct Choice {
    T value;
    std::string label;
    std::function<void(T)> callback;

    virtual ~Choice() { callback = NULL; }
    virtual std::string getLabel() { return label; }
    virtual std::vector<std::tuple<std::string, T>> getChoices() = 0;

    void set(T newValue) {
        this->value = newValue;
        if (callback) {
            callback(newValue);
        }
    }

    T get() { return value; }

    json_t* toJson() {
        json_t* rootJ = json_object();
        json_object_set_new(rootJ, "value", json_integer(value));
        json_object_set_new(rootJ, "label", json_string(getLabel().c_str()));
        return rootJ;
    }

    void fromJson(json_t* rootJ) {
        json_t* valueJ = json_object_get(rootJ, "value");
        if (valueJ) {
            set((T)json_integer_value(valueJ));
        }
    }
};