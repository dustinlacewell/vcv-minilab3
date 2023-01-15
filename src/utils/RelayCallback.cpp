#pragma once

#include <rack.hpp>
#include <utils/RelayCallback.hpp>
#include <utils/Relay.hpp>


template <typename T>
RelayCallback<T>::RelayCallback(std::function<void(T)> callback) {
    this->callback = callback;
}


template <typename T>
void RelayCallback<T>::registerCallback(Relay<T>* relay) {
    this->relay = relay;
    this->handle = relay->registerCallback(callback);
}

template <typename T>
void RelayCallback<T>::unregisterCallback() {
    if (handle >= 0 && relay) {
        this->relay->unregisterCallback(handle);
        this->handle = -1;
    }
}