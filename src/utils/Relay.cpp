#pragma once

#include <rack.hpp>
#include <utils/Relay.hpp>


template <typename T>
int Relay<T>::registerCallback(std::function<void(T)> callback) {
    int handle = nextHandle++;
    callbacks[handle] = callback;
    return handle;
}

template <typename T>
void Relay<T>::unregisterCallback(int handle) {
    if (callbacks.find(handle) != callbacks.end()) {
        callbacks.erase(handle);
    }
}

template <typename T>
void Relay<T>::processMessage(T msg) {
    for (auto& it : callbacks) {
        it.second(msg);
    }
}