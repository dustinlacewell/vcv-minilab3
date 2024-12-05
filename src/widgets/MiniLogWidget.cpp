#include "widgets/MiniLogWidget.hpp"
#include "MiniLogWidget.hpp"

MiniLogWidget::MiniLogWidget(MiniLog* module) : BaseWidget<MiniLog, MiniLogWidget>() {
    setModule(module);
    loadPanel(asset::plugin(pluginInstance, "res/MiniLog.svg"));

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
}

void MiniLogWidget::createStatusLight(MiniLog* module) {
    addChild(createLightCentered<SmallLight<GreenLight>>(
        findNamed("Light").value(), module, MiniLog::STATUS_LIGHT
    ));
}

void MiniLogWidget::createLogWidget(MiniLog* module) {
    log = createWidget<TextLogWidget>(mm2px(Vec(0, 11.f)));
    log->box.size = mm2px(Vec(40.64, 128.5f - 11.f - 4.f));
    addChild(log);
}