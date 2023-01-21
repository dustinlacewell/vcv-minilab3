#include <iomanip>
#include <rack.hpp>

#include "MiniLab3.hpp"
#include "widgets/MiniLogWidget.hpp"

void resetMessages(dsp::RingBuffer<std::string, 512>& messages) {
    messages.clear();
    messages.push(string::f("RATE %i", int(APP->engine->getSampleRate())));
}

MiniLog::MiniLog() {
    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
    configLight(CONNECTED_LIGHT, "Connected");

    binder = new PadBinder(&midiInput);

    expanderDivider.setDivision(1024);

    resetMessages(messages);
}

void MiniLog::whenReinit(std::function<void()> callback) {
    reinitCallbacks.push_back(callback);
}

void MiniLog::process(const ProcessArgs& args) {
    if (expanderDivider.process()) {
        binder->process(rightExpander.module);
        if (midiInput.driverId >= 0 && midiInput.deviceId >= 0) {
            adjustLight(true);
        } else {
            adjustLight(false);
        }
    }

    midi::Message msg;
    while (midiInput.tryPop(&msg, args.frame)) {
        processMessage(msg);
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
    lights[CONNECTED_LIGHT].setBrightness(connected ? 1.0 : 0.0);
}

Model* modelMiniLog = createModel<MiniLog, MiniLogWidget>("MiniLog");