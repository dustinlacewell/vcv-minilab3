#pragma once

#include <rack.hpp>
#include <ui/OutputPort.hpp>
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