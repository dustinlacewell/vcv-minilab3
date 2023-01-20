#include <rack.hpp>

#include "MiniLog.hpp"
#include "ui/TextLogWidget.hpp"

struct MiniLogWidget : ModuleWidget {
    TextLogWidget* log;
    bool dirty = true;

    explicit MiniLogWidget(MiniLog* module);

    void step() override;
};