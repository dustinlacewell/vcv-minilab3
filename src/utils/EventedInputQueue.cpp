#include "../plugin.hpp"

#include "Relay.cpp"

struct EventedInputQueue : midi::InputQueue {

    Relay<bool> deviceStatusRelay;

    void setDeviceId(int deviceId) override {
        InputQueue::setDeviceId(deviceId);
        deviceStatusRelay.processMessage(deviceId >= 0);
    }
};