#pragma once

#include <rack.hpp>
#include "menu/AbsoluteParamMenu.hpp"
#include "menu/RelativeParamMenu.hpp"
#include "ui/OutputPort.hpp"
#include "utils/AbsoluteParam.hpp"
#include "utils/RelativeParam.hpp"

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
        auto* param = getParam(module);
        port->contextMenus.push_back(new RelativeParamMenu(param));
    }

    addOutput(port);
    return port;
}