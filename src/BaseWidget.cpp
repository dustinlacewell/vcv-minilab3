#include "BaseWidget.hpp"
#include "G8Pad.hpp"
#include "MiniLab3.hpp"
#include "menu/AbsoluteParamMenu.hpp"
#include "menu/RelativeParamMenu.hpp"

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

template struct BaseWidget<G8Pad>;
template struct BaseWidget<MiniLab3>;