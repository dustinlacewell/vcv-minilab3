#include <G8Pad.hpp>
#include <MiniLab3.hpp>
#include <utils/PadBinder.hpp>

#include "../plugin.hpp"


int PadBinder::process(Module *
module) {
if (module == NULL) {
return
unbind();
} else if (module->model == modelMiniLab3) {
return bindToMiniLab3(module);
} else if (module->model == modelG8Pad) {
return bindToG8Pad(module);
} else {
return
unbind();
}}

void PadBinder::attachMidi(InputQueue &midiInput) {
    bool driverMatch = midiInput.driverId == this->midiInput->driverId;
    bool deviceMatch = midiInput.deviceId == this->midiInput->deviceId;
    bool channelMatch = midiInput.channel == this->midiInput->channel;

    if (driverMatch && deviceMatch && channelMatch) {
        return;
    }

    this->unbind();

    INFO("Attaching pad to root midi");
    INFO("Driver: %d", midiInput.driverId);
    INFO("Device: %d", midiInput.deviceId);
    INFO("Channel: %d", midiInput.channel);

    this->midiInput->setDriverId(midiInput.driverId);
    this->midiInput->setDeviceId(midiInput.deviceId);
    this->midiInput->setChannel(midiInput.channel);
}

int PadBinder::unbind() {
    this->midiInput->setChannel(-1);
    this->midiInput->setDeviceId(-1);
    this->midiInput->setDriverId(-1);
    return -1;
}

int PadBinder::bindToMiniLab3(Module *
module) {
MiniLab3 *miniLab3 = dynamic_cast<MiniLab3 *>(
module);

if (miniLab3) {
attachMidi(miniLab3
->midiInput);
return 1;
} else {
return
unbind();
}}

int PadBinder::bindToG8Pad(Module *
module) {
G8Pad *leftPad = dynamic_cast<G8Pad *>(
module);
int padId = leftPad->getId();
if (
leftPad &&padId
>= 0) {
attachMidi(leftPad
->midiInput);
return padId + 1;
} else {
return
unbind();
}}