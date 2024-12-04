#pragma once
#include <rack.hpp>
#include "BaseModule.hpp"

struct G8Pad : BaseModule {
    int driverId = -1;
    int deviceId = -1;
    int channel = -1;

    G8Pad();
    void process(const ProcessArgs& args) override;

   private:
    dsp::ClockDivider driverDivider;
    dsp::ClockDivider positionDivider;
    dsp::ClockDivider midiDivider;
    dsp::ClockDivider paramDivider;

    int getPadIdFromNote(int note) const;
    void processMessage(const midi::Message& msg);
};