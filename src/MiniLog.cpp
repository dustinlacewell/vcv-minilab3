#include <iomanip>
#include <rack.hpp>

#include "MiniLog.hpp"
#include "widgets/MiniLogWidget.hpp"

void resetMessages(dsp::RingBuffer<std::string, 512>& messages) {
    messages.clear();
    messages.push(string::f("RATE %i", int(APP->engine->getSampleRate())));
}

MiniLog::MiniLog() {
    DEBUG("MiniLog: Starting construction");
    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
    DEBUG("MiniLog: Did config()");
    configLight(STATUS_LIGHT, "Connected");
    DEBUG("MiniLog: Did configLight()");

    lightDivider.setDivision(1024);
    DEBUG("MiniLog: Did setDivision()");
    resetMessages(messages);
    DEBUG("MiniLog: Did resetMessages()");
    DEBUG("MiniLog: Construction complete");
}

void MiniLog::whenReinit(std::function<void()> callback) {
    reinitCallbacks.push_back(callback);
}

void MiniLog::process(const ProcessArgs& args) {
    if (lightDivider.process()) {
        if (rightExpander.module) {
            auto miniLab = dynamic_cast<MiniLab3*>(rightExpander.module);
            if (miniLab) {
                adjustLight(true);
            } else {
                adjustLight(false);
            }
        } else {
            adjustLight(false);
        }
    }
}

void MiniLog::onReset() {
    resetMessages(messages);
    midiMessageFilter.reset();
    Module::onReset();
    for (auto& callback : reinitCallbacks) {
        callback();
    }
}

void MiniLog::processMessage(midi::Message& msg) {
    if (messages.full()) {
        return;
    }
    if (midiMessageFilter.shouldHandle(msg)) {
        std::string renderedMsg = midiMessageRenderer.render(msg);
        messages.push(renderedMsg);
    }
}

void MiniLog::adjustLight(bool connected) {
    lights[STATUS_LIGHT].setBrightness(connected ? 1.0 : 0.0);
}

Model* modelMiniLog = createModel<MiniLog, MiniLogWidget>("MiniLog");