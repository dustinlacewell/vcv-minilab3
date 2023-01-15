#pragma once

#include <rack.hpp>
#include "Relay.hpp"


template <typename T>
struct RelayCallback {

    int handle = -1;

    std::function<void(T)> callback;

    Relay<T>* relay;

    RelayCallback(std::function<void(T)> callback);

    void registerCallback(Relay<T>* relay);
    void unregisterCallback();
};