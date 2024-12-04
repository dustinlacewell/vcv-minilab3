#include <rack.hpp>

#include "MiniLog.hpp"
#include "widgets/BaseWidget.hpp"
#include "ui/TextLogWidget.hpp"
#include "ui/SvgHelper.hpp"

struct MiniLogWidget : BaseWidget<MiniLog>, SvgHelper<MiniLogWidget> {
    TextLogWidget* log;
    bool dirty = true;

    explicit MiniLogWidget(MiniLog* module);

    void step() override;
};