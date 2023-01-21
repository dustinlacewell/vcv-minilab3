#pragma once

#include <rack.hpp>

using namespace rack;

struct Pile {

    int decCode = 63;
    int incCode = 66;
    int strength = 1;

    int value = 0;

    Pile();
    Pile(int strength);
    Pile(int strength, int decCode, int incCode);

    void setDecCode(int newDecCode);
    void setIncCode(int newIncCode);

    int getStrength();
    void setStrength(int newStrength);

    int getValue();
    void setValue(int newValue);

    int inc();
    int dec();
    int send(int code);

    json_t* toJson();
    void fromJson(json_t* rootJ);
};