#include <iomanip>
#include <rack.hpp>

#include "MiniLab3.hpp"
#include "widgets/MiniLogWidget.hpp"

MiniLog::MiniLog() {
    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
    configLight(CONNECTED_LIGHT, "Connected");

    midiInput.setChannel(-1);
    midiInput.setDeviceId(-1);
    midiInput.setDriverId(-1);

    expanderDivider.setDivision(1024);

    onReset();
}

void MiniLog::process(const ProcessArgs& args) {
    if (expanderDivider.process()) {
        // check if right expander is MiniLab3
        auto miniLab3 = dynamic_cast<MiniLab3*>(rightExpander.module);
        if (miniLab3) {
            // copy minilab3's mini input settings
            midiInput.setDriverId(miniLab3->midiInput.driverId);
            midiInput.setDeviceId(miniLab3->midiInput.deviceId);
            midiInput.setChannel(miniLab3->midiInput.channel);
            bool ready = miniLab3->midiInput.deviceId >= 0;
            adjustLight(ready);
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
    midiMessageFilter.reset();
    messages.push(string::f("RATE %i", int(APP->engine->getSampleRate())));
    Module::onReset();
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