#pragma once

#include <rack.hpp>
#include "menu/AbsoluteParamMenu.hpp"
#include "menu/RelativeParamMenu.hpp"
#include "params/AbsoluteParam.hpp"
#include "params/RelativeParam.hpp"
#include "plugin.hpp"
#include "ui/FancyPanelBorder.hpp"
#include "ui/OutputPort.hpp"
#include "utils/paintToColor.hpp"

using namespace rack;

template <typename T, typename W>
struct BaseWidget : ModuleWidget, SvgHelper<W> {
    FancyPanelBorder* fancyPanelBorder;

    void draw(const DrawArgs& args) override {
        if (module) {
            DrawArgs newDrawArgs = args;
            if (!fancyPanelBorder->rightBorder) {
                newDrawArgs.clipBox.size.x += 2;
            }
            ModuleWidget::draw(newDrawArgs);
        } else {
            ModuleWidget::draw(args);
        }
    }

    void loadPanel(std::string filename) {
        SvgHelper<W>::loadPanel(asset::plugin(pluginInstance, filename));

        auto panel = dynamic_cast<SvgPanel*>(getPanel());

        if (panel) {
            panel->panelBorder->hide();
            panel->fb->box.size.x += 2;
            fancyPanelBorder = new FancyPanelBorder;
            fancyPanelBorder->box.size = panel->fb->box.size;
            panel->fb->addChild(fancyPanelBorder);
        }
    }

    OutputPort* createAbsolutePort(
        Vec pos,
        T* module,
        int outputId,
        std::function<AbsoluteParam*(T*)> getParam
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
        std::function<RelativeParam*(T*)> getParam
    );

    void createRelativePort(
        std::string elementName,
        T* module,
        int outputId,
        std::function<RelativeParam*(T*)> getParam
    );

    void updateBorders(
        std::vector<plugin::Model*> leftModels = {},
        std::vector<plugin::Model*> rightModels = {}
    ) {
        T* myModule = dynamic_cast<T*>(module);

        if (!myModule) {
            return;
        }

        if (!fancyPanelBorder) {
            DEBUG("updateBorders: no fancyPanelBorder");
            return;
        }

        auto panel = dynamic_cast<SvgPanel*>(getPanel());

        if (!panel) {
            DEBUG("updateBorders: no panel");
            return;
        }

        if (!leftModels.empty()) {
            auto leftExpander = myModule->leftExpander.module;
            auto leftIsGood = false;

            if (leftExpander) {
                for (auto leftModel : leftModels) {
                    if (leftExpander->model == leftModel) {
                        leftIsGood = true;
                        break;
                    }
                }
            }

            if (leftIsGood) {
                if (fancyPanelBorder->leftBorder) {
                    fancyPanelBorder->leftBorder = false;
                    panel->fb->setDirty();
                }
            } else {
                if (!fancyPanelBorder->leftBorder) {
                    fancyPanelBorder->leftBorder = true;
                    panel->fb->setDirty();
                }
            }
        }

        if (!rightModels.empty()) {
            auto rightExpander = myModule->rightExpander.module;
            auto rightIsGood = false;

            if (rightExpander) {
                for (auto rightModel : rightModels) {
                    if (rightExpander->model == rightModel) {
                        rightIsGood = true;
                        break;
                    }
                }
            }

            if (rightIsGood) {
                if (fancyPanelBorder->rightBorder) {
                    fancyPanelBorder->rightBorder = false;
                    panel->fb->setDirty();
                }
            } else {
                if (!fancyPanelBorder->rightBorder) {
                    fancyPanelBorder->rightBorder = true;
                    panel->fb->setDirty();
                }
            }
        }
    }

    void onReset();
};

template <typename T, typename W>
OutputPort* BaseWidget<T, W>::createAbsolutePort(
    Vec pos,
    T* module,
    int outputId,
    std::function<AbsoluteParam*(T*)> getParam
) {
    auto* port = createOutputCentered<OutputPort>(pos, module, outputId);

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

    createAbsolutePort(*posMaybe, module, outputId, getParam);
}

template <typename T, typename W>
OutputPort* BaseWidget<T, W>::createRelativePort(
    Vec pos,
    T* module,
    int outputId,
    std::function<RelativeParam*(T*)> getParam
) {
    auto* port = createOutputCentered<OutputPort>(pos, module, outputId);

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

    createRelativePort(*posMaybe, module, outputId, getParam);
}