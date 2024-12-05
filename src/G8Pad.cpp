#include "G8Pad.hpp"
#include "consts/midi.hpp"
#include "plugin.hpp"
#include "utils/padNotes.hpp"
#include "widgets/G8PadWidget.hpp"

rack::plugin::Model* modelG8Pad = createModel<G8Pad, G8PadWidget>("G8Pad");

G8Pad::G8Pad() : BaseModule() {
    driverDivider.setDivision(4096);
    positionDivider.setDivision(1024);
    midiDivider.setDivision(16);
    paramDivider.setDivision(256);
}

void G8Pad::process(const ProcessArgs& args) {
    if (positionDivider.process()) {
        Module* current = leftExpander.module;

        int pos = 0;
        bool found = false;
        while (current) {
            if (current->model == modelG8Pad) {
                pos++;
            } else if (current->model == modelMiniLab3) {
                pos++;
                found = true;
                break;
            }

            current = current->leftExpander.module;
        }

        if (found) {
            position = pos;
        } else {
            position = -1;
            isActive = false;
            lights[STATUS_LIGHT].setBrightness(0.0f);
            gate->send(0);
            return;
        }
    }

    if (driverDivider.process() && dirty && position != -1) {
        dirty = false;
        int currentDriverId = midiInput.getDriverId();
        int currentDeviceId = midiInput.getDeviceId();
        int currentChannel = midiInput.getChannel();

        if (driverId != currentDriverId) {
            DEBUG(
                "G8Pad: MIDI driver changed to %d, was %d",
                driverId,
                currentDriverId
            );
            midiInput.setDriverId(driverId);
            DEBUG("G8Pad: MIDI driver now: %d", midiInput.getDriverId());
        }

        if (deviceId != currentDeviceId) {
            DEBUG(
                "G8Pad: MIDI device changed to %d was %d",
                deviceId,
                currentDeviceId
            );
            midiInput.setDeviceId(deviceId);
            DEBUG("G8Pad: MIDI device now: %d", midiInput.getDeviceId());
        }

        if (channel != currentChannel) {
            DEBUG(
                "G8Pad: MIDI channel changed to %d was %d",
                channel,
                currentChannel
            );
            midiInput.setChannel(channel);
            DEBUG("G8Pad: MIDI channel now: %d", midiInput.getChannel());
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

void G8Pad::processMessage(const midi::Message& msg) {
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
                lights[STATUS_LIGHT].setBrightness(1.0f);
                gate->send(1);
                velocity->send(value);
            }
        } else if (isNoteOff) {
            if (isActive) {
                isActive = false;
                lights[STATUS_LIGHT].setBrightness(0.0f);
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
