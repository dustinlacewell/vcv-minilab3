#pragma once
#include <iomanip>
#include <rack.hpp>
#include <memory>

#include "logging/MidiMessageFilter.hpp"
#include "logging/MidiMessageRenderer.hpp"
#include "plugin.hpp"
#include "ui/TextLogWidget.hpp"

const int BUFFERSIZE = 800;

struct MiniLog : Module {
    enum ParamId { PARAMS_LEN };
    enum InputId { INPUTS_LEN };
    enum OutputId { OUTPUTS_LEN };
    enum LightId { STATUS_LIGHT, LIGHTS_LEN };

    dsp::ClockDivider lightDivider;
    MidiMessageFilter midiMessageFilter;
    MidiMessageRenderer midiMessageRenderer;

    dsp::RingBuffer<std::string, 512> messages;
    std::vector<std::function<void()>> reinitCallbacks;

    MiniLog();
    void whenReinit(std::function<void()> callback);
    void process(const ProcessArgs& args) override;
    void onReset() override;
    void processMessage(midi::Message& msg);
    void adjustLight(bool connected);
};