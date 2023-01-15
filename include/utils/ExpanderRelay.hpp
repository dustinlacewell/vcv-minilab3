#pragma once

#include <rack.hpp>
#include <utils/Relay.hpp>

using namespace rack;


template <typename T>
struct ExpanderRelay {

    Relay<T> connectedRelay;
    Relay<T> disconnectedRelay;

    Model* model;

    bool side = false;

    ExpanderRelay(Model* model, bool side);
    void onExpanderChange(const Module::ExpanderChangeEvent& e, Module::Expander leftExpander, Module::Expander rightExpander);
};