#include <rack.hpp>

#include <utils/EventedInputQueue.hpp>
#include <utils/Relay.hpp>

using namespace rack;


void EventedInputQueue::setDeviceId(int deviceId) {
    InputQueue::setDeviceId(deviceId);
    deviceStatusRelay.processMessage(deviceId >= 0);
}