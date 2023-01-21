#pragma once

#include <rack.hpp>

#include "props/CallbackQuantity.hpp"
#include "props/VoltageModeChoice.hpp"
#include "utils/Clamp.hpp"
#include "utils/Pile.hpp"
#include "utils/Slew.hpp"
#include "utils/VoltageMode.hpp"
#include "utils/VoltageRescaler.hpp"

using namespace rack;

struct BaseParam {
   protected:
    json_t* resetData;
    std::string name;
    engine::Output* output;
    Pile* pile;
    Clamp<int>* clamp;
    VoltageRescaler* rescaler;
    Slew* slew;

    std::vector<std::function<void(float)>> valueChangeCallbacks;

   public:
    CallbackQuantity* slewLimitQuantity;
    VoltageModeChoice* voltageModeChoice;

    BaseParam(std::string name, engine::Output* output);
    ~BaseParam();

    void save();

    virtual void load();
    virtual void resend();
    virtual void send(int value);
    virtual void process();
    virtual void sendCallbacks(float value);

    void whenValueChanges(std::function<void(float)> callback);

    std::string getName();

    void setValue(int newValue);
    float getValue();
    float getNormalizedValue();

    int getMin();
    int getMax();
    void setRange(int newMin, int newMax);

    float getSlew();
    bool slewEnabled();
    void setSlew(float newSlew);

    VoltageMode getVoltageMode();
    void setVoltageMode(VoltageMode newMode);

    virtual json_t* toJson();
    virtual void fromJson(json_t* rootJ);
};