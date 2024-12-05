#include "Pile.hpp"

Pile::Pile() {
    this->value = 0;
    this->decCode = 64;
    this->incCode = 65;
    this->strength = 1;
}

Pile::Pile(int strength) {
    this->value = 0;
    this->decCode = 64;
    this->incCode = 65;
    this->strength = strength;
}

Pile::Pile(int strength, int decCode, int incCode) {
    this->value = 0;
    this->decCode = decCode;
    this->incCode = incCode;
    this->strength = strength;
}

void Pile::setDecCode(int newDecCode) {
    decCode = newDecCode;
}

void Pile::setIncCode(int newIncCode) {
    incCode = newIncCode;
}

int Pile::getStrength() {
    return strength;
}

void Pile::setStrength(int newStrength) {
    strength = newStrength > 0 ? newStrength : 1;
}

int Pile::getValue() {
    return this->value;
}

void Pile::setValue(int newValue) {
    value = newValue;
}

int Pile::inc() {
    this->value += this->strength;
    return value;
}

int Pile::dec() {
    this->value -= this->strength;
    return value;
}

int Pile::send(int code) {
    if (code == -1) {
        return value;
    }

    if (code <= this->decCode) {
        return this->dec();
    } else if (code >= this->incCode) {
        return this->inc();
    }

    return value;
}

json_t* Pile::toJson() {
    json_t* rootJ = json_object();
    json_object_set_new(rootJ, "strength", json_integer(this->strength));
    json_object_set_new(rootJ, "value", json_integer(this->value));
    return rootJ;
}

void Pile::fromJson(json_t* rootJ) {
    json_t* strengthJ = json_object_get(rootJ, "strength");
    if (strengthJ) {
        this->strength = json_integer_value(strengthJ);
    }
    json_t* valueJ = json_object_get(rootJ, "value");
    if (valueJ) {
        this->value = json_integer_value(valueJ);
    }
}