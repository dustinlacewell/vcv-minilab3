#include "BaseModule.hpp"

#include <utility>
#include "params/AbsoluteParam.hpp"
#include "params/RelativeParam.hpp"

BaseModule::BaseModule() {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    configInput(MIDI_INPUT, "MIDI");
    configLight(STATUS_LIGHT, "Status");

    gate = createAbsoluteOutput(GATE_OUTPUT, "Gate", [](AbsoluteParam* p) {
        p->setSlew(0.0f);
        p->setRange(0, 1);
        p->setVoltageMode(VoltageMode::UNIPOLAR_5);
        p->setValue(0);
    });
    velocity =
        createAbsoluteOutput(VELOCITY_OUTPUT, "Velocity", [](AbsoluteParam* p) {
            p->setSlew(0.0f);
            p->setRange(0, 127);
            p->setVoltageMode(VoltageMode::UNIPOLAR_10);
            p->setValue(0);
        });
    bend =
        createAbsoluteOutput(BEND_OUTPUT, "Pitch Bend", [](AbsoluteParam* p) {
            p->setSlew(0.0f);
            p->setRange(0, 127);
            p->setVoltageMode(VoltageMode::BIPOLAR_5);
            p->setValue(64);
        });
    mod = createAbsoluteOutput(MOD_OUTPUT, "Modulation", [](AbsoluteParam* p) {
        p->setSlew(0.0f);
        p->setRange(0, 127);
        p->setVoltageMode(VoltageMode::UNIPOLAR_10);
        p->setValue(0);
    });
    touch =
        createAbsoluteOutput(TOUCH_OUTPUT, "Aftertouch", [](AbsoluteParam* p) {
            p->setSlew(0.0f);
            p->setRange(0, 127);
            p->setVoltageMode(VoltageMode::UNIPOLAR_10);
            p->setValue(0);
        });

    for (int i = 0; i < 8; i++) {
        knobs[i] = createRelativeOutput(
            KNOB_OUTPUT + i,
            rack::string::f("Knob %d", i + 1),
            [i](RelativeParam* p) {
                p->setStrength(Strength::FivePercent);
                p->setResolution(Resolution::Medium);
                p->setVoltageMode(VoltageMode::UNIPOLAR_10);
                p->setValue(0);
                p->setSlew(0.0f);
            }
        );
    }
}

AbsoluteParam* BaseModule::createAbsoluteOutput(int output, std::string label) {
    configOutput(output, label);
    auto param = std::make_unique<AbsoluteParam>(label, &outputs[output]);
    auto raw = param.get();
    params.push_back(std::move(param));
    return raw;
}

AbsoluteParam* BaseModule::createAbsoluteOutput(
    int output,
    std::string label,
    std::function<void(AbsoluteParam*)> setupCallback
) {
    auto param = createAbsoluteOutput(output, std::string(label));
    setupCallback(param);
    param->save();
    return param;
}

RelativeParam* BaseModule::createRelativeOutput(int output, std::string label) {
    configOutput(output, label);
    auto param = std::make_unique<RelativeParam>(label, &outputs[output]);
    auto raw = param.get();
    params.push_back(std::move(param));
    return raw;
}

RelativeParam* BaseModule::createRelativeOutput(
    int output,
    std::string label,
    std::function<void(RelativeParam*)> setupCallback
) {
    auto param = createRelativeOutput(output, std::string(label));
    setupCallback(param);
    param->save();
    return param;
}

void BaseModule::processParams() {
    for (const auto& param : params) {
        param->process();
    }
}

void BaseModule::outputsToJson(json_t* rootJ) {
    for (const auto& param : params) {
        json_object_set_new(rootJ, param->getName().c_str(), param->toJson());
    }
}

void BaseModule::outputsFromJson(json_t* rootJ) {
    for (const auto& param : params) {
        json_t* paramJ = json_object_get(rootJ, param->getName().c_str());
        if (paramJ) {
            param->fromJson(paramJ);
        }
    }
}

void BaseModule::onReset() {
    for (const auto& param : params) {
        param->load();
    }
}

json_t* BaseModule::dataToJson() {
    json_t* rootJ = json_object();

    if (!rootJ)
        return nullptr;

    outputsToJson(rootJ);

    auto driverId = midiInput.getDriverId();
    auto deviceId = midiInput.getDeviceId();
    auto channel = midiInput.getChannel();

    json_object_set_new(rootJ, "midiDriver", json_integer(driverId));
    json_object_set_new(rootJ, "midiDevice", json_integer(deviceId));
    json_object_set_new(rootJ, "midiChannel", json_integer(channel));

    return rootJ;
}

void BaseModule::dataFromJson(json_t* rootJ) {
    outputsFromJson(rootJ);

    if (json_t* midiDriverJ = json_object_get(rootJ, "midiDriver")) {
        auto driver = (int)json_integer_value(midiDriverJ);
        midiInput.setDriverId(driver);
    }
    if (json_t* midiDeviceJ = json_object_get(rootJ, "midiDevice")) {
        auto device = (int)json_integer_value(midiDeviceJ);
        midiInput.setDeviceId(device);
    }
    if (json_t* midiChannelJ = json_object_get(rootJ, "midiChannel")) {
        auto channel = (int)json_integer_value(midiChannelJ);
        midiInput.setChannel(channel);
    }
}