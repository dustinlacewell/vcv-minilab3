#include "PadBinder.hpp"

#include "../G8Pad.hpp"
#include "../plugin.hpp"

int PadBinder::process(Module* module) {
    if (module == NULL) {
        return unbind();
    } else if (module->model == modelMiniLab3) {
        return bindToMiniLab3(module);
    } else if (module->model == modelG8Pad) {
        return bindToG8Pad(module);
    } else {
        return unbind();
    }
}

void PadBinder::attachMidi(InputQueue& newQueue) {
    bool driverMatch = newQueue.driverId == midiInput->driverId;
    bool deviceMatch = newQueue.deviceId == midiInput->deviceId;
    bool channelMatch = newQueue.channel == midiInput->channel;

    if (driverMatch && deviceMatch && channelMatch) {
        return;
    }

    this->unbind();

    midiInput->setDriverId(newQueue.driverId);
    midiInput->setDeviceId(newQueue.deviceId);
    midiInput->setChannel(newQueue.channel);
}

int PadBinder::unbind() {
    midiInput->setChannel(-1);
    midiInput->setDeviceId(-1);
    midiInput->setDriverId(-1);
    return -1;
}

int PadBinder::bindToMiniLab3(Module* module) {
    MiniLab3* miniLab3 = dynamic_cast<MiniLab3*>(module);

    if (miniLab3) {
        attachMidi(miniLab3->midiInput);
        return 1;
    } else {
        return unbind();
    }
}

int PadBinder::bindToG8Pad(Module* module) {
    G8Pad* leftPad = dynamic_cast<G8Pad*>(module);
    int padId = leftPad->getId();
    if (leftPad != nullptr && padId >= 0) {
        attachMidi(leftPad->midiInput);
        return padId + 1;
    } else {
        return unbind();
    }
}