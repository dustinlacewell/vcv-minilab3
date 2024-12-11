#include "MiniLab.hpp"

#include "MiniPad.hpp"
#include "MiniLog.hpp"
#include "consts/midi.hpp"
#include "widgets/MiniLabWidget.hpp"
#include "utils/getBrightness.hpp"

rack::plugin::Model* modelMiniLab =
    createModel<MiniLab, MiniLabWidget>("MiniLab");

MiniLab::MiniLab() : BaseModule() {
    scanDivider.setDivision(1024);
    midiDivider.setDivision(16);
    paramDivider.setDivision(8);

    isReady = false;
    isActive = true;
    lights[STATUS_LIGHT].setBrightness(1.0f);

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

    gate->send(1);
}

void MiniLab::process(const ProcessArgs& args) {
    if (scanDivider.process()) {
        auto driverId = midiInput.getDriverId();
        auto deviceId = midiInput.getDeviceId();
        auto channel = midiInput.getChannel();

        if (channel == ControlChannel) {
            midiInput.setChannel(-1);
            return;
        }
        
        if (!isReady) {
            if (driverId != -1 && deviceId != -1 && channel != ControlChannel) {
                isReady = true;
            }
        } else {
            if (driverId == -1 || deviceId == -1 || channel == ControlChannel) {
                isReady = false;
            }
        }

        auto [nActive, nTotal] = scan();

        auto isActive = isReady && nActive == 0;

        if (isActive != this->isActive) {
            this->isActive = isActive;
            gate->send(isActive ? 1 : 0);
        }

        auto brightness = getBrightness(isReady, isActive);
        if (lights[STATUS_LIGHT].getBrightness() != brightness) {
            lights[STATUS_LIGHT].setBrightness(brightness);
        }
    }

    if (midiDivider.process()) {
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
    }

    if (paramDivider.process()) {
        processParams();
    }
}

void MiniLab::processMessage(const midi::Message& msg) {
    auto note = msg.getNote();
    auto value = msg.getValue();
    auto status = msg.getStatus();
    auto channel = msg.getChannel();

    auto isControlChange = status == 0xb;
    auto isNoteOn = status == 0x9;
    auto isNoteOff = status == 0x8;
    auto isNoteChange = isNoteOn || isNoteOff;

    // never handle control channel messages
    if (channel == ControlChannel) {
        return;
    }

    // always process sliders
    if (isControlChange) {
        if (isSlider(note)) {
            int index = sliderIndex(note);
            sliders[index]->send(value);
            return;
        }
    }

    if (!isActive) {
        return;
    }

    switch (status) {
        case PitchBend:
            bend->send(value);
            break;
        case KeyPressure:
            touch->send(value);
            break;
        case ControlChange:
            if (note == ModWheel) {
                mod->send(value);
            } else if (isKnob(note)) {
                int index = knobIndex(note);
                knobs[index]->send(value);
            }
            break;
    }
}

std::tuple<int, int> MiniLab::scan() {
    auto myDriverId = midiInput.getDriverId();
    auto myDeviceId = midiInput.getDeviceId();
    auto myChannel = midiInput.getChannel();

    int nActive = 0;
    int padIndex = 0;  // Start at 1 since MiniLab is 0
    Module* current = rightExpander.module;

    while (current) {
        if (current->model == modelMiniPad) {
            padIndex++;
            auto pad = static_cast<MiniPad*>(current);

            if (myDriverId != pad->driverId) {
                pad->driverId = myDriverId;
                pad->dirty = true;
            }

            if (myDeviceId != pad->deviceId) {
                pad->deviceId = myDeviceId;
                pad->dirty = true;
            }

            if (myChannel != pad->channel) {
                pad->channel = myChannel;
                pad->dirty = true;
            }

            if (pad->isActive) {
                nActive++;
            }
        }

        current = current->rightExpander.module;
    }

    return std::make_tuple(nActive, padIndex);
}

void MiniLab::processParams() {
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
