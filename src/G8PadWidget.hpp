#include "BaseWidget.hpp"
#include "G8Pad.hpp"
#include "ui/LedText.hpp"

//template struct BaseWidget<G8Pad>;

struct G8PadWidget : BaseWidget<G8Pad> {
    LedTextDisplay* padIdText;

    explicit G8PadWidget(G8Pad* module);

    void step() override;
    void appendContextMenu(Menu* menu) override;
};