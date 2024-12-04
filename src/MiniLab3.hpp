#pragma once
#include <rack.hpp>
#include "BaseModule.hpp"
#include "G8Pad.hpp"
#include "MiniLog.hpp"
#include "consts/midi.hpp"
#include "plugin.hpp"

struct MiniLab3 : BaseModule {
    std::array<AbsoluteParam*, 4> sliders{};

    static const int FirstSlider = 80;

    MiniLab3() : BaseModule() {
        DEBUG("MiniLab3: Starting construction");
        isActive = true;
        lights[STATUS_LIGHT].setBrightness(1.0f);

        // Create sliders
        for (int i = 0; i < 4; i++) {
            sliders[i] = createAbsoluteOutput(
                SLIDER_OUTPUT + i,
                rack::string::f("Slider %d", i + 1),
                [](AbsoluteParam* p) {
                    p->setVoltageMode(VoltageMode::UNIPOLAR_10);
                    p->setRange(0, 127);
                    p->setSlew(0.0f);
                    p->setValue(0);
                }
            );
        }

        DEBUG("MiniLab3: Construction complete");
    }

    void process(const ProcessArgs& args) override {
        auto [nActive, nTotal] = scan();

        auto isActive = nActive == 0;

        if (isActive != this->isActive) {
            this->isActive = isActive;
            lights[STATUS_LIGHT].setBrightness(isActive ? 1.0f : 0.0f);
            gate->send(isActive ? 1 : 0);
        }

        midi::Message msg;
        while (midiInput.tryPop(&msg, args.frame)) {
            if (leftExpander.module) {
                auto log = dynamic_cast<MiniLog*>(leftExpander.module);
                if (log) {
                    log->processMessage(msg);
                }
            }
            processMessage(msg);
        }

        processParams();
    }

   private:
    void processMessage(const midi::Message& msg) {
        auto note = msg.getNote();
        auto value = msg.getValue();
        auto status = msg.getStatus();
        auto channel = msg.getChannel();

        auto isControlChange = status == 0xb;
        auto isControlChannel = channel == ControlChannel;
        auto isNoteOn = status == 0x9;
        auto isNoteOff = status == 0x8;
        auto isNoteChange = isNoteOn || isNoteOff;

        // always process sliders
        if (isControlChange) {
            if (note >= FirstSlider && note <= FirstSlider + 3) {
                int slider = note - FirstSlider;
                sliders[slider]->send(value);
                return;
            }
        }

        if (!isActive) {
            DEBUG("MiniLab3: Not active, skipping message");
            return;
        }

        // never handle pad toggles
        if (isControlChannel && isNoteChange) {
            return;
        }

        switch (status) {
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

    std::tuple<int, int> scan() {
        int nActive = 0;
        int padIndex = 0;  // Start at 1 since MiniLab is 0
        Module* current = rightExpander.module;

        while (current) {
            if (current->model == modelG8Pad) {
                padIndex++;
                auto pad = static_cast<G8Pad*>(current);
                auto myDriverId = midiInput.getDriverId();
                auto myDeviceId = midiInput.getDeviceId();
                auto myChannel = midiInput.getChannel();

                if (myDriverId != pad->driverId) {
                    pad->driverId = myDriverId;
                    DEBUG("MiniLab3: Pad %d MIDI Updated, driver %d", padIndex, pad->driverId);
                }

                if (myDeviceId != pad->deviceId) {
                    pad->deviceId = myDeviceId;
                    DEBUG("MiniLab3: Pad %d MIDI Updated, driver %d, device %d", padIndex, pad->driverId, pad->deviceId);
                }

                if (myChannel != pad->channel) {
                    pad->channel = myChannel;
                    DEBUG("MiniLab3: Pad %d MIDI Updated, driver %d, device %d, channel %d", padIndex, pad->driverId, pad->deviceId, pad->channel);
                }

                if (pad->isActive) {
                    nActive++;
                }
            }

            current = current->rightExpander.module;
        }

        return std::make_tuple(nActive, padIndex);
    }

    void processParams() {
        // Process all parameters
        gate->process();
        velocity->process();
        bend->process();
        mod->process();
        touch->process();

        for (auto& knob : knobs) {
            knob->process();
        }

        for (auto& slider : sliders) {
            slider->process();
        }
    }
};