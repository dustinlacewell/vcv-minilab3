#include "widgets/MiniLogWidget.hpp"
#include "MiniLogWidget.hpp"

MiniLogWidget::MiniLogWidget(MiniLog* module)
    : BaseWidget<MiniLog, MiniLogWidget>() {
    setModule(module);
    loadPanel("res/Log.svg");

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
    if (!log) {
        return;
    }

    updateBorders({nullptr}, {modelMiniLab});
}

void MiniLogWidget::createStatusLight(MiniLog* module) {
    addChild(createLightCentered<SmallLight<GreenLight>>(
        *findNamed("Light"), module, MiniLog::STATUS_LIGHT
    ));
}

void MiniLogWidget::createLogWidget(MiniLog* module) {
    log = createWidget<TextLogWidget>(Vec(4, 34.5));
    log->box.size = Vec(112, 338);
    addChild(log);
}