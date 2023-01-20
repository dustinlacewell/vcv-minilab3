#pragma once

#include <rack.hpp>

#include "CallbackQuantity.hpp"
#include "Clamp.hpp"
#include "Slew.hpp"
#include "VoltageMode.hpp"
#include "VoltageModeChoice.hpp"
#include "VoltageRescaler.hpp"

using namespace rack;

struct BaseParam {
   protected:
    engine::Output* output;
    Clamp<int>* clamp;
    VoltageRescaler* rescaler;
    Slew* slew;

   public:
    CallbackQuantity* slewLimitQuantity;
    VoltageModeChoice* voltageModeChoice;

    BaseParam(engine::Output* output);
    ~BaseParam();

    virtual void send(int value);
    virtual void process();

    void setRange(int min, int max);
    void setSlew(float slew) const;
    void setVoltageMode(VoltageMode voltageMode) const;

    virtual json_t* toJson();
    virtual void fromJson(json_t* rootJ);
};