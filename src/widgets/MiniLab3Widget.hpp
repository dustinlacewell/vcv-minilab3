#include "MiniLab3.hpp"
#include "widgets/BaseWidget.hpp"
#include "ui/SvgHelper.hpp"

//template struct BaseWidget<MiniLab3>;

struct MiniLab3Widget : BaseWidget<MiniLab3>, SvgHelper<MiniLab3Widget> {
    explicit MiniLab3Widget(MiniLab3* module);
    void step() override;
    void appendContextMenu(Menu* menu) override;
};