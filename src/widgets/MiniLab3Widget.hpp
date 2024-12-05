#include "MiniLab3.hpp"
#include "ui/SvgHelper.hpp"
#include "widgets/BaseWidget.hpp"

struct MiniLab3Widget : BaseWidget<MiniLab3, MiniLab3Widget> {
    explicit MiniLab3Widget(MiniLab3* module);
    void step() override;
    void appendContextMenu(Menu* menu) override;

   private:
    void createMidiSelector(MiniLab3* module);
    void createStatusLight(MiniLab3* module);
    void createGatePort(MiniLab3* module);
    void createBendPort(MiniLab3* module);
    void createModPort(MiniLab3* module);
    void createKnobPorts(MiniLab3* module);
    void createSliderPorts(MiniLab3* module);
};