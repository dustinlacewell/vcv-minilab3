#pragma once

#include <rack.hpp>
#include "menu/AbsoluteParamMenu.hpp"
#include "menu/RelativeParamMenu.hpp"
#include "params/AbsoluteParam.hpp"
#include "params/RelativeParam.hpp"
#include "ui/OutputPort.hpp"

using namespace rack;

template <typename T>
struct BaseWidget : ModuleWidget {

    OutputPort* createAbsolutePort(
        Vec pos,
        T* module,
        int outputId,
        const std::function<AbsoluteParam*(T*)>& getParam
    );

    OutputPort* createRelativePort(
        Vec pos,
        T* module,
        int outputId,
        const std::function<RelativeParam*(T*)>& getParam
    );

    void onReset();
};

template <typename T>
OutputPort* BaseWidget<T>::createAbsolutePort(
    Vec pos,
    T* module,
    int outputId,
    const std::function<AbsoluteParam*(T*)>& getParam
) {
    auto* port = createOutputCentered<OutputPort>(mm2px(pos), module, outputId);

    if (module) {
        auto* param = getParam(module);
        port->contextMenus.push_back(new AbsoluteParamMenu(param));
        port->setParam(param);
    }

    addOutput(port);
    return port;
}

template <typename T>
OutputPort* BaseWidget<T>::createRelativePort(
    Vec pos,
    T* module,
    int outputId,
    const std::function<RelativeParam*(T*)>& getParam
) {
    auto* port = createOutputCentered<OutputPort>(mm2px(pos), module, outputId);

    if (module) {
        RelativeParam* param = getParam(module);
        port->contextMenus.push_back(new RelativeParamMenu(param));
        port->setParam(param);
    }

    addOutput(port);
    return port;
}