#include "MiniLab3.hpp"

#include "G8Pad.hpp"
#include "MiniLog.hpp"
#include "consts/midi.hpp"
#include "widgets/MiniLab3Widget.hpp"

rack::plugin::Model* modelMiniLab3 =
    createModel<MiniLab3, MiniLab3Widget>("MiniLab3");

MiniLab3::MiniLab3() : BaseModule() {
    scanDivider.setDivision(1024);
    midiDivider.setDivision(16);
    paramDivider.setDivision(8);

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
}

void MiniLab3::process(const ProcessArgs& args) {
    if (scanDivider.process()) {
        auto [nActive, nTotal] = scan();

        auto isActive = nActive == 0;

        if (isActive != this->isActive) {
            this->isActive = isActive;
            lights[STATUS_LIGHT].setBrightness(isActive ? 1.0f : 0.0f);
            gate->send(isActive ? 1 : 0);
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

void MiniLab3::processMessage(const midi::Message& msg) {
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
        if (isSlider(note)) {
            int index = sliderIndex(note);
            sliders[index]->send(value);
            return;
        }
    }

    if (!isActive) {
        return;
    }

    // never handle pad toggles
    if (isControlChannel && isNoteChange) {
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

std::tuple<int, int> MiniLab3::scan() {
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

void MiniLab3::processParams() {
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
