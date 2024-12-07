#include "MiniPad.hpp"
#include "consts/midi.hpp"
#include "plugin.hpp"
#include "utils/padNotes.hpp"
#include "widgets/MiniPadWidget.hpp"
#include "utils/getBrightness.hpp"

rack::plugin::Model* modelMiniPad =
    createModel<MiniPad, MiniPadWidget>("MiniPad");

MiniPad::MiniPad() : BaseModule() {
    driverDivider.setDivision(4096);
    positionDivider.setDivision(1024);
    midiDivider.setDivision(16);
    paramDivider.setDivision(256);
}

void MiniPad::process(const ProcessArgs& args) {
    if (positionDivider.process()) {
        Module* current = leftExpander.module;

        int pos = 0;
        bool found = false;
        bool labReady = false;

        // auto driverId = midiInput.getDriverId();
        // auto deviceId = midiInput.getDeviceId();
        // auto channel = midiInput.getChannel();

        // if (!isReady) {
        //     if (driverId != -1 && deviceId != -1 && channel != 15) {
        //         isReady = true;
        //     }
        // } else {
        //     if (driverId == -1 || deviceId == -1 || channel == 15) {
        //         isReady = false;
        //     }
        // }

        while (current) {
            if (current->model == modelMiniPad) {
                pos++;
            } else if (current->model == modelMiniLab) {
                pos++;
                found = true;
                labReady = static_cast<MiniLab*>(current)->isReady;
                break;
            }

            current = current->leftExpander.module;
        }

        if (found) {
            position = pos;
        } else {
            position = -1;
            isActive = false;
            gate->send(0);
            return;
        }

        auto brightness = getBrightness(labReady, isActive);

        if (lights[STATUS_LIGHT].getBrightness() != brightness) {
            lights[STATUS_LIGHT].setBrightness(brightness);
        }
    }

    if (driverDivider.process() && dirty && position != -1) {
        dirty = false;
        int currentDriverId = midiInput.getDriverId();
        int currentDeviceId = midiInput.getDeviceId();
        int currentChannel = midiInput.getChannel();

        if (driverId != currentDriverId) {
            DEBUG(
                "MiniPad: MIDI driver changed to %d, was %d",
                driverId,
                currentDriverId
            );
            midiInput.setDriverId(driverId);
            DEBUG("MiniPad: MIDI driver now: %d", midiInput.getDriverId());
        }

        if (deviceId != currentDeviceId) {
            DEBUG(
                "MiniPad: MIDI device changed to %d was %d",
                deviceId,
                currentDeviceId
            );
            midiInput.setDeviceId(deviceId);
            DEBUG("MiniPad: MIDI device now: %d", midiInput.getDeviceId());
        }

        if (channel != currentChannel) {
            DEBUG(
                "MiniPad: MIDI channel changed to %d was %d",
                channel,
                currentChannel
            );
            midiInput.setChannel(channel);
            DEBUG("MiniPad: MIDI channel now: %d", midiInput.getChannel());
        }
    }

    if (midiDivider.process()) {
        midi::Message msg;
        while (midiInput.tryPop(&msg, args.frame)) {
            processMessage(msg);
        }
    }

    if (paramDivider.process()) {
        processParams();
    }
}

void MiniPad::processMessage(const midi::Message& msg) {
    auto note = msg.getNote();
    auto value = msg.getValue();
    auto status = msg.getStatus();
    auto channel = msg.getChannel();

    auto isControlChannel = channel == ControlChannel;
    auto isNoteOn = status == 0x9;
    auto isNoteOff = status == 0x8;
    auto isNoteChange = isNoteOn || isNoteOff;

    int padId = padForNote(note);

    if (isControlChannel && isNoteChange) {
        if (padId == -1 || padId != position) {
            return;
        }

        if (isNoteOn) {
            if (!isActive) {
                isActive = true;
                gate->send(1);
                velocity->send(value);
            }
        } else if (isNoteOff) {
            if (isActive) {
                isActive = false;
                gate->send(0);
            }
        }

        return;
    }

    if (!isActive) {
        return;
    }

    switch (msg.getStatus()) {
        case PitchBend:
            bend->send(value);
            break;
        case KeyPressure:
            if (padId == position)
                touch->send(value);
            break;
        case ControlChange:
            if (note == ModWheel) {
                mod->send(value);
            } else if (isKnob(note)) {
                int knob = knobIndex(note);
                knobs[knob]->send(value);
            }
            break;
    }
}
