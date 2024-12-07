#pragma once

#include <rack.hpp>
#include "BaseModule.hpp"

struct MiniLab : BaseModule {
    bool isReady;
    std::array<AbsoluteParam*, 4> sliders{};

    dsp::ClockDivider scanDivider;
    dsp::ClockDivider midiDivider;
    dsp::ClockDivider paramDivider;

    MiniLab();
    void process(const ProcessArgs& args) override;

   private:
    std::tuple<int, int> scan();
    void processParams();
    void processMessage(const midi::Message& msg);
};