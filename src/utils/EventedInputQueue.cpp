#include <rack.hpp>

#include "EventedInputQueue.hpp"
#include "Relay.hpp"

using namespace rack;

void EventedInputQueue::setDeviceId(int deviceId) {
    InputQueue::setDeviceId(deviceId);
    deviceStatusRelay.processMessage(deviceId >= 0);
}