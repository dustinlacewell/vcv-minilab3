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
    std::unique_ptr<Pile> pile;
    std::unique_ptr<Clamp<int>> clamp;
    std::unique_ptr<VoltageRescaler> rescaler;
    std::unique_ptr<Slew> slew;
    std::vector<std::function<void(float)>> valueChangeCallbacks;

   public:
    std::unique_ptr<CallbackQuantity> slewLimitQuantity;
    std::unique_ptr<VoltageModeChoice> voltageModeChoice;

    BaseParam(std::string name, engine::Output* output);

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
    virtual void setRange(int newMin, int newMax);
    float getSlew();
    bool slewEnabled();
    void setSlew(float newSlew);
    VoltageMode getVoltageMode();
    void setVoltageMode(VoltageMode newMode);
    virtual json_t* toJson();
    virtual void fromJson(json_t* rootJ);
};