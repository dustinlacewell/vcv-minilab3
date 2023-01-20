#include <iomanip>
#include <rack.hpp>

#include "MiniLab3.hpp"
#include "plugin.hpp"
#include "ui/TextLogWidget.hpp"
#include "utils/MidiMessageFilter.hpp"
#include "utils/MidiMessageRenderer.hpp"

const int BUFFERSIZE = 800;

struct MiniLog : Module {
    enum ParamId { PARAMS_LEN };
    enum InputId { INPUTS_LEN };
    enum OutputId { OUTPUTS_LEN };
    enum LightId { CONNECTED_LIGHT, LIGHTS_LEN };

    midi::InputQueue midiInput;
    ClockDivider expanderDivider;

    MidiMessageFilter midiMessageFilter;
    MidiMessageRenderer midiMessageRenderer;

    dsp::RingBuffer<std::string, 512> messages;

    MiniLog();
    void process(const ProcessArgs& args) override;
    void onReset() override;
    void processMessage(midi::Message& msg);
    void adjustLight(bool connected);
};