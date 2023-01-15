#pragma once

#include "../plugin.hpp"
#include "Relay.cpp"

template <typename T>
struct ExpanderRelay {

    Relay<T> connectedRelay;
    Relay<T> disconnectedRelay;

    Model* model;

    bool side = false;

    ExpanderRelay(Model* model, bool side) {
        this->model = model;
        this->side = side;
    }

    void onExpanderChange(const Module::ExpanderChangeEvent& e, Module::Expander leftExpander, Module::Expander rightExpander) {
        if (e.side != side) {
            return;
        }
        if (e.side) {
            if (rightExpander.module && rightExpander.module->model == model) {
                connectedRelay.processMessage((T)(rightExpander.module));
            }
            else {
                disconnectedRelay.processMessage((T)(rightExpander.module));
            }
        } else {
            if (leftExpander.module && leftExpander.module->model == model) {
                connectedRelay.processMessage((T)(leftExpander.module));
            }
            else {
                disconnectedRelay.processMessage((T)(leftExpander.module));
            }
        }
    }
};