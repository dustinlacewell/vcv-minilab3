#pragma once

#include <rack.hpp>
#include "menu/AbsoluteParamMenu.hpp"
#include "menu/RelativeParamMenu.hpp"
#include "params/AbsoluteParam.hpp"
#include "params/RelativeParam.hpp"
#include "ui/OutputPort.hpp"

using namespace rack;

template <typename T, typename W>
struct BaseWidget : ModuleWidget, SvgHelper<W> {

    OutputPort* createAbsolutePort(
        Vec pos,
        T* module,
        int outputId,
        std::function<AbsoluteParam*(T*)> getParam,
        bool convertToPx = true
    );

    void createAbsolutePort(
        std::string elementName,
        T* module,
        int outputId,
        std::function<AbsoluteParam*(T*)> getParam
    );

    OutputPort* createRelativePort(
        Vec pos,
        T* module,
        int outputId,
        std::function<RelativeParam*(T*)> getParam,
        bool convertToPx = true
    );

    void createRelativePort(
        std::string elementName,
        T* module,
        int outputId,
        std::function<RelativeParam*(T*)> getParam
    );

    void onReset();
};

template <typename T, typename W>
OutputPort* BaseWidget<T, W>::createAbsolutePort(
    Vec pos,
    T* module,
    int outputId,
    std::function<AbsoluteParam*(T*)> getParam,
    // optional boolean of whether to convert to px
    bool convertToPx
) {
    auto* port = createOutputCentered<OutputPort>(
        convertToPx ? mm2px(pos) : pos, module, outputId
    );

    if (module) {
        auto* param = getParam(module);
        port->contextMenus.push_back(new AbsoluteParamMenu(param));
        port->setParam(param);
    }

    addOutput(port);
    return port;
}

template <typename T, typename W>
void BaseWidget<T, W>::createAbsolutePort(
    std::string elementName,
    T* module,
    int outputId,
    std::function<AbsoluteParam*(T*)> getParam
) {
    auto posMaybe = SvgHelper<W>::findNamed(elementName);

    if (!posMaybe.has_value()) {
        DEBUG("No %s position found", elementName.c_str());
        return;
    }

    createAbsolutePort(
        posMaybe.value(),
        module,
        outputId,
        getParam,
        false
    );
}

template <typename T, typename W>
OutputPort* BaseWidget<T, W>::createRelativePort(
    Vec pos,
    T* module,
    int outputId,
    std::function<RelativeParam*(T*)> getParam,
    // optional boolean of whether to convert to px
    bool convertToPx
) {
    auto* port = createOutputCentered<OutputPort>(
        convertToPx ? mm2px(pos) : pos, module, outputId
    );

    if (module) {
        RelativeParam* param = getParam(module);
        port->contextMenus.push_back(new RelativeParamMenu(param));
        port->setParam(param);
    }

    addOutput(port);
    return port;
}

template <typename T, typename W>
void BaseWidget<T, W>::createRelativePort(
    std::string elementName,
    T* module,
    int outputId,
    std::function<RelativeParam*(T*)> getParam
) {
    auto posMaybe = SvgHelper<W>::findNamed(elementName);

    if (!posMaybe.has_value()) {
        DEBUG("No %s position found", elementName.c_str());
        return;
    }

    createRelativePort(
        posMaybe.value(),
        module,
        outputId,
        getParam,
        false
    );
}