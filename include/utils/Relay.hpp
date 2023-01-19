#pragma once

#include <rack.hpp>


template<typename T>
struct Relay {
    int nextHandle = 0;

    std::map<int, std::function<void(T)>> callbacks;

    int registerCallback(std::function<void(T)> callback);
    void unregisterCallback(int handle);
    void processMessage(T msg);
};