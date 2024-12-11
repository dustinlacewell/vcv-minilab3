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
            midiInput.setDriverId(driverId);
            midiControl.setDriverId(driverId);
        }

        if (deviceId != currentDeviceId) {
            midiInput.setDeviceId(deviceId);
            midiControl.setDeviceId(deviceId);
        }

        if (channel != currentChannel) {
            midiInput.setChannel(channel);
            midiControl.setChannel(ControlChannel);
        }
    }

    if (midiDivider.process()) {
        midi::Message msg;

        while (midiControl.tryPop(&msg, args.frame)) {
            processControl(msg);
        }

        while (midiInput.tryPop(&msg, args.frame)) {
            processMessage(msg);
        }
    }

    if (paramDivider.process()) {
        processParams();
    }
}

void MiniPad::processControl(const midi::Message& msg) {
    DEBUG("MiniPad::processControl");
    auto channel = msg.getChannel();

    if (channel != ControlChannel) {
        DEBUG("MiniPad::processControl: channel != ControlChannel");
        return;
    }

    auto note = msg.getNote();
    auto value = msg.getValue();
    auto status = msg.getStatus();    

    auto isNoteOn = status == 0x9;
    auto isNoteOff = status == 0x8;
    auto isNoteChange = isNoteOn || isNoteOff;

    int padId = padForNote(note);
    DEBUG("MiniPad::processControl: padId = %d", padId);

    if (isNoteChange) {
        if (padId == -1 || padId != position) {
            DEBUG("MiniPad::processControl: padId: %d != %d", padId, position);
            return;
        }

        if (isNoteOn) {
            DEBUG("MiniPad::processControl: isNoteOn");
            if (!isActive) {
                DEBUG("MiniPad::processControl: setting active");
                isActive = true;
                gate->send(1);
                velocity->send(value);
            }
        } else if (isNoteOff) {
            DEBUG("MiniPad::processControl: isNoteOff");
            if (isActive) {
                DEBUG("MiniPad::processControl: setting inactive");
                isActive = false;
                gate->send(0);
            }
        }

        return;
    }
}

void MiniPad::processMessage(const midi::Message& msg) {
    auto channel = msg.getChannel();

    if (channel == ControlChannel) {
        return;
    }

    if (!isActive) {
        return;
    }

    auto note = msg.getNote();
    auto value = msg.getValue();
    auto status = msg.getStatus();
    int padId = padForNote(note);    

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

void MiniPad::dataFromJson(json_t* rootJ) {
    BaseModule::dataFromJson(rootJ);

    driverId = midiInput.getDriverId();
    deviceId = midiInput.getDeviceId();    

    midiControl.setDriverId(driverId);
    midiControl.setDeviceId(deviceId);
    midiControl.setChannel(ControlChannel);
}