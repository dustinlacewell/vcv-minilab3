#include "MiniLab.hpp"
#include "ui/SvgHelper.hpp"
#include "ui/FancyPanelBorder.hpp"
#include "widgets/BaseWidget.hpp"

struct MiniLabWidget : BaseWidget<MiniLab, MiniLabWidget> {
    explicit MiniLabWidget(MiniLab* module);
    void step() override;
    void appendContextMenu(Menu* menu) override;

   private:
    void createMidiSelector(MiniLab* module);
    void createStatusLight(MiniLab* module);
    void createGatePort(MiniLab* module);
    void createBendPort(MiniLab* module);
    void createModPort(MiniLab* module);
    void createKnobPorts(MiniLab* module);
    void createSliderPorts(MiniLab* module);
};