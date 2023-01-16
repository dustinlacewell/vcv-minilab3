#pragma once

#include <rack.hpp>
#include <MiniLab3.hpp>
#include <utils/ExpanderRelay.hpp>
#include <utils/Relay.hpp>

using namespace rack;


template <typename T>
ExpanderRelay<T>::ExpanderRelay(Model* model, bool side) {
    this->model = model;
    this->side = side;
}

template <typename T>
void ExpanderRelay<T>::onExpanderChange(const Module::ExpanderChangeEvent& e, Module::Expander leftExpander, Module::Expander rightExpander) {
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
};

template class ExpanderRelay<MiniLab3*>;