#pragma once
#include <rack.hpp>
#include "BaseModule.hpp"
#include "consts/midi.hpp"

struct G8Pad : BaseModule {
    int driverId = -1;
    int deviceId = -1;
    int channel = -1;

    void process(const ProcessArgs& args) override {
        int currentDriverId = midiInput.getDriverId();
        int currentDeviceId = midiInput.getDeviceId();
        int currentChannel = midiInput.getChannel();

        if (driverId != currentDriverId) {
            DEBUG("G8Pad: MIDI driver changed to %d", driverId);
            midiInput.setDriverId(driverId);
        }

        if (deviceId != currentDeviceId) {
            DEBUG("G8Pad: MIDI device changed to %d", deviceId);
            midiInput.setDeviceId(deviceId);
        }

        if (channel != currentChannel) {
            DEBUG("G8Pad: MIDI channel changed to %d", channel);
            midiInput.setChannel(channel);
        }

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


        midi::Message msg;
        while (midiInput.tryPop(&msg, args.frame)) {
            processMessage(msg);
        }

        processParams();
    }

   private:
    int getPadIdFromNote(int note) const {
        switch (note) {
            case 36:
                return 1;
            case 48:
                return 2;
            case 60:
                return 3;
            case 72:
                return 4;
            case 84:
                return 5;
            case 96:
                return 6;
            case 108:
                return 7;
            case 120:
                return 8;
            default:
                return -1;
        }
    }

    void processMessage(const midi::Message& msg) {
        auto note = msg.getNote();
        auto value = msg.getValue();
        auto status = msg.getStatus();
        auto channel = msg.getChannel();

        auto isControlChannel = channel == ControlChannel;
        auto isNoteOn = status == 0x9;
        auto isNoteOff = status == 0x8;
        auto isNoteChange = isNoteOn || isNoteOff;

        // DEBUG ALL
        DEBUG("MiniLab3: Note %d, Value %d, Status %d, Channel %d", note, value, status, channel);
        DEBUG("MiniLab3: isControlChannel %d, isNoteOn %d, isNoteOff %d, isNoteChange %d", isControlChannel, isNoteOn, isNoteOff, isNoteChange);

        // always process pad toggles
        if (isControlChannel && isNoteChange) {
            int padId = getPadIdFromNote(note);

            if (padId == -1 || padId != position) {
                return;
            }

            if (isNoteOn) {
                if (!isActive) {
                    DEBUG("MiniLab3: Pad %d ON", padId);
                    isActive = true;
                    lights[STATUS_LIGHT].setBrightness(1.0f);
                    gate->send(1);
                    velocity->send(value);
                }
            } else if (isNoteOff) {
                if (isActive) {
                    DEBUG("MiniLab3: Pad %d OFF", padId);
                    isActive = false;
                    lights[STATUS_LIGHT].setBrightness(0.0f);
                    gate->send(0);
                }
            }

            return;
        }

        if (!isActive) {
            DEBUG("MiniLab3: Not active, skipping message");
            return;
        }

        switch (msg.getStatus()) {
            case 0xe:  // Pitch Bend
                bend->send(value);
                break;
            case 0xa:  // Aftertouch
                touch->send(value);
                break;
            case 0xb:  // Control Change
                if (note == ModWheel) {
                    mod->send(value);
                } else if (note >= FirstKnob && note <= FirstKnob + 7) {
                    int knob = note - FirstKnob;
                    knobs[knob]->send(value);
                }
                break;
        }
    }
};