#include "widgets/MiniLogWidget.hpp"
#include "MiniLogWidget.hpp"

MiniLogWidget::MiniLogWidget(MiniLog* module)
    : BaseWidget<MiniLog, MiniLogWidget>() {
    setModule(module);
    loadPanel("res/MiniLog.svg");

    createStatusLight(module);
    createLogWidget(module);

    if (module) {
        module->whenReinit([this]() {
            log->clear();
            dirty = true;
        });
    }
}

void MiniLogWidget::step() {
    ModuleWidget::step();

    if (!module) {
        return;
    }

    auto* miniLog = reinterpret_cast<MiniLog*>(this->module);
    while (!miniLog->messages.empty()) {
        std::string s = miniLog->messages.shift();
        log->push(s);
    }

    auto* log = dynamic_cast<MiniLog*>(module);
    auto _panel = dynamic_cast<SvgPanel*>(getPanel());

    if (!log) {
        return;
    }

    updateBorders({nullptr}, {modelMiniLab});
}

void MiniLogWidget::createStatusLight(MiniLog* module) {
    addChild(createLightCentered<SmallLight<GreenLight>>(
        findNamed("Light").value(), module, MiniLog::STATUS_LIGHT
    ));
}

void MiniLogWidget::createLogWidget(MiniLog* module) {
    log = createWidget<TextLogWidget>(Vec(4, 34.5));
    log->box.size = Vec(112, 338);
    addChild(log);
}