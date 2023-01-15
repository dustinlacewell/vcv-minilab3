#pragma once

#include "../plugin.hpp"

template <typename T>
struct Relay {
    int nextHandle = 0;

    std::map<int, std::function<void(T)>> callbacks;

    int registerCallback(std::function<void(T)> callback) {
        int handle = nextHandle++;
        callbacks[handle] = callback;
        return handle;
    }

    void unregisterCallback(int handle) {
        if (callbacks.find(handle) != callbacks.end()) {
            callbacks.erase(handle);
        }
    }

    void processMessage(T msg) {
        for (auto& it : callbacks) {
            it.second(msg);
        }
    }
};