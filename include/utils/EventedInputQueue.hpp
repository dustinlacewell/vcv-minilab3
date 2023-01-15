#pragma once

#include <rack.hpp>

#include "Relay.hpp"

using namespace rack;


struct EventedInputQueue : midi::InputQueue {

    Relay<bool> deviceStatusRelay;

    void setDeviceId(int deviceId) override;
};