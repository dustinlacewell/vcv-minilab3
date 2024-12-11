#pragma once

#include <rack.hpp>
#include "BaseModule.hpp"

struct MiniPad : BaseModule {
    midi::InputQueue midiControl;
    int driverId = -1;
    int deviceId = -1;
    int channel = -1;
    bool isReady = false;
    bool dirty = false;

    MiniPad();
    void process(const ProcessArgs& args) override;
    void dataFromJson(json_t* rootJ) override;

   private:
    dsp::ClockDivider driverDivider;
    dsp::ClockDivider positionDivider;
    dsp::ClockDivider midiDivider;
    dsp::ClockDivider paramDivider;

    int getPadIdFromNote(int note) const;
    void processControl(const midi::Message& msg);
    void processMessage(const midi::Message& msg);    
};