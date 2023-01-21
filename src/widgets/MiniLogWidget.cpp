#include "widgets/MiniLogWidget.hpp"

MiniLogWidget::MiniLogWidget(MiniLog* module) {
    setModule(module);
    setPanel(createPanel(asset::plugin(pluginInstance, "res/MiniLog.svg")));

    addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0))
    );
    addChild(createWidget<ScrewSilver>(
        Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)
    ));
    addChild(createWidget<ScrewSilver>(Vec(
        box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH
    )));
    addChild(createLight<SmallLight<GreenLight>>(
        mm2px(Vec(31.233, 7.894)), module, MiniLog::CONNECTED_LIGHT
    ));

    log = createWidget<TextLogWidget>(mm2px(Vec(0, 13.f)));
    log->box.size = mm2px(Vec(40.64, 128.5f - 13.f - 5.f));
    addChild(log);

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
