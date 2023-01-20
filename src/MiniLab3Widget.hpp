#include "BaseWidget.hpp"
#include "MiniLab3.hpp"

//template struct BaseWidget<MiniLab3>;

struct MiniLab3Widget : BaseWidget<MiniLab3> {
    explicit MiniLab3Widget(MiniLab3* module);
    ~MiniLab3Widget() override;

    void step() override;
    void appendContextMenu(Menu* menu) override;
};