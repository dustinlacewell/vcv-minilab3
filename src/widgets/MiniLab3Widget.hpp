#include "MiniLab3.hpp"
#include "widgets/BaseWidget.hpp"

//template struct BaseWidget<MiniLab3>;

struct MiniLab3Widget : BaseWidget<MiniLab3> {
    explicit MiniLab3Widget(MiniLab3* module);
    void step() override;
    void appendContextMenu(Menu* menu) override;
};