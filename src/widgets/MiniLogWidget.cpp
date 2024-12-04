#include "widgets/MiniLogWidget.hpp"

MiniLogWidget::MiniLogWidget(MiniLog* module) : SvgHelper<MiniLogWidget>() {
    DEBUG("MiniLogWidget: Starting construction");
    setModule(module);
    DEBUG("MiniLogWidget: Did setModule()");
    loadPanel(asset::plugin(pluginInstance, "res/MiniLog.svg"));
    DEBUG("MiniLogWidget: Did loadPanel()");

    addChild(createLightCentered<SmallLight<GreenLight>>(
        findNamed("Light").value(), 
        module, 
        MiniLog::STATUS_LIGHT
    ));
    DEBUG("MiniLogWidget: Did createLightCentered()");

    log = createWidget<TextLogWidget>(mm2px(Vec(0, 11.f)));
    log->box.size = mm2px(Vec(40.64, 128.5f - 11.f - 4.f));
    addChild(log);
    DEBUG("MiniLogWidget: Did createWidget()");

    if (module) {
        module->whenReinit([this]() {
            log->clear();
            dirty = true;
        });
        DEBUG("MiniLogWidget: Did whenReinit()");
    }

    DEBUG("MiniLogWidget: Construction complete");
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
