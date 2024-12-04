#include "G8Pad.hpp"
#include "ui/LedText.hpp"
#include "widgets/BaseWidget.hpp"
#include "ui/SvgHelper.hpp"


//template struct BaseWidget<G8Pad>;

struct G8PadWidget : BaseWidget<G8Pad>, SvgHelper<G8PadWidget> {
    LedTextDisplay* padIdText;

    explicit G8PadWidget(G8Pad* module);

    void step() override;
    void appendContextMenu(Menu* menu) override;
};