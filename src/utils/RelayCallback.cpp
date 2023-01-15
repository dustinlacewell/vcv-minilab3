#pragma once

#include "../plugin.hpp"
#include "Relay.cpp"

template <typename T>
struct RelayCallback {

    int handle = -1;

    std::function<void(T)> callback;

    Relay<T>* relay;

    void registerCallback(Relay<T>* relay) {
        this->relay = relay;
        this->handle = relay->registerCallback(callback);
    }

    void unregisterCallback() {
        if (handle >= 0 && relay) {
            this->relay->unregisterCallback(handle);
            this->handle = -1;
        }
    }

    RelayCallback(std::function<void(T)> callback) : callback(callback) {}
};